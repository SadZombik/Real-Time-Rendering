#ifndef FRAMEWORK_H
#define FRAMEWORK_H

#include <string_view>
#include <glm/glm.hpp>
#include <functional>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

constexpr std::string_view res_dir{TOSTRING(RES_DIR)};

// For specifying paths
inline std::string operator+(std::string_view sv, const char* cstr) {
    return std::string(sv) + cstr;
}

namespace Framework {
    GLFWwindow* CreateWindow(int width, int height, const char* title);

    void EnableDepthTest();
    void ImGuiCallback(const std::function<void()>& ImGuiCode);

    void Shutdown();
}; // namespace Framework

namespace Transforms {
    struct TransformParameters;
    glm::mat4 GetTranslationMatrix(float t_x, float t_y, float t_z);
    glm::mat4 GetRotationMatrix(float angle, glm::vec3& axis);
    glm::mat4 GetScaleMatrix(float s_x, float s_y, float s_z);
    glm::mat4 GetUniformScaleMatrix(float s);
    glm::mat4 GetShearMatrix(glm::vec2& l_x, glm::vec2& l_y, glm::vec2& l_z);
}; // namespace Transforms

#endif // !FRAMEWORK_H
