#ifndef MOUSE_H
#define MOUSE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

/*
    mouse class to handle mouse callbacks
*/

class Mouse {
public:
    /*
        callbacks
    */

    // cursor position changed
    static void cursorPosCallback(GLFWwindow* window, double _x, double _y);
    // mouse button state changed
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    // scroll wheel moved
    static void mouseWheelCallback(GLFWwindow* window, double dx, double dy);

    /*
        accessors
    */

    static double getMouseX();
    static double getMouseY();

    static double getDX();
    static double getDY();

    static double getScrollDX();
    static double getScrollDY();

    static bool button(int button);
    static bool buttonChanged(int button);
    static bool buttonWentUp(int button);
    static bool buttonWentDown(int button);

private:
    /*
        static mouse values
    */

    static double x;
    static double y;

    static double lastX;
    static double lastY;
    
    static double dx;
    static double dy;

    static double scrollDx;
    static double scrollDy;

    static bool firstMouse;

    static bool buttons[];
    static bool buttonsChanged[];
};

#endif