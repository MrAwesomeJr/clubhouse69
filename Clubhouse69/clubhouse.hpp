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
            Clubhouse()
            :
            glfw_init_(glfwInit()),
            window_manager_("Clubhouse69", 1280, 720),
            menlo_(Text(font_Menlo, "./resources/font/menlo/menlo.png"))
            {
                directory_manager_.get_games(128, games_);
                glOrtho(0, 1280, 720, 0, -1, 1);
            }

            ~Clubhouse() {
                glfwTerminate();
            }

            int run() {
                while (state_ == Menu && !glfwWindowShouldClose(&window_manager_.get_window())){
                    render_menu_();
                }

                return 0;
            }

        private:
            bool glfw_init_, glew_init_;
            double menu_scroll_distance_ = 0;
            std::vector<Game> games_;
            WindowManager window_manager_;
            DirectoryManager directory_manager_;
            Text menlo_;

            enum State_ {Menu};
            State_ state_ = Menu;

            void update_screen_() {
                glFlush();
                glfwSwapBuffers(&window_manager_.get_window());
                window_manager_.get_input().reset_inputs();
                glfwPollEvents();
            }

            void render_menu_() {
                glClear(GL_COLOR_BUFFER_BIT);
                glClearColor(0, 0, 0, 1.0);
                glColor3f(1,1,1);

                // display menu buttons
                menu_scroll_distance_ += window_manager_.get_input().get_scroll_distance()[1];
                menlo_.render_centered_text(window_manager_.get_window(), 640, 120+menu_scroll_distance_, 64, "Clubhouse69");

                std::vector<Button> local_game_buttons;

                for (int i = 0; i < directory_manager_.games_count; i++) {
                    local_game_buttons.push_back(Button(games_[i].get_button_128(), window_manager_.get_input(), games_[i].get_local_execute()));
                }

                for(int i = 0; i < directory_manager_.games_count; i++) {
                    // TODO: fix
                    local_game_buttons[i].set_coords(240+(i%3)*320, menu_scroll_distance_+240+((int)i/3)*320);
                    local_game_buttons[i].set_vertices_as_rectangle();
                    local_game_buttons[i].draw(window_manager_.get_window());
//                    local_game_buttons[i].get_params().set_coords(240+(i%3)*320, menu_scroll_distance_+240+((int)i/3)*320);
//                    local_game_buttons[i].get_params().set_vertices_as_rectangle();
//                    local_game_buttons[i].draw(window_manager_.get_window());
                }

                update_screen_();
            }
    };
}