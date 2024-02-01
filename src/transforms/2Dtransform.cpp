#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <shader.h>
#include <framework.h>
#include <OpenGlObjects.h>

#include <imgui.h>
#include <imgui_internal.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#define S_WIDTH  1280
#define S_HEIGHT 720

int main() {
    auto* window = Framework::CreateWindow(S_WIDTH, S_HEIGHT);

    Shader shader(
        res_dir + "/shaders/vertex.glsl", 
        res_dir + "/shaders/fragment.glsl"
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

    // unsigned int VBO, VAO;
    // glGenVertexArrays(1, &VAO);
    // glGenBuffers(1, &VBO);
    // glBindVertexArray(VAO);

    // glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
    // glEnableVertexAttribArray(1);

    // glBindBuffer(GL_ARRAY_BUFFER, 0); 

    // glBindVertexArray(0); 

    VAO.Unbind();
    VBO.Unbind();

    float t_x = 0.0;
    float t_y = 0.0;
    float angle = 0.0;
    float scale = 1.0;
    glm::vec2 shear_x{};
    glm::vec2 shear_y{};

    float color[] = {128, 128, 128};

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        Framework::ProcessImGui([&] {
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
        });

        {
            shader.Use();
            
            const auto T = Transforms::GetTranslationMatrix(t_x, t_y, 0.0f);
            const auto R = Transforms::GetRotationMatrix(angle, glm::vec3{0.0f, 0.0f, 1.0f});
            const auto S = Transforms::GetUniformScaleMatrix(scale);
            const auto H = Transforms::GetShearMatrix(shear_x, shear_y, glm::vec2{});

            // TRS order
            const auto model = T * R * S * H;

            shader.SetMat4("model", model);
            shader.SetVec3("in_color", color[0], color[1], color[2]);

            VAO.Bind();
            glDrawArrays(GL_TRIANGLES, 0, 6);
            // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }

    ImGui_ImplGlfw_Shutdown();
    //ImGui::DestroyContext(); // TODO: Add terminate function

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
