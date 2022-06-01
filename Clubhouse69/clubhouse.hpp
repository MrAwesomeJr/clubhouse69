#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <dirent.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include "opengltextures.hpp"
#include "windowmanager.hpp"
#include "directorymanager.hpp"
#include "text.hpp"
#include "resources/font/menlo/menlo.hpp"

namespace ch69{

    class Clubhouse{
        public:
            Clubhouse(const std::string& window_name = "Clubhouse69")
            :
            glfw_init_(glfwInit()),
            window_name_(window_name),
            window_manager_(window_name, 1280, 720),
            menlo_(Text(font_Menlo, "./resources/font/menlo/menlo.png"))
            {
                directory_manager_.count_games();
                directory_manager_.get_icon_textures(128, icon_textures_);
                glOrtho(0, 1280, 720, 0, -1, 1);
            }

            void run(){
                std::string state = "menu";
                while (state == "menu" && !glfwWindowShouldClose(&window_manager_.get_window())){
                    render_menu_();
                }

                glfwTerminate();
            }

        private:
            bool glfw_init_, glew_init_;
            double menu_scroll_distance_ = 0;
            std::vector<ImageTexture> icon_textures_;
            WindowManager window_manager_;
            const std::string window_name_;
            DirectoryManager directory_manager_;
            Text menlo_;

            void update_screen_(){
                glFlush();
                glfwSwapBuffers(&window_manager_.get_window());
                window_manager_.get_input().reset_inputs();
                glfwPollEvents();
            }

            void render_menu_(){
                glClear(GL_COLOR_BUFFER_BIT);
                glClearColor(0, 0, 0, 1.0);
                glColor3f(1,1,1);

                // display menu buttons
                menu_scroll_distance_ += window_manager_.get_input().get_scroll_distance()[1];
                menlo_.render_centered_text(window_manager_.get_window(), 640, 120+menu_scroll_distance_, 64, "Clubhouse69");

                for(int i = 0; i < icon_textures_.size(); i++) {
                    icon_textures_[i].get_params().set_coords(240+(i%3)*320, menu_scroll_distance_+240+((int)i/3)*320);
                    icon_textures_[i].get_params().set_vertices_as_rectangle();
                    icon_textures_[i].draw(window_manager_.get_window());
                }

                update_screen_();
            }
    };
}