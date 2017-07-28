#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <cmath>

namespace csg {

    // camera for orbitting about a single point
    class OrbitCamera {
        
        public:
        OrbitCamera(float distance=10.0, float ms=0.01, float zs=0.03) :
            distance(distance), mouseSensitivity(ms), zoomSensitivity(zs) {

            distance = 10.0;
            theta = 0.0;
            phi = 0.0;
        }

        glm::mat4 getCameraMatrix() {
            glm::vec3 center(0.0, 0.0, 0.0);
            glm::vec3 up(0.0, 0.0, 1.0);
            return glm::lookAt(getCameraPos(), center, up);
        }

        glm::vec3 getCameraPos() { 
            return glm::vec3(
                distance * std::sin(theta) * std::cos(phi),
                distance * std::sin(theta) * std::sin(phi),
                distance * std::cos(theta)
            );
        }

        void setAngles(float _theta, float _phi) {
            theta = _theta;
            phi = _phi;
        }

        static void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
            OrbitCamera *c = (OrbitCamera*) glfwGetWindowUserPointer(window);
            if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
                glfwGetCursorPos(window, &(c->prevPosX), &(c->prevPosY));
            }
        }

        static void cursorPosCallback(GLFWwindow *window, double xpos, double ypos) {
            if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
                OrbitCamera *c = (OrbitCamera*) glfwGetWindowUserPointer(window);
                float deltaX = xpos - c->prevPosX;
                float deltaY = ypos - c->prevPosY;

                c->theta += c->mouseSensitivity * deltaY;
                c->phi += c->mouseSensitivity * -deltaX;

                if (c->theta > -0.0001) c->theta = -0.0001;
                if (c->theta < -M_PI+0.0001) c->theta = -M_PI+0.0001;

                c->prevPosX = xpos;
                c->prevPosY = ypos;
            }
        }

        static void scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
            OrbitCamera *c = (OrbitCamera*) glfwGetWindowUserPointer(window);
            c->distance += c->zoomSensitivity * -yoffset;
        }

        float theta, phi;
        private:
        float distance;
        float mouseSensitivity, zoomSensitivity;

        double prevPosX, prevPosY;
    };

}