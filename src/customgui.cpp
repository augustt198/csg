#include "customgui.h"

#include "imgui.h"

void csg::gui::LinkedDragFloat3(const char *label, float *v, bool *linked, const char *fmts[3], float v_speed, float v_min, float v_max) {
    float prev0 = v[0], prev1 = v[1], prev2 = v[2];
    
    ImGui::PushID(label);

    const char *ids[3] = {"##x", "##y", "##z"};
    ImGui::PushItemWidth(80.0);
    for (int i = 0; i < 3; i++) {
        ImGui::DragFloat(ids[i], &(v[i]), v_speed, v_min, v_max, fmts[i]);
        ImGui::SameLine();
    }
    ImGui::PopItemWidth();

    ImGui::Checkbox("##checkbox", linked);

    if (*linked) {
        if (v[0] != prev0 && prev0 != 0.0) {
            v[1] = v[0] * prev1 / prev0;
            v[2] = v[0] * prev2 / prev0;
        } else if (v[1] != prev1 && prev1 != 0.0) {
            v[0] = v[1] * prev0 / prev1;
            v[2] = v[1] * prev2 / prev1;
        } else if (v[2] != prev2 && prev2 != 0.0) {
            v[0] = v[2] * prev0 / prev2;
            v[1] = v[2] * prev1 / prev2;
        }
    }

    ImGui::PopID();
}