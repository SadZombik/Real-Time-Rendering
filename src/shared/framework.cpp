#include "Framework.h"

#include <iostream>

#include <glm/gtc/matrix_transform.hpp>

#include "imgui.h"
#include "imgui_internal.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

static GLFWwindow *window;

static void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

namespace Framework {
    GLFWwindow* CreateWindow(int width, int height, const char* title) {
        if (!glfwInit()) {
            std::cerr << "GLFW init failed" << std::endl;
            exit(-1);
        }

        window = glfwCreateWindow(width, height, title, NULL, NULL);
        if (!window) {
            std::cerr << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            exit(-1);
        }

        glfwMakeContextCurrent(window);
        glfwSwapInterval(1);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cerr << "GLAD init failed" << std::endl;
            exit(-1);
        }
#ifndef NDEBUG
        std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
#endif
        glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

        // Setup ImGui binding
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 460 core");
        ImGui::StyleColorsDark();

        return window;
    }

    void EnableDepthTest() {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
    }

    void ImGuiCallback(const std::function<void()>& ImGuiCode) {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGuiCode();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void Shutdown() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        glfwDestroyWindow(window);
        glfwTerminate();
    }
};

namespace Transforms {
    // Identity matrix
    static constexpr glm::mat4 I(1.0);

    glm::mat4 GetTranslationMatrix(const float t_x, const float t_y, const float t_z) {
        return glm::translate(I, glm::vec3{t_x, t_y, t_z});
    }

    glm::mat4 GetRotationMatrix(const float angle, glm::vec3& axis) {
        const auto angle_rad = glm::radians(angle);
        return glm::rotate(I, angle_rad, axis);
    }

    glm::mat4 GetScaleMatrix(const float s_x, const float s_y, const float s_z) {
        return glm::scale(I, glm::vec3{s_x, s_y, s_z});
    }

    glm::mat4 GetUniformScaleMatrix(const float s) {
        return glm::scale(I, glm::vec3{s, s, s});
    }

    glm::mat4 GetShearMatrix(const glm::vec2& l_x, const glm::vec2& l_y, const glm::vec2& l_z) {
        return glm::shear(I, glm::vec3{}, l_x, l_y, l_z);
    }
};
