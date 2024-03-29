#ifndef FRAMEWORK_H
#define FRAMEWORK_H

#include <string_view>
#include <glm/glm.hpp>
#include <functional>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_internal.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#define DEFAULT_WIDTH 1280
#define DEFAULT_HEIGHT 720

constexpr std::string_view res_dir{TOSTRING(RES_DIR)};

// For specifying paths
inline std::string operator+(std::string_view sv, const char* cstr) {
    return std::string(sv) + cstr;
}

namespace Framework {
    GLFWwindow* CreateWindow(const char* title, int width = DEFAULT_WIDTH, int height = DEFAULT_HEIGHT);

    void EnableDepthTest();
    void DisableDepthTest();
    void ClearBuffer();
    void ImGuiCallback(const std::function<void()>& ImGuiCode);

    void Shutdown();
}; // namespace Framework

namespace Transforms {
    struct TransformParameters;
    glm::mat4 GetTranslationMatrix(const float t_x, const float t_y, const float t_z);
    glm::mat4 GetRotationMatrix(const float angle, const glm::vec3& axis);
    glm::mat4 GetScaleMatrix(const float s_x, const float s_y, const float s_z);
    glm::mat4 GetUniformScaleMatrix(const float s);
    glm::mat4 GetShearMatrix(const glm::vec2& l_x, const glm::vec2& l_y, const glm::vec2& l_z);
}; // namespace Transforms

#endif // !FRAMEWORK_H
