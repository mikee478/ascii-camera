#pragma once

#include <GLFW/glfw3.h>
#include <array>
#include "imgui/imgui.h"

namespace Input
{
    std::array<int, GLFW_KEY_LAST+1> key_pressed;
    std::array<int, GLFW_MOUSE_BUTTON_LAST+1> mouse_button_pressed;

    bool UpPressed() {return key_pressed[GLFW_KEY_UP];}
    bool DownPressed() {return key_pressed[GLFW_KEY_DOWN];}
    bool LeftPressed() {return key_pressed[GLFW_KEY_LEFT];}
    bool RightPressed() {return key_pressed[GLFW_KEY_RIGHT];}
    bool WPressed() {return key_pressed[GLFW_KEY_W];}
    bool SPressed() {return key_pressed[GLFW_KEY_S];}
    bool APressed() {return key_pressed[GLFW_KEY_A];}
    bool DPressed() {return key_pressed[GLFW_KEY_D];}
    bool SpacePressed() {return key_pressed[GLFW_KEY_SPACE];}
    bool RightShiftPressed() {return key_pressed[GLFW_KEY_RIGHT_SHIFT];}
    bool EscapePressed() {return key_pressed[GLFW_KEY_ESCAPE];}

    bool MouseButtonLeftPressed() {return mouse_button_pressed[GLFW_MOUSE_BUTTON_LEFT];}
    bool MouseButtonRightPressed() {return mouse_button_pressed[GLFW_MOUSE_BUTTON_RIGHT];}
    bool MouseButtonMiddlePressed() {return mouse_button_pressed[GLFW_MOUSE_BUTTON_MIDDLE];}

    void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if(action == GLFW_PRESS) key_pressed[key] = true;
        else if(action == GLFW_RELEASE) key_pressed[key] = false;
    }

    void MousePosCallback(GLFWwindow* window, double x_pos, double y_pos)
    {
    }

    void GetMouseDelta(GLFWwindow* window, int &delta_x, int &delta_y)
    {
        static bool init = false;
        static double prev_mouse_x = 0;
        static double prev_mouse_y = 0;

        double mouse_x, mouse_y;
        glfwGetCursorPos(window, &mouse_x, &mouse_y);

        if(init)
        {
            delta_x = mouse_x - prev_mouse_x;
            delta_y = mouse_y - prev_mouse_y;
        }
        else
        {
            init = true;
            delta_x = 0;
            delta_y = 0;
        }

        prev_mouse_x = mouse_x;
        prev_mouse_y = mouse_y;
    }

    void GetMouseDragDelta(GLFWwindow* window, int &delta_x, int &delta_y)
    {
        static int prev_mouse_x = 0;
        static int prev_mouse_y = 0;
        static bool was_drag = false;

        double mouse_x, mouse_y;
        glfwGetCursorPos(window, &mouse_x, &mouse_y);

        bool drag = MouseButtonLeftPressed();
        
        if(was_drag && drag)
        {
            delta_x = mouse_x - prev_mouse_x;
            delta_y = mouse_y - prev_mouse_y;
        }
        else 
        {
            delta_x = 0;
            delta_y = 0;
        }

        prev_mouse_x = mouse_x;
        prev_mouse_y = mouse_y;
        was_drag = drag;
    }

    void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
    {
        // ignore mouse button over imgui window 
        if(action == GLFW_PRESS) mouse_button_pressed[button] = true;
        else if(action == GLFW_RELEASE) mouse_button_pressed[button] = false;
    }
};