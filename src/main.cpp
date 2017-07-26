#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include <cstdio>
#include <vector>
#include <fstream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"

#include "node.h"
#include "mc.h"
#include "shader.h"
#include "mesh.h"
#include "camera.h"
#include "glmutil.h"
#include "customgui.h"

#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))

GLFWwindow *create_window() {
    int width = 1100;
    int height = 720;
    const char *title = "CSG";

    return glfwCreateWindow(width, height, title, NULL, NULL);
}

void error_callback(int error, const char *desc) {
    std::printf("[GLFW ERROR] %s (%d)\n", desc, error);
}

void render_node(csg::node::Node *n);
csg::node::Type render_add_menu(const char *popup_id);
void render_add_node(csg::node::Node **nodePtr, int id, int sub_id);
csg::node::Node *create_node_from_type(csg::node::Type type);

int main(int argc, char **argv) {
    if (!glfwInit()) {
        std::printf("Failed to init GLFW\n");
        return -1;
    }

    glfwSetErrorCallback(error_callback);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 
    GLFWwindow *win = create_window();
    if (!win) {
        std::printf("Couldn't create GLFW window\n");
        return -1;
    }

    ImGui_ImplGlfwGL3_Init(win, true);

    glfwMakeContextCurrent(win);
    glfwSwapInterval(1);
    
    if (gl3wInit()) {
        std::printf("Failed to init OpenGL\n");
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF("Karla-Regular.ttf", 18.0f, NULL, NULL);

    std::printf("OpenGL %s\n", glGetString(GL_VERSION));
    std::printf("GLSL %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

    csg::node::Node *rootNode = NULL;

    csg::ShaderProgram shader("shaders/shader.vert", "shaders/shader.frag");
    csg::Mesh *mesh = nullptr;

    csg::OrbitCamera camera;
    glfwSetWindowUserPointer(win, &camera);
    glfwSetCursorPosCallback(win, csg::OrbitCamera::cursorPosCallback);
    glfwSetMouseButtonCallback(win, csg::OrbitCamera::mouseButtonCallback);
    glfwSetScrollCallback(win, csg::OrbitCamera::scrollCallback);
    camera.setAngles(0.1, 0.1);

    while (!glfwWindowShouldClose(win)) {
        glfwPollEvents();
        ImGui_ImplGlfwGL3_NewFrame();

        glClearColor(0.2, 0.2, 0.2, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (mesh) {
            glUseProgram(shader.program);
            shader.setMat4("cameraDir", camera.getCameraMatrix());

            int width, height;
            glfwGetWindowSize(win, &width, &height);
            float aspect = (float) width / height;
            glm::mat4 perspective = glm::perspective(0.5f, aspect, 0.001f, 1000.0f);
            shader.setMat4("perspective", perspective);

            mesh->render();
            glUseProgram(0);
        }

        static int subdivisions = 200;
        ImGui::Begin("CSG");
        ImGui::Text("Geometry Tree");
        ImGui::SameLine(ImGui::GetWindowWidth()-65);
        if (ImGui::Button("Render") && rootNode != NULL) {
            csg::mc::MarchingCubes mcInstance(-1, -1, -1, 1, 1, 1, 2.0/subdivisions, 2.0/subdivisions, 2.0/subdivisions);
            std::vector<csg::Vertex> *verts = mcInstance.isosurface(*rootNode, 0.0);
            std::printf("Isosurface created. Vert count: %lu\n", verts->size());

            if (verts->size() > 0) {
                if (mesh) {
                    mesh->updateVertices(*verts);
                } else {
                    mesh = new csg::Mesh(*verts);
                }

                /*
                std::ofstream stlFile("mesh.stl");
                mesh->writeSTL(stlFile);
                stlFile.close();
                */
            }
        }
        if (ImGui::TreeNode("Render Options")) {
            ImGui::Text("Camera: theta %.3f / phi %.3f", camera.theta, camera.phi);
            ImGui::DragInt("Subdivisions", &subdivisions, 0.5, 20, 1000);
            ImGui::TreePop();
        }
        ImGui::Separator();

        if (rootNode != NULL) {
            render_node(rootNode);
        } else {
            if (ImGui::Button(" + Add "))
                ImGui::OpenPopup("add");
            csg::node::Type selected = render_add_menu("add");
            if (selected != csg::node::CSG_NONE) {
                rootNode = create_node_from_type(selected);
            }
        }

        ImGui::End();

        ImGui::Render();

        glfwSwapBuffers(win);
    }

    glfwTerminate();
}

void render_node(csg::node::Node *n) {
    if (n == NULL) {
        ImGui::BulletText("Some NULL shet");
    } else if (n->type == csg::node::CSG_UNION) {
        csg::node::Union *unionNode = (csg::node::Union*) n;
        if (ImGui::TreeNode(unionNode->idString("Union###%d"))) {
            if (unionNode->left != NULL)
                render_node(unionNode->left);
            else
                render_add_node(&(unionNode->left), unionNode->id, 1);

            if (unionNode->right != NULL)
                render_node(unionNode->right);
            else
                render_add_node(&(unionNode->right), unionNode->id, 2);
            ImGui::TreePop();
        }
    } else if (n->type == csg::node::CSG_DIFFERENCE) {
        csg::node::Difference *diffNode = (csg::node::Difference*) n;
        if (ImGui::TreeNode(diffNode->idString("Difference###%d"))) {
            if (diffNode->left != NULL)
                render_node(diffNode->left);
            else
                render_add_node(&(diffNode->left), diffNode->id, 1);

            if (diffNode->right != NULL)
                render_node(diffNode->right);
            else
                render_add_node(&(diffNode->right), diffNode->id, 2);
            ImGui::TreePop();
        }
    } else if (n->type == csg::node::CSG_INTERSECTION) {
        csg::node::Intersection *interNode = (csg::node::Intersection*) n;
        if (ImGui::TreeNode(interNode->idString("Intersection###%d"))) {
            if (interNode->left != NULL)
                render_node(interNode->left);
            else
                render_add_node(&(interNode->left), interNode->id, 1);

            if (interNode->right != NULL)
                render_node(interNode->right);
            else
                render_add_node(&(interNode->right), interNode->id, 2);
            ImGui::TreePop();
        }
    } else if (n->type == csg::node::CSG_SPHERE) {
        csg::node::Sphere *sphere = (csg::node::Sphere*) n;
        ImGui::BulletText("Sphere");
        ImGui::Indent();
        const char *label = sphere->idString("###%d");
        const char *fmts[3] = {"A: %.3f", "B: %.3f", "C: %.3f"};
        csg::gui::LinkedDragFloat3(label, &(sphere->ax1), &(sphere->isLinked), fmts);
        ImGui::Unindent();
    } else if (n->type == csg::node::CSG_CUBE) {
        csg::node::Cube *cube = (csg::node::Cube*) n;
        ImGui::BulletText("Cube");
        ImGui::Indent();
        const char *label = cube->idString("###%d");
        const char *fmts[3] = {"X: %.3f", "Y: %.3f", "Z: %.3f"};
        csg::gui::LinkedDragFloat3(label, &(cube->ax1), &(cube->isLinked), fmts);
        ImGui::Unindent();
    } else if (n->type == csg::node::CSG_TRANSLATE) {
        csg::node::Translate *translate = (csg::node::Translate*) n;
        ImGui::BulletText("Translate");
        ImGui::Indent();
        const char *label = translate->idString("###%d");
        const char *fmts[3] = {"X: %.3f", "Y: %.3f", "Z: %.3f"};
        csg::gui::LinkedDragFloat3(label, &(translate->dx), &(translate->isLinked), fmts);

        if (translate->node) {
            render_node(translate->node);
        } else {
            render_add_node(&(translate->node), translate->id, 1);
        }
    } else if (n->type == csg::node::CSG_SCALE) {
        csg::node::Scale *scale = (csg::node::Scale*) n;
        ImGui::BulletText("Scale");
        ImGui::Indent();
        const char *label = scale->idString("###%d");
        const char *fmts[3] = {"SX: %.3f", "SY: %.3f", "SZ: %.3f"};
        csg::gui::LinkedDragFloat3(label, &(scale->sx), &(scale->isLinked), fmts);

        if (scale->node) {
            render_node(scale->node);
        } else {
            render_add_node(&(scale->node), scale->id, 1);
        }
    }
}

void render_add_node(csg::node::Node **nodePtr, int id, int sub_id) {
    ImGui::Bullet();
    ImGui::SameLine();

    char id_str[15] = {0};
    char button_str[30] = {0};
    std::sprintf(id_str, "%d_%d", id, sub_id);
    std::sprintf(button_str, " + Add ###%d_%d", id, sub_id);

    if (ImGui::Button(button_str))
        ImGui::OpenPopup(id_str);
    csg::node::Type selectedType = render_add_menu(id_str);
    if (selectedType != csg::node::CSG_NONE) {
        csg::node::Node *newNode = create_node_from_type(selectedType);
        *nodePtr = newNode;
    }
}

csg::node::Type render_add_menu(const char *popup_id) {
    csg::node::Type selected = csg::node::CSG_NONE;
    csg::node::Type types[] = {csg::node::CSG_UNION, csg::node::CSG_INTERSECTION,
        csg::node::CSG_DIFFERENCE, csg::node::CSG_SPHERE, csg::node::CSG_CUBE,
        csg::node::CSG_TRANSLATE, csg::node::CSG_SCALE};
    const char *typeNames[] = {"Union", "Intersection", "Difference", "Sphere", "Cube", "Translate", "Scale"};
    if (ImGui::BeginPopup(popup_id)) {
        for (int i = 0; i < IM_ARRAYSIZE(types); i++) {
            if (ImGui::Selectable(typeNames[i]))
                selected = types[i];
        }

        ImGui::EndPopup();
    }

    return selected;
}

csg::node::Node *create_node_from_type(csg::node::Type type) {
    if (type == csg::node::CSG_UNION) {
        return new csg::node::Union();
    } else if (type == csg::node::CSG_INTERSECTION) {
        return new csg::node::Intersection();
    } else if (type == csg::node::CSG_DIFFERENCE) {
        return new csg::node::Difference();
    } else if (type == csg::node::CSG_SPHERE) {
        return new csg::node::Sphere();
    } else if (type == csg::node::CSG_CUBE) {
        return new csg::node::Cube();
    } else if (type == csg::node::CSG_TRANSLATE) {
        return new csg::node::Translate();
    } else if (type == csg::node::CSG_SCALE) {
        return new csg::node::Scale();
    } else {
        return NULL;
    }
}
