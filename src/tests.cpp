#include "tests.h"
#include "utils.h"
#include "shader.h"

#include "imgui.h"
#include "imgui_internal.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>

#define S_WIDTH  640
#define S_HEIGHT 480

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

static GLFWwindow *window;

struct Camera
{
	glm::vec3 Position = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 FrontVector = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 UpVector = glm::vec3(0.0f, 1.0f, 0.0f);

	float yaw = -90.0f;
	float pitch = 0.0f;
	float lastX = (float)S_WIDTH  / 2.0;
	float lastY = (float)S_HEIGHT / 2.0;
	float fov = 45.0f;
	bool  firstMouse = true;

	glm::mat4 GetPerspectiveMatrix() const
	{
		return glm::perspective(
			glm::radians(fov),
			(float)S_WIDTH / (float)S_HEIGHT, 
			0.1f, 
			100.0f
		);
	}

	glm::mat4 GetViewMatrix() const
	{
		return glm::lookAt(
			Position,
			Position + FrontVector,
			UpVector
		);
	}
};

static glm::mat4 GetTranslationMatrix(float t_x, float t_y, float t_z) {
    return glm::translate(glm::mat4(1.0f), glm::vec3{t_x, t_y, t_z});
}

namespace BasicTransforms {
    int Init() {
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

        // Setup style
        ImGui::StyleColorsDark();

        return 0;
    }

    void TestTranslation() {
        const std::string res_dir{TOSTRING(RES_DIR)};
        Shader shader(
            res_dir + "/shaders/vertex.glsl", 
            res_dir + "/shaders/fragment.glsl"
        );

        auto T = GetTranslationMatrix(0.5, 0.4, 0.3);
        
        float cube_size = 5;
        float surf_size = 5;

        while (!glfwWindowShouldClose(window))
        {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            
            glClear(GL_COLOR_BUFFER_BIT);
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

            ImGui::Begin("Settings");  
            ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            // ImGui::SliderFloat("fov", &camera.fov, 45.0f, 90.0f);
            ImGui::SliderFloat("cube size", &cube_size, 0.1f, 10.0f);
            ImGui::SliderFloat("plane size", &surf_size, 0.1f, 10.0f);
            ImGui::End();
            ImGui::Render();
            
            /* ImGui Render */
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            


            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }
    
    void Clear() {
        glfwTerminate();
    }
}; // namespace BasicTransforms