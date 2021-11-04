#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#include "opengltextures.hpp"

int main(){
    if (glfwInit())
        {
            // default screen settings
            GLint windowWidth = 500;
            GLint windowHeight = 500;
            char title[] = "Project Clubhouse";
            GLFWwindow * window;
            window = glfwCreateWindow(windowWidth, windowHeight, title, NULL, NULL);

            glfwMakeContextCurrent(window);
            glewInit();

            glfwGetWindowSize(window, &windowWidth, &windowHeight);
            glOrtho(0, windowWidth, windowHeight, 0, -1, 1);

            unsigned int imageTex;
            tex::imageParams imageParams;
            unsigned char * imageData = stbi_load("./Resources/Images/tps.png", &imageParams.resolutionX, &imageParams.resolutionY, NULL, STBI_rgb_alpha);
            imageParams.setParams(0,0,500,500);
            tex::loadTexture(imageData,imageTex,imageParams);

            while (!glfwWindowShouldClose(window)) {
                glClear(GL_COLOR_BUFFER_BIT);
	            glClearColor(1, 1, 1, 1.0);
                glColor3f(1,1,1);

                glfwGetCursorPos(window, &imageParams.x, &imageParams.y);
                tex::drawTexture(imageTex,imageParams);

                glFlush();
                glfwSwapBuffers(window);
                glfwPollEvents();
            }

            glfwTerminate();
        } else {
            std::cout << "glfw/glew didn't initialise correctly";
        }

    return 0;
}