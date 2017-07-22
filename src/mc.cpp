#include "mc.h"
#include "mc_data.h"

using namespace csg;

std::vector<Vec3> *mc::MarchingCubes::isosurface(mc::ScalarFieldFunc f, float w) {
    std::vector<Vec3> *verts = new std::vector<Vec3>();

    for (float x = minX; x <= maxX; x += stepX) {
        for (float y = minY; y < maxY; y += stepY) {
            for (float z = minZ; z < maxZ; z += stepZ) {
                float f0 = f(x, y, z),
                      f1 = f(x, y+stepY, z),
                      f2 = f(x+stepX, y+stepY, z),
                      f3 = f(x+stepX, y, z),
                      f4 = f(x, y, z+stepZ),
                      f5 = f(x, y+stepY, z+stepZ),
                      f6 = f(x+stepX, y+stepY, z+stepZ),
                      f7 = f(x+stepX, y, z+stepZ);

                int idx = 0;
                idx = (f0>w)<<0 | (f1>w)<<1 | (f2>w)<<2 | (f3>w)<<3 | (f4>w)<<4 | (f5>w)<<5 | (f6>w)<<6 | (f7>w)<<7;

                Vec3 vertlist[12];
                
                int edges = edgeTable[idx];
                if (edges & (1<<0))
                    vertlist[0] = lerp(w, f0, f1, Vec3(x, y, z), Vec3(x, y+stepY, z));
                if (edges & (1<<1))
                    vertlist[1] = lerp(w, f1, f2, Vec3(x, y+stepY, z), Vec3(x+stepX, y+stepY, z+stepZ));
                if (edges & (1<<2))
                    vertlist[2] = lerp(w, f2, f3, Vec3(x+stepX, y+stepY, z+stepZ), Vec3(x+stepX, y, z));
                if (edges & (1<<3))
                    vertlist[3] = lerp(w, f3, f0, Vec3(x+stepX, y, z), Vec3(x, y, z));
                if (edges & (1<<4))
                    vertlist[4] = lerp(w, f4, f5, Vec3(x, y, z+stepZ), Vec3(x, y+stepY, z+stepZ));
                if (edges & (1<<5))
                    vertlist[5] = lerp(w, f5, f6, Vec3(x, y+stepY, z+stepZ), Vec3(x+stepX, y+stepY, z+stepZ));
                if (edges & (1<<6))
                    vertlist[6] = lerp(w, f6, f7, Vec3(x+stepX, y+stepY, z+stepZ), Vec3(x+stepX, y, z+stepZ));
                if (edges & (1<<7))
                    vertlist[7] = lerp(w, f7, f4, Vec3(x+stepX, y, z+stepZ), Vec3(x, y, z+stepZ));
                if (edges & (1<<8))
                    vertlist[8] = lerp(w, f0, f4, Vec3(x, y, z), Vec3(x, y, z+stepZ));
                if (edges & (1<<9))
                    vertlist[9] = lerp(w, f1, f5, Vec3(x, y+stepY, z), Vec3(x, y+stepY, z+stepZ));
                if (edges & (1<<10))
                    vertlist[10] = lerp(w, f2, f6, Vec3(x+stepX, y+stepY, z), Vec3(x+stepX, y+stepY, z+stepZ));
                if (edges & (1<<11))
                    vertlist[11] = lerp(w, f3, f7, Vec3(x+stepX, y, z), Vec3(x+stepX, y, z+stepZ));

                for (int i = 0; triTable[idx][i] != -1; i++) {
                    verts->push_back(vertlist[triTable[idx][i]]);
                }
            }
        }
    }

    return verts;
}
