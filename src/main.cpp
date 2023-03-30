#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <iostream>
#include <vector>
#include <algorithm>

#include "renderer.h"
#include "vertex_buffer.h"
#include "shader.h"
#include "index_buffer.h"
#include "vertex_buffer_layout.h"
#include "vertex_array.h"
#include "renderer.h"
#include "input.h"
#include "camera.h"
#include "font_renderer.h"

#include <opencv2/imgproc/imgproc.hpp>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

int main(void)
{
    // Initialize the GLFW library
    if (!glfwInit())
        return -1;

    // specify the client API version that the created context must be compatible with
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    /* 
    specifies whether the OpenGL context should be forward-compatible, i.e. one
    where all functionality deprecated in the requested version  of OpenGL is 
    removed. This must only be used if the requested OpenGL version is 3.0 or above.
    */
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a windowed mode window and its OpenGL context
    const int WINDOW_WIDTH = 700;
    const int WINDOW_HEIGHT = 700;
    const char* WINDOW_TITLE = "ASCII Camera";
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
    if (!window) // Error
    {
        std::cout << "Window create error" << std::endl;
        glfwTerminate();
        return -1;
    }

    /* Make the OpenGL context of the specified window current
        An OpenGL context represents many things. A context stores all of the state 
        associated with this instance of OpenGL. It represents the (potentially visible) 
        default framebuffer that rendering commands will draw to when not drawing to a 
        framebuffer object. Think of a context as an object that holds all of OpenGL; 
        when a context is destroyed, OpenGL is destroyed.
     */
    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, Input::KeyCallback);
    glfwSetCursorPosCallback(window, Input::MousePosCallback);
    glfwSetMouseButtonCallback(window, Input::MouseButtonCallback);

	// Min number of screen updates to wait for until the buffers are swapped by glfwSwapBuffers
    glfwSwapInterval(1);

    // Create OpenGL rendering context before calling glewInit
    // Initialize the extension entry points
    if(glewInit() != GLEW_OK)
        std::cout << "GLEW ERROR!" << std::endl;

    const glm::vec2 MIN_BOUND(0.0f, 0.0f);
    const glm::vec2 MAX_BOUND(static_cast<float>(WINDOW_WIDTH),static_cast<float>(WINDOW_HEIGHT));
    Shader::projection_mat = glm::ortho(MIN_BOUND.x, MAX_BOUND.x, MIN_BOUND.y, MAX_BOUND.y);

    FontRenderer font_renderer("/Users/michael/Documents/projects/ascii-camera/res/fonts/Menlo.ttc", 8);

    const int ROWS = 77;
    const int COLS = 139;
    std::vector<char> text(ROWS*(COLS+1), '\n');

    Camera camera;
    cv::Mat frame, flip, gray, crop;
    cv::Mat resize_frame(ROWS, COLS, CV_8UC1);

    std::string ascii_map = " .,:;i1tfLCG08@";
    std::reverse(ascii_map.begin(), ascii_map.end());

    glClearColor(1.0, 1.0, 1.0, 1.0);

    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 150");

    float contrast = 2.5;

    while (!glfwWindowShouldClose(window) && !Input::EscapePressed())
    {
        glfwPollEvents();

        // ImGui_ImplOpenGL3_NewFrame();
        // ImGui_ImplGlfw_NewFrame();
        // ImGui::NewFrame();
        // ImGui::Begin("Configuration");
        // ImGui::SliderFloat("Contrast", &contrast, 0.0f, 10.0f);
        // ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
        // ImGui::End();

        Clear();

        if(camera.GetFrame(frame))
        {
            crop = frame(cv::Rect((frame.cols - frame.rows) / 2, 0, frame.rows, frame.rows));
            cv::cvtColor(crop, gray, cv::COLOR_BGR2GRAY);
            cv::flip(gray, flip, 1);
            cv::resize(flip, resize_frame, resize_frame.size(), 0, 0, cv::INTER_AREA);

            for(int r = 0;r<ROWS;r++)
            {
                for(int c = 0; c<COLS+1; c++)
                {
                    if(c == COLS) continue;
                    double val = resize_frame.at<uchar>(r,c);
                    val = std::clamp((val - 128) * contrast + 128, 0.0, 255.0);
                    int index = floor(val / 256.0 * ascii_map.length());
                    text[c + r * (COLS+1)] = ascii_map[index];
                }
            }

            font_renderer.RenderText(text, 2, 690);
        }

        // ImGui::Render();
        // ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Swap front and back buffers
        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // Destroys all remaining windows and cursors and frees allocated resources
    glfwTerminate();
    return 0;
}