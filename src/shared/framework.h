#ifndef FRAMEWORK_H
#define FRAMEWORK_H

#include <string_view>
#include <glm/glm.hpp>
#include <functional>

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

constexpr std::string_view res_dir{TOSTRING(RES_DIR)};

// For specifying paths
inline std::string operator+(std::string_view sv, const char* cstr) {
    return std::string(sv) + cstr;
}

struct GLFWwindow;

namespace Colors {
    // Converts RGBA color into 32 bit integer
    constexpr uint32_t rgba_to_hex(int r, int g, int b, int a = 255) {
        return (a << 24) | (r << 16) | (g << 8) | b;
    }

    constexpr glm::vec3 hex_to_rgba(uint32_t color) {
        return glm::vec3 {
            (color >> 24) & 0xFF,
            (color >> 16) & 0xFF,
            (color >> 8) & 0xFF,
        };
    }

    constexpr auto RED      = rgba_to_hex(230, 41, 55);
    constexpr auto ORANGE   = rgba_to_hex(255, 161, 0);
    constexpr auto YELLOW   = rgba_to_hex(253, 249, 0);
    constexpr auto GREEN    = rgba_to_hex(0, 228, 48);
    constexpr auto SKYBLUE  = rgba_to_hex(102, 191, 255);
    constexpr auto BLUE     = rgba_to_hex(0, 121, 241);
    constexpr auto PURPLE   = rgba_to_hex(200, 122, 255);
    constexpr auto GRAY     = rgba_to_hex(80, 80, 80);
    constexpr auto BLACK    = rgba_to_hex(0, 0, 0);
    constexpr auto WHITE    = rgba_to_hex(255, 255, 255);
};

namespace Framework {
    GLFWwindow* CreateWindow(int width, int height);
    
    void ProcessImGui(const std::function<void()>& ImGuiCode);
}; // namespace Framework

namespace Transforms {
    glm::mat4 GetTranslationMatrix(float t_x, float t_y, float t_z);
    glm::mat4 GetRotationMatrix(float angle, glm::vec3& axis);
    glm::mat4 GetScaleMatrix(float s_x, float s_y, float s_z);
    glm::mat4 GetUniformScaleMatrix(float s);
    glm::mat4 GetShearMatrix(glm::vec2& l_x, glm::vec2& l_y, glm::vec2& l_z);
}; // namespace Transforms

#endif // !FRAMEWORK_H