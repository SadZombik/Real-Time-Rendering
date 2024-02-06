#define GLM_ENABLE_EXPERIMENTAL
#include <Framework.h>
#include <Utils.h>
#include <Shader.h>
#include <OpenGlObjects.h>
#include <CameraController.h>

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include <imgui.h>
#include <imgui_internal.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#define S_WIDTH 1280
#define S_HEIGHT 720

void WindowSizeCallback(GLFWwindow* window, int width, int height) {
    CameraController* cam = static_cast<CameraController*>(glfwGetWindowUserPointer(window));
    cam->SetRatio((float)width / (float)height);
}

// Cube
constexpr float vertices[] = {
     0.5f,  0.5f, 0.0f,  // top right
     0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f
};

constexpr size_t NumVertices = sizeof(vertices) / sizeof(float) / 3;

constexpr unsigned int indices[] = {
    0, 1, 3,
    1, 2, 3
};

struct {
    float x_m = 10.0f;
    float y_m = 15.0f;
    float z_m = 30.0f;
    float x_q = 10.0f;
    float y_q = 15.0f;
    float z_q = 30.0f;
} Rotations;

int main() {
    auto* window = Framework::CreateWindow(S_WIDTH, S_HEIGHT, "Quaternoins Basics");
    Framework::EnableDepthTest();

    CameraController cam(S_WIDTH, S_HEIGHT);
    glfwSetWindowUserPointer(window, &cam);
    glfwSetWindowSizeCallback(window, WindowSizeCallback);

    Shader shader_m(
        res_dir + "/shaders/quat/vertex.glsl",
        res_dir + "/shaders/quat/fragment.glsl"
    );

    Shader shader_q(
        res_dir + "/shaders/quat/vertex.glsl",
        res_dir + "/shaders/quat/fragment.glsl"
    );

    VertexArrayObject vao;
    VertexBufferObject vbo;
    ElementsBufferObject ebo;
    vao.Bind();
    vbo.Bind();

    vbo.SetData(sizeof(vertices), vertices);
    ebo.Bind();
    ebo.SetData(sizeof(indices), indices);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    while (!glfwWindowShouldClose(window)) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		    glfwSetWindowShouldClose(window, true);
        }

        cam.Update();
        cam.KeyboardCallback(window);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        Framework::ImGuiCallback([&] {
            ImGui::Begin("Settings");
            ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

            ImGui::Text("Quaternion Rotation");
            ImGui::SliderFloat("X_q", &Rotations.x_q, -180.0f, 180.0f);
            ImGui::SliderFloat("Y_q", &Rotations.y_q, -180.0f, 180.0f);
            ImGui::SliderFloat("Z_q", &Rotations.z_q, -180.0f, 180.0f);

            ImGui::Text("Matrix Rotation");
            ImGui::SliderFloat("X_m", &Rotations.x_m, -180.0f, 180.0f);
            ImGui::SliderFloat("Y_m", &Rotations.y_m, -180.0f, 180.0f);
            ImGui::SliderFloat("Z_m", &Rotations.z_m, -180.0f, 180.0f);

            ImGui::End();
        });

        {
            const auto model_q = glm::mat4_cast(
                glm::angleAxis(glm::radians(Rotations.z_q), glm::vec3(0.0f, 0.0f, 1.0f)) *
                glm::angleAxis(glm::radians(Rotations.y_q), glm::vec3(0.0f, 1.0f, 0.0f)) *
                glm::angleAxis(glm::radians(Rotations.x_q), glm::vec3(1.0f, 0.0f, 0.0f))
            );

            shader_q.Use();
            shader_q.SetMat4("projection", cam.GetPerspectiveMatrix());
            shader_q.SetMat4("view", cam.GetViewMatrix());
            shader_q.SetMat4("model", model_q);

            vao.Bind();
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            vao.Unbind();
        }

        {
            auto model_m = 
                Transforms::GetRotationMatrix(Rotations.z_m, glm::vec3(0.0f, 0.0f, 1.0f)) *
                Transforms::GetRotationMatrix(Rotations.y_m, glm::vec3(0.0f, 1.0f, 0.0f)) *
                Transforms::GetRotationMatrix(Rotations.x_m, glm::vec3(1.0f, 0.0f, 0.0f));

            model_m = Transforms::GetTranslationMatrix(1.5f, 0.0f, 0.0f) * model_m;

            shader_m.Use();
            shader_m.SetMat4("projection", cam.GetPerspectiveMatrix());
            shader_m.SetMat4("view", cam.GetViewMatrix());
            shader_m.SetMat4("model", model_m);

            vao.Bind();
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            vao.Unbind();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    Framework::Shutdown();

    return 0;
}
