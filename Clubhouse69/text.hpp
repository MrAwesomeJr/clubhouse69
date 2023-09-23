#ifndef TEXT_HPP_INCLUDED
#define TEXT_HPP_INCLUDED

#include "opengltextures.hpp"
#include "resources/font/font.hpp"

namespace ch69 {
    class Text{
        public:
            Text(const Font& font, const std::string& font_image_path)
            :
            font_(font),
            texture_(ImageTexture(font_image_path)) {}

            int get_text_length(const std::string& text, int font_size) {
                int length = 0;
                for(int index = 0; index < text.length(); index++) {
                    for (int i = 0; i < font_.character_count; i++){
                        if (font_.characters[i].code_point == text[index]){
                            length += font_.characters[i].width;
                        }
                    }
                }
                // scale to font size;
                return (length * font_size / font_.size);
            }

            void render_text(const GLFWwindow& window, double x, double y, int font_size, const std::string& text) {
                /* origin_x and origin_y define the bottom left corner relative to x and y in the font data.
                the point referenced by origin_x and origin_y should be fixed, which means the texture is moved based on this point.
                the top left corner is (x, y - height)
                the bottom right corner is (x + width, y)*/
                int x_offset = 0;
                ImageParams params;
                Character current_character;
                std::vector<GLfloat> tex_coord;
                bool character_found;

                for(int index = 0; index < text.length(); index++) {
                    character_found = false;
                    for (int i = 0; i < font_.character_count; i++){
                        if (font_.characters[i].code_point == text[index]) {
                            current_character = font_.characters[i];
                            character_found = true;
                            break;
                        } else if ((font_.characters[i].code_point == 65533 ) && (!character_found)) {
                            current_character = font_.characters[i];
                        }
                    }

                    if (character_found) {
                        params.set_coords(x - current_character.origin_x + x_offset,
                                          y - current_character.origin_y + (font_.size - current_character.height) * font_size / font_.size);
                        x_offset += current_character.width * font_size / font_.size;
                        params.set_size(current_character.width * font_size / font_.size,
                                        current_character.height * font_size / font_.size);
                        tex_coord = {float(current_character.x) / float(font_.width), float(current_character.y) / float(font_.height),
                                     float(current_character.x + current_character.width) / float(font_.width), float(current_character.y) / float(font_.height),
                                     float(current_character.x + current_character.width) / float(font_.width), float(current_character.y + current_character.height) / float(font_.height),
                                     float(current_character.x) / float(font_.width), float(current_character.y + current_character.height) / float(font_.height)};
                        params.set_tex_coord(tex_coord);
                        params.set_vertices_as_rectangle();

                        texture_.set_params(params);
                        texture_.draw(window);
                    }
                }
            }

            void render_centered_text(const GLFWwindow& window, double x, double y, int font_size, const std::string& text) {
                int new_x = x - (get_text_length(text, font_size) / 2);
                int new_y = y - (font_size / 2);
                render_text(window, new_x, new_y, font_size, text);
            }

        private:
            Font font_;
            ImageTexture texture_;
    };
}

#endif // OPENGLTEXT_HPP_INCLUDED