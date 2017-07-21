#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include <cstdio>

#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"

#include "node.h"

#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))

GLFWwindow *create_window() {
    int width = 640;
    int height = 480;
    const char *title = "CSG";

    return glfwCreateWindow(width, height, title, NULL, NULL);
}

void error_callback(int error, const char *desc) {
    std::printf("[GLFW ERROR] %s (%d)\n", desc, error);
}

void render_node(csg::node::Node *n);

const char *render_add_menu(const char *popup_id);

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

    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF("ProggyTiny.ttf", 18.0f, NULL, NULL);

    std::printf("OpenGL %s\n", glGetString(GL_VERSION));
    std::printf("GLSL %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

    csg::node::Node *rootNode = NULL;

    while (!glfwWindowShouldClose(win)) {
        glfwPollEvents();
        ImGui_ImplGlfwGL3_NewFrame();

        glClearColor(0.2, 0.2, 0.2, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui::Begin("CSG");
        ImGui::Text("Geometry Tree");
        ImGui::Separator();

        if (rootNode != NULL) {
            render_node(rootNode);
        } else {
            if (ImGui::Button(" + Add "))
                ImGui::OpenPopup("add");
            const char *selected = render_add_menu("add");
            if (selected != NULL) {
                if (strcmp("Union", selected) == 0) {
                    rootNode = new csg::node::Union();
                } else if (strcmp("Difference", selected) == 0) {
                    rootNode = new csg::node::Difference();
                }
            }
        }

        ImGui::End();

        ImGui::Render();

        glfwSwapBuffers(win);
    }

    glfwTerminate();
}

void render_node(csg::node::Node *n) {
    //std::printf("Called render node with %p\n", n);
    if (n == NULL) {
        ImGui::Bullet();
        ImGui::SameLine();
        ImGui::Button(" + Add ");
    } else if (n->type == csg::node::CSG_UNION) {
        csg::node::Union *unionNode = (csg::node::Union*) n;
        if (ImGui::TreeNode("Union")) {
            render_node(unionNode->left);
            render_node(unionNode->right);
            ImGui::TreePop();
        }
    } else if (n->type == csg::node::CSG_DIFFERENCE) {
        csg::node::Difference *diffNode = (csg::node::Difference*) n;
        if (ImGui::TreeNode("Difference")) {
            render_node(diffNode->left);
            render_node(diffNode->right);
            ImGui::TreePop();
        }
    } else {
        ImGui::BulletText("whattup kids?");
    }
}

const char *render_add_menu(const char *popup_id) {
    const char *selected = NULL;
    const char *types[] = {"Union", "Intersection", "Difference", "Sphere"};
    if (ImGui::BeginPopup(popup_id)) {
        for (int i = 0; i < IM_ARRAYSIZE(types); i++) {
            if (ImGui::Selectable(types[i]))
                selected = types[i];
        }

        ImGui::EndPopup();
    }

    return selected;
}
