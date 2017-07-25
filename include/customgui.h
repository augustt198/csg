#pragma once

namespace csg {
    namespace gui {

        void LinkedDragFloat3(const char *label, float *v, bool *linked, const char *fmts[3], float v_speed = 0.01f, float v_min = 0.0f, float v_max = 0.0f);   

    }
}