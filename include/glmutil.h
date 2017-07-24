#include <glm/glm.hpp>

#include <cstdio>

void printMat4(glm::mat4 m) {
    printf("mat4( ");
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++)
            printf("%.03f ", m[i][j]);

        if (i < 3)
            printf("\n      ");
        else
            printf(")\n");
    }
}