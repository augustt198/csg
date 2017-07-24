#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include <cstdio>
#include <vector>

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

void render_node(csg::node::Node *n, int *id_counter);
csg::node::Type render_add_menu(const char *popup_id);
void render_add_node(csg::node::Node **nodePtr, int *id_counter);
csg::node::Node *create_node_from_type(csg::node::Type type);

float testthingy(float x, float y, float z) {
    return x + y + z;
}

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

    csg::mc::MarchingCubes mcInstance(-1, -1, -1, 1, 1, 1, 0.015, 0.015, 0.015);

    csg::ShaderProgram shader("shaders/shader.vert", "shaders/shader.frag");
    csg::Mesh *mesh = nullptr;

    csg::OrbitCamera camera;

    while (!glfwWindowShouldClose(win)) {
        glfwPollEvents();
        ImGui_ImplGlfwGL3_NewFrame();

        glClearColor(0.2, 0.2, 0.2, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (mesh) {
            glUseProgram(shader.program);
            camera.setAngles(glfwGetTime()/2, glfwGetTime()/2);
            shader.setMat4("cameraDir", camera.getCameraMatrix());

            int width, height;
            glfwGetWindowSize(win, &width, &height);
            float aspect = (float) width / height;
            glm::mat4 perspective = glm::perspective(0.5f, aspect, 0.001f, 1000.0f);
            shader.setMat4("perspective", perspective);

            mesh->render();
            glUseProgram(0);
        }

        ImGui::Begin("CSG");
        ImGui::Text("Geometry Tree");
        ImGui::SameLine(ImGui::GetWindowWidth()-65);
        if (ImGui::Button("Render") && rootNode != NULL) {
            std::vector<csg::Vertex> *verts = mcInstance.isosurface(*rootNode, 0.0);
            std::printf("Isosurface created. Vert count: %lu\n", verts->size());

            if (verts->size() > 0) {
                if (mesh) {
                    mesh->updateVertices(*verts);
                } else {
                    mesh = new csg::Mesh(*verts);
                    for (int i = 0; i < 10; i++) {
                        csg::Vertex v = (*verts)[i];
                        std::printf("normal %f %f %f\n", v.normal.x, v.normal.y, v.normal.z);
                    }
                }
            }
        }
        ImGui::Separator();

        int id_counter = 0;
        if (rootNode != NULL) {
            render_node(rootNode, &id_counter);
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

void render_node(csg::node::Node *n, int *id_counter) {
    //std::printf("Called render node with %p\n", n);
    if (n == NULL) {
        ImGui::BulletText("Some NULL shet");
    } else if (n->type == csg::node::CSG_UNION) {
        csg::node::Union *unionNode = (csg::node::Union*) n;
        if (ImGui::TreeNode("Union")) {
            if (unionNode->left != NULL)
                render_node(unionNode->left, id_counter);
            else
                render_add_node(&(unionNode->left), id_counter);

            if (unionNode->right != NULL)
                render_node(unionNode->right, id_counter);
            else
                render_add_node(&(unionNode->right), id_counter);
            ImGui::TreePop();
        }
    } else if (n->type == csg::node::CSG_DIFFERENCE) {
        csg::node::Difference *diffNode = (csg::node::Difference*) n;
        if (ImGui::TreeNode("Difference")) {
            if (diffNode->left != NULL)
                render_node(diffNode->left, id_counter);
            else
                render_add_node(&(diffNode->left), id_counter);

            if (diffNode->right != NULL)
                render_node(diffNode->right, id_counter);
            else
                render_add_node(&(diffNode->right), id_counter);
            ImGui::TreePop();
        }
    } else if (n->type == csg::node::CSG_SPHERE) {
        csg::node::Sphere *sphere = (csg::node::Sphere*) n;

        if (sphere->isLinked) {
            // update proportions
            sphere->prop1 = sphere->ax1;
            sphere->prop2 = sphere->ax2;
            sphere->prop3 = sphere->ax3;
        }

        
        float ax1_prev = sphere->ax1,
              ax2_prev = sphere->ax2,
              ax3_prev = sphere->ax3;

        ImGui::BulletText("Sphere");
        ImGui::Indent();
        ImGui::DragFloat3("###Radius", &(sphere->ax1), 0.02, 0.001f, 1000.0);
        ImGui::SameLine();
        ImGui::Checkbox("###checx", &(sphere->isLinked));

        if (sphere->isLinked) {
            if (ax1_prev != sphere->ax1) { // ax1 changed
                sphere->ax2 = sphere->ax1 * (sphere->prop2 / sphere->prop1);
                sphere->ax3 = sphere->ax1 * (sphere->prop3 / sphere->prop1);
            } else if (ax2_prev != sphere->ax2) {
                sphere->ax1 = sphere->ax2 * (sphere->prop1 / sphere->prop2);
                sphere->ax3 = sphere->ax2 * (sphere->prop3 / sphere->prop2);
            } else if (ax3_prev != sphere->ax3) {
                sphere->ax1 = sphere->ax3 * (sphere->prop1 / sphere->prop3);
                sphere->ax2 = sphere->ax3 * (sphere->prop2 / sphere->prop3);
            }
        }

        ImGui::Unindent();
    } else if (n->type == csg::node::CSG_CUBE) {
        ImGui::BulletText("Cube");
    } else {
        ImGui::BulletText("Unknown!");
    }
}

void render_add_node(csg::node::Node **nodePtr, int *id_counter) {
    ImGui::Bullet();
    ImGui::SameLine();

    (*id_counter)++;
    char id_str[15] = {0};
    char button_str[30] = {0};
    std::sprintf(id_str, "%d", *id_counter);
    std::sprintf(button_str, " + Add ###%d", *id_counter);

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
        csg::node::CSG_DIFFERENCE, csg::node::CSG_SPHERE, csg::node::CSG_CUBE};
    const char *typeNames[] = {"Union", "Intersection", "Difference", "Sphere", "Cube"};
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
    } else {
        return NULL;
    }
}
