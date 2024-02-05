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

int main() {
    auto* window = Framework::CreateWindow(S_WIDTH, S_HEIGHT, "Quaternoins Basics");
    Framework::EnableDepthTest();

    CameraController cam(S_WIDTH, S_HEIGHT);
    glfwSetWindowUserPointer(window, &cam);
    glfwSetWindowSizeCallback(window, WindowSizeCallback);

    Shader shader(
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

    float phi = 45.0f;
    const glm::vec3 u_q(1.0f, 0.0f, 0.0f);

    glm::mat4 model(1.0);

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
            ImGui::SliderFloat("Angle", &phi, -180.0f, 180.0f);

            ImGui::End();
        });

        {
            const auto phi_rad = glm::radians(phi);
            glm::quat q_x(glm::cos(phi_rad), glm::sin(phi_rad * u_q)); // Rotates phi radians around u_q axis
            model = glm::mat4_cast(q_x) * model;

            shader.Use();
            shader.SetMat4("projection", cam.GetPerspectiveMatrix());
            shader.SetMat4("view", cam.GetViewMatrix());
            shader.SetMat4("model", model);

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

