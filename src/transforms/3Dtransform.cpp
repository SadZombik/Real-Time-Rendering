#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>

#include <Framework.h>
#include <Shader.h>
#include <OpenGlObjects.h>
#include <CameraController.h>

#include <imgui.h>
#include <imgui_internal.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#define S_WIDTH 1280
#define S_HEIGHT 720

// Pyramid
constexpr float vertices[] = {
    // base
        // first triangle
        // position           // tex coords
         0.5f, -0.5f,  0.5f,  1.0f, 1.0f, // top right
        -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, // top left 
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // bottom right

        // second triangle
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // bottom right
        -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, // top left
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom left

    // walls
        // right
         0.5f, -0.5f,  0.5f,  1.0f, 1.0f,
         0.0f,  0.5f,  0.0f,  0.5f, 0.5f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,

        // left
        -0.5f, -0.5f,  0.5f,  0.0f, 1.0f,
         0.0f,  0.5f,  0.0f,  0.5f, 0.5f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        // near
         0.5f, -0.5f,  0.5f,  1.0f, 1.0f,
         0.0f,  0.5f,  0.0f,  0.5f, 0.5f,
        -0.5f, -0.5f,  0.5f,  0.0f, 1.0f,

        // far
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.0f,  0.5f,  0.0f,  0.5f, 0.5f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f
};

constexpr size_t NumVertices = sizeof(vertices) / sizeof(float) / 5;

int main() {
    GLFWwindow* window = Framework::CreateWindow(S_WIDTH, S_HEIGHT, "3D Transforms");
    Framework::EnableDepthTest();

    CameraController cam(S_WIDTH, S_HEIGHT);

    Texture texture(res_dir + "/textures/3d/wood.png");
    texture.Bind(0);

    Shader shader(
        res_dir + "/shaders/3d/vertex.glsl", 
        res_dir + "/shaders/3d/fragment.glsl"
    );
    shader.Use();
    shader.SetInt("pyramyd_texture", 0);

    VertexBufferObject VBO;
    VertexArrayObject VAO;

    VAO.Bind();
    VBO.Bind();
    VBO.SetData(sizeof(vertices), vertices);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    VAO.Bind();

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

    float color[3] = { 128, 128, 128 };

    while (!glfwWindowShouldClose(window)) {
        cam.Update();
        cam.KeyboardCallback(window);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        Framework::ImGuiCallback([&] {
            ImGui::Begin("Settings");
            ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

            ImGui::Text("Translation");
            ImGui::SliderFloat("Tx", &t_x, -1.0f, 1.0f);
            ImGui::SliderFloat("Ty", &t_y, -1.0f, 1.0f);
            ImGui::SliderFloat("Tz", &t_z, -1.0f, 1.0f);

            ImGui::Text("Rotation");
            ImGui::SliderFloat("Angle X", &angle_x, -IM_PI, IM_PI);
            ImGui::SliderFloat("Angle Y", &angle_y, -IM_PI, IM_PI);
            ImGui::SliderFloat("Angle Z", &angle_z, -IM_PI, IM_PI);

            ImGui::Text("Uniform scaling");
            ImGui::SliderFloat("Scale", &scale, -2.0f, 2.0f);
            
            ImGui::Text("Shearing");
            ImGui::SliderFloat("shear_x.x", &shear_x.x, -1.0f, 1.0f);
            ImGui::SliderFloat("shear_x.y", &shear_x.y, -1.0f, 1.0f);

            ImGui::SliderFloat("shear_y.x", &shear_y.x, -1.0f, 1.0f);
            ImGui::SliderFloat("shear_y.y", &shear_y.y, -1.0f, 1.0f);

            ImGui::SliderFloat("shear_z.x", &shear_z.x, -1.0f, 1.0f);
            ImGui::SliderFloat("shear_z.y", &shear_z.y, -1.0f, 1.0f);

            ImGui::Text("Color");
            ImGui::ColorPicker3("color", color);
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

            shader.Use();
            shader.SetMat4("projection", cam.GetPerspectiveMatrix());
            shader.SetMat4("view", cam.GetViewMatrix());
            shader.SetMat4("model", model);
            shader.SetVec3("in_color", color[0], color[1], color[2]);

            VAO.Bind();
            glDrawArrays(GL_TRIANGLES, 0, NumVertices);
            VAO.Unbind();
        }
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    Framework::Shutdown();
    return 0;
}
