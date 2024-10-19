#include <Framework.h>
#include <shapes/Cylinder.h>

#include <unordered_map>

static const std::unordered_map<int, const char*> renderModes = {
    { GL_POINTS, "GL_POINTS"},
    { GL_LINE_STRIP, "GL_LINE_STRIP"},
    { GL_LINE_LOOP, "GL_LINE_LOOP"},
    { GL_LINES, "GL_LINES"},
    { GL_LINE_STRIP_ADJACENCY, "GL_LINE_STRIP_ADJACENCY"},
    { GL_LINES_ADJACENCY, "GL_LINES_ADJACENCY"},
    { GL_TRIANGLE_STRIP, "GL_TRIANGLE_STRIP"},
    { GL_TRIANGLE_FAN, "GL_TRIANGLE_FAN"},
    { GL_TRIANGLES, "GL_TRIANGLES"},
    { GL_TRIANGLE_STRIP_ADJACENCY, "GL_TRIANGLE_STRIP_ADJACENCY"},
    { GL_TRIANGLES_ADJACENCY, "GL_TRIANGLES_ADJACENCY"},
    { GL_PATCHES, "GL_PATCHES"}
};

int main() {
    auto* window = Framework::CreateWindow("Arm Animation");
    Framework::EnableDepthTest();

    CameraController cam(DEFAULT_WIDTH, DEFAULT_HEIGHT);
    cam.SetPitch(-30);

    Cylinder cylinder;

    float radius = 1.0f;
    float height = 1.0f;
    int num_vertices = 6;
    int num_instances = 1;

    int currentMode = GL_LINE_STRIP;
    const auto modesCount = renderModes.size();

    float color[3] = { 128, 128, 128 };
    float t_x = 0.0f;
    float t_y = 0.0f;
    float t_z = 0.0f;

    while (!glfwWindowShouldClose(window)) {
        Framework::ClearBuffer();

        cam.ArrowKeysCallback(window, ArrowKeysFunction::VIEW);
        cam.KeyboardCallback(window);
        cam.Update();

        Framework::ImGuiCallback([&] {
            if (ImGui::BeginListBox("##", ImVec2(-FLT_MIN, modesCount * ImGui::GetTextLineHeightWithSpacing()))) {
                for (const auto& it : renderModes) {
                    const bool isSelected = (currentMode == it.first);
                    if (ImGui::Selectable(it.second, isSelected)) {
                        currentMode = it.first;
                        cylinder.SetRenderMode(currentMode);
                    }
                    if (isSelected) {
                        ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndListBox();
            }

            ImGui::Text("Color");
            ImGui::ColorPicker3("color", color);

            ImGui::Text("Translation");
            ImGui::SliderFloat("Tx", &t_x, -1.0f, 1.0f);
            ImGui::SliderFloat("Ty", &t_y, -1.0f, 1.0f);
            ImGui::SliderFloat("Tz", &t_z, -1.0f, 1.0f);

            ImGui::Text("Geometry");
            if (ImGui::SliderFloat("Radius", &radius, 0.0f, 3.0f)   ||
                ImGui::SliderFloat("Height", &height, 0.0f, 10.0f)  ||
                ImGui::SliderInt("Vertices", &num_vertices, 4, 256) || 
                ImGui::SliderInt("Instances", &num_instances, 1, NUM_CYLINDER_INSTANCES)
                ) {
                cylinder.GenerateVertices(radius, height, num_vertices, num_instances);
            }
        });

        const auto T = Transforms::GetTranslationMatrix(t_x, t_y, t_z);

        for (int i = 0; i < NUM_CYLINDER_INSTANCES; i++) {
            cylinder.SetModelMatrix(T, i);
        }

        cylinder.SetColor(color);
        cylinder.Update(cam);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}
