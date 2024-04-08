#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Shader.h>
#include <Framework.h>
#include <OpenGlObjects.h>

#include <imgui.h>
#include <imgui_internal.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

int main() {
    auto* window = Framework::CreateWindow("2D Transforms");

    Shader shader(
        res_dir + "/shaders/2d/vertex.glsl", 
        res_dir + "/shaders/2d/fragment.glsl"
    );

    float vertices[] = {
        // positions
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    VertexBufferObject VBO;
    VertexArrayObject VAO;

    VAO.Bind();

    VBO.Bind();
    VBO.SetData(sizeof(vertices), vertices);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    VAO.Unbind();
    VBO.Unbind();

    float t_x = 0.0f;
    float t_y = 0.0f;
    float angle = 0.0f;
    float scale = 1.0f;
    glm::vec2 shear_x{};
    glm::vec2 shear_y{};

    float color[3] = { 128, 128, 128 };

    while (!glfwWindowShouldClose(window)) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		    glfwSetWindowShouldClose(window, true);
        }

        Framework::ClearBuffer();

        Framework::ImGuiCallback([&] {
            ImGui::Begin("Settings");
            ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            
            ImGui::Text("Translation");
            ImGui::SliderFloat("Tx", &t_x, -1.0f, 1.0f);
            ImGui::SliderFloat("Ty", &t_y, -1.0f, 1.0f);
            
            ImGui::Text("Rotation");
            ImGui::SliderFloat("Angle", &angle, -IM_PI, IM_PI);
            
            ImGui::Text("Uniform scaling");
            ImGui::SliderFloat("scale", &scale, -2.0f, 2.0f);
            
            ImGui::Text("Shearing");
            ImGui::SliderFloat("x", &shear_x.x, -1.0f, 1.0f);
            ImGui::SliderFloat("y", &shear_y.x, -1.0f, 1.0f);

            ImGui::Text("Color");
            ImGui::ColorPicker3("color", color);
            ImGui::End();
        });

        {
            shader.Use();
            
            const auto T = Transforms::GetTranslationMatrix(t_x, t_y, 0.0f);
            const auto R = Transforms::GetRotationMatrix(angle, glm::vec3{0.0f, 0.0f, 1.0f});
            const auto S = Transforms::GetUniformScaleMatrix(scale);
            const auto H = Transforms::GetShearMatrix(shear_x, shear_y, glm::vec2{});

            // TRS order
            const auto model = T * R * H * S;

            shader.SetMat4("model", model);
            shader.SetVec3("in_color", color[0], color[1], color[2]);

            VAO.Bind();
            glDrawArrays(GL_TRIANGLES, 0, 3);
        }
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    Framework::Shutdown();
    return 0;
}
