#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>

#include <filesystem>

#include "shader.h"

#include "imgui.h"
#include "imgui_internal.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#define S_WIDTH  640
#define S_HEIGHT 480

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

constexpr std::string_view res_dir{TOSTRING(RES_DIR)};

std::string operator+(std::string_view sv, const char* cstr) {
    return std::string(sv) + cstr;
}

// Identity matrix
static constexpr glm::mat4 I(1.0);

static constexpr glm::mat4 GetTranslationMatrix(float t_x, float t_y, float t_z) {
    return glm::translate(I, glm::vec3{t_x, t_y, t_z});
}

static glm::mat4 GetRotationMatrix(float angle, glm::vec3& axis) {
    return glm::rotate(I, angle, axis);
}

static glm::mat4 GetScaleMatrix(float s_x, float s_y, float s_z) {
    return glm::scale(I, glm::vec3{s_x, s_y, s_z});
}

static glm::mat4 GetUniformScaleMatrix(float s) {
    return glm::scale(I, glm::vec3{s, s, s});
}

static glm::mat4 GetShearMatrix(glm::vec2& l_x, glm::vec2& l_y, glm::vec2& l_z) {
    return glm::shear(I, glm::vec3{}, l_x, l_y, l_z);
}

int main() {
    GLFWwindow *window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(S_WIDTH, S_HEIGHT, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    /* Initialize GLAD */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        return -1;
    
    // Setup ImGui binding
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 460 core");
    ImGui::StyleColorsDark();

    Shader shader(
        res_dir + "/shaders/vertex.glsl", 
        res_dir + "/shaders/fragment.glsl"
    );

    const float vertices[] = {
         0.5f,  0.5f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left 
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    glBindVertexArray(0); 

    float t_x = 0.0;
    float t_y = 0.0;
    float t_z = 0.0;
    float angle = 0.0;
    float scale = 1.0;
    glm::vec2 shear_x{};
    glm::vec2 shear_y{};

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        // Imgui
        {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            ImGui::Begin("Settings");  
            ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            
            ImGui::Text("Translation");
            ImGui::SliderFloat("Tx", &t_x, -1.0f, 1.0f);
            ImGui::SliderFloat("Ty", &t_y, -1.0f, 1.0f);
            //ImGui::SliderFloat("Tz", &t_z, -1.0f, 1.0f);
            
            ImGui::Text("Rotation");
            ImGui::SliderFloat("Angle", &angle, -IM_PI, IM_PI);
            
            ImGui::Text("Uniform scaling");
            ImGui::SliderFloat("scale", &scale, -2.0f, 2.0f);
            
            ImGui::Text("Shearing");
            ImGui::SliderFloat("x", &shear_x.x, -1.0f, 1.0f);
            ImGui::SliderFloat("y", &shear_y.x, -1.0f, 1.0f);

            ImGui::End();
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }
        // Object
        {
            shader.Use();
            
            const auto T = GetTranslationMatrix(t_x, t_y, t_z);
            const auto R = GetRotationMatrix(angle, glm::vec3{0.0, 0.0, 1.0});
            const auto S = GetUniformScaleMatrix(scale);
            const auto H = GetShearMatrix(shear_x, shear_y, glm::vec2{});

            // TRS order
            const auto model = T * R * S * H;

            shader.SetMat4("model", model);
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();    
    return 0;
}
