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

#include <array>

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

class Rotation {
public:
    Rotation() {
        shader = Shader(
            res_dir + "/shaders/quat/vertex.glsl",
            res_dir + "/shaders/quat/fragment.glsl"
        );
        angles[0] = 0.0f;
        angles[1] = 0.0f;
        angles[2] = 0.0f;
    }

    void Update(
        const VertexArrayObject& vao,
        const std::function<glm::mat4(const glm::vec3&)>& modelMatrixCallback,
        const glm::mat4& viewMatrix,
        const glm::mat4& projMatrix
    ) {
        const auto model = modelMatrixCallback(glm::vec3(angles[0], angles[1], angles[2]));

        shader.Use();
        shader.SetMat4("projection", projMatrix);
        shader.SetMat4("view", viewMatrix);
        shader.SetMat4("model", model);

        vao.Bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        vao.Unbind();
    }

    inline float* GetAngles() { return angles.data(); }

private:
    std::array<float, 3> angles;
    Shader shader;
};

int main() {
    auto* window = Framework::CreateWindow("Quaternoins Basics");
    Framework::EnableDepthTest();

    CameraController cam(DEFAULT_WIDTH, DEFAULT_HEIGHT);
    glfwSetWindowUserPointer(window, &cam);
    glfwSetWindowSizeCallback(window, WindowSizeCallback);

    Rotation quatRotation;
    Rotation matRotation;
    Rotation testRotation;

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
            ImGui::SliderFloat3("Q Angles", quatRotation.GetAngles(), -180.0f, 180.0f);
            
            ImGui::Text("Matrix Rotation");
            ImGui::SliderFloat3("M Angles", matRotation.GetAngles(), -180.0f, 180.0f);
            
            ImGui::Text("Conjugate Rotation");
            ImGui::SliderFloat3("C Angles", testRotation.GetAngles(), -180.0f, 180.0f);

            ImGui::End();
        });

        quatRotation.Update(vao, [&](const glm::vec3& angles) -> glm::mat4 {
            return glm::mat4_cast(
                glm::angleAxis(glm::radians(angles.z), glm::vec3(0.0f, 0.0f, 1.0f)) *
                glm::angleAxis(glm::radians(angles.y), glm::vec3(0.0f, 1.0f, 0.0f)) *
                glm::angleAxis(glm::radians(angles.x), glm::vec3(1.0f, 0.0f, 0.0f))
            );
        }, cam.GetViewMatrix(), cam.GetPerspectiveMatrix());
    
        matRotation.Update(vao, [&](const glm::vec3& angles) -> glm::mat4 {
            return Transforms::GetTranslationMatrix(1.5f, 0.0f, 0.0f) * (
                Transforms::GetRotationMatrix(angles.z, glm::vec3(0.0f, 0.0f, 1.0f)) *
                Transforms::GetRotationMatrix(angles.y, glm::vec3(0.0f, 1.0f, 0.0f)) *
                Transforms::GetRotationMatrix(angles.x, glm::vec3(1.0f, 0.0f, 0.0f))
            );
        }, cam.GetViewMatrix(), cam.GetPerspectiveMatrix());
        
        testRotation.Update(vao, [&](const glm::vec3& angles) -> glm::mat4 {
            const auto q = 
                glm::angleAxis(glm::radians(angles.z), glm::vec3(0.0f, 0.0f, 1.0f)) *
                glm::angleAxis(glm::radians(angles.y), glm::vec3(0.0f, 1.0f, 0.0f)) *
                glm::angleAxis(glm::radians(angles.x), glm::vec3(1.0f, 0.0f, 0.0f));
            
            const auto q_c = glm::conjugate(q);
            return Transforms::GetTranslationMatrix(-1.5f, 0.0f, 0.0f) * glm::mat4_cast(q_c);
        }, cam.GetViewMatrix(), cam.GetPerspectiveMatrix());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    Framework::Shutdown();

    return 0;
}
