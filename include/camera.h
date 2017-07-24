#include <glm/core.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <cmath>

namespace csg {

    class OrbitCamera {
        
        public:
        Camera(float distance=5.0) : distance(distance) {
            theta = 0.0;
            phi = 0.0;
        }

        glm::mat4 getCameraMatrix() {
            glm::vec3 cameraPos(
                distance * std::sin(theta) * std::cos(phi),
                distance * std::sin(theta) * std::sin(phi),
                distance * std::cos(theta)
            );
            glm::vec3 center(0.0, 0.0, 0.0);
            glm::vec3 up(0.0, 0.0, 1.0);

            return glm::lookAt(cameraPos, center, up);
        }

        void setAngles(float _theta, float _phi) {
            theta = _theta;
            phi = _phi;
        }

        private:
        float distance;
        float theta, phi;

    }

}