#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Framework.h>
#include <Shader.h>
#include <OpenGlObjects.h>
#include <CameraController.h>

#include <imgui.h>
#include <imgui_internal.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <shapes/Pyramid.h>

#define S_WIDTH 1280
#define S_HEIGHT 720

void WindowSizeCallback(GLFWwindow* window, int width, int height) {
    CameraController* cam = static_cast<CameraController*>(glfwGetWindowUserPointer(window));
    cam->SetRatio((float)width / (float)height);
}

int main() {
    GLFWwindow* window = Framework::CreateWindow(S_WIDTH, S_HEIGHT, "3D Transforms");
    Framework::EnableDepthTest();

    CameraController cam(S_WIDTH, S_HEIGHT);
    glfwSetWindowUserPointer(window, &cam);
    glfwSetWindowSizeCallback(window, WindowSizeCallback);

    Pyramid pyramid;

    float t_x = 0.0f;
    float t_y = 0.0f;
    float t_z = 0.0f;
    float angle_x = 0.0f;
    float angle_y = 0.0f;
    float angle_z = 0.0f;
    float scale = 1.0f;
    glm::vec2 shear_x{};
    glm::vec2 shear_y{};
    glm::vec2 shear_z{};

    bool coloring = true;
    float color[3] = { 128, 128, 128 };

    constexpr float pi_deg = glm::degrees(IM_PI);

    while (!glfwWindowShouldClose(window)) {
        cam.Update();
        cam.KeyboardCallback(window);

        Framework::ClearBuffer();

        Framework::ImGuiCallback([&] {
            ImGui::Begin("Settings");
            ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

            ImGui::Text("Translation");
            ImGui::SliderFloat("Tx", &t_x, -1.0f, 1.0f);
            ImGui::SliderFloat("Ty", &t_y, -1.0f, 1.0f);
            ImGui::SliderFloat("Tz", &t_z, -1.0f, 1.0f);

            ImGui::Text("Rotation");
            ImGui::SliderFloat("Angle X", &angle_x, -pi_deg, pi_deg);
            ImGui::SliderFloat("Angle Y", &angle_y, -pi_deg, pi_deg);
            ImGui::SliderFloat("Angle Z", &angle_z, -pi_deg, pi_deg);

            ImGui::Text("Uniform scaling");
            ImGui::SliderFloat("Scale", &scale, -2.0f, 2.0f);

            ImGui::Text("Shearing");
            ImGui::SliderFloat("shear_x.x", &shear_x.x, -1.0f, 1.0f);
            ImGui::SliderFloat("shear_x.y", &shear_x.y, -1.0f, 1.0f);

            ImGui::SliderFloat("shear_y.x", &shear_y.x, -1.0f, 1.0f);
            ImGui::SliderFloat("shear_y.y", &shear_y.y, -1.0f, 1.0f);

            ImGui::SliderFloat("shear_z.x", &shear_z.x, -1.0f, 1.0f);
            ImGui::SliderFloat("shear_z.y", &shear_z.y, -1.0f, 1.0f);

            if (ImGui::Button(coloring ? "Disable Coloring" : "Enable Coloring")) {
                coloring = !coloring;

                if (coloring) {
                    pyramid.SetShaders(
                        res_dir + "/shaders/3d/vertex.glsl",
                        res_dir + "/shaders/3d/fragment.glsl"
                    );
                } else {
                    pyramid.SetShaders(
                        res_dir + "/shaders/3d/vertex_no_color.glsl",
                        res_dir + "/shaders/3d/fragment_no_color.glsl"
                    );
                }
            }

            if (coloring) {
                ImGui::Text("Color");
                ImGui::ColorPicker3("color", color);
            }

            ImGui::End();
        });

        {
            const auto T = Transforms::GetTranslationMatrix(t_x, t_y, t_z);
            const auto R_x = Transforms::GetRotationMatrix(angle_x, glm::vec3{1.0f, 0.0f, 0.0f});
            const auto R_y = Transforms::GetRotationMatrix(angle_y, glm::vec3{0.0f, 1.0f, 0.0f});
            const auto R_z = Transforms::GetRotationMatrix(angle_z, glm::vec3{0.0f, 0.0f, 1.0f});
            const auto S = Transforms::GetUniformScaleMatrix(scale);
            const auto H = Transforms::GetShearMatrix(shear_x, shear_y, shear_z);

            const auto model = T * R_x * R_y * R_z * H * S;

            pyramid.SetModelMatrix(model);
            pyramid.SetColor(color);
            pyramid.Update(cam);
        }
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    Framework::Shutdown();
    return 0;
}
