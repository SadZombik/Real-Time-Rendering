#include "framework.h"

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include "imgui.h"
#include "imgui_internal.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

static GLFWwindow *window;

namespace Framework {
    GLFWwindow* CreateWindow(int width, int height) {
        if (!glfwInit()) {
            std::cerr << "GLFW init failed" << std::endl;
            exit(-1);
        }

        window = glfwCreateWindow(width, height, "2D Transforms", NULL, NULL);
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
        // Setup ImGui binding
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 460 core");
        ImGui::StyleColorsDark();

        return window;
    }

    void ProcessImGui(const std::function<void()>& ImGuiCode) {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();      

        ImGuiCode();

        ImGui::End();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
};

namespace Transforms {
    // Identity matrix
    static constexpr glm::mat4 I(1.0);

    glm::mat4 GetTranslationMatrix(float t_x, float t_y, float t_z) {
        return glm::translate(I, glm::vec3{t_x, t_y, t_z});
    }

    glm::mat4 GetRotationMatrix(float angle, glm::vec3& axis) {
        return glm::rotate(I, angle, axis);
    }

    glm::mat4 GetScaleMatrix(float s_x, float s_y, float s_z) {
        return glm::scale(I, glm::vec3{s_x, s_y, s_z});
    }

    glm::mat4 GetUniformScaleMatrix(float s) {
        return glm::scale(I, glm::vec3{s, s, s});
    }

    glm::mat4 GetShearMatrix(glm::vec2& l_x, glm::vec2& l_y, glm::vec2& l_z) {
        return glm::shear(I, glm::vec3{}, l_x, l_y, l_z);
    }
};
