#ifndef OPENGLTEXTURES_HPP_INCLUDED
#define OPENGLTEXTURES_HPP_INCLUDED

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "input.hpp"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#endif // STB_IMAGE_IMPLEMENTATION

namespace ch69{
    class ImageParams{
        public:
            void set_vertices_as_rectangle() {
                vertices_ = rectangular_vertices_(coords_, size_);
            }

            void move(double d_x, double d_y) {
                coords_[0] += d_x;
                coords_[1] += d_y;
            }

            const double* get_coords() const {
                return coords_;
            }

            void set_coords(double coords_x, double coords_y) {
                coords_[0] = coords_x;
                coords_[1] = coords_y;
            }

            const int* get_size() const {
                return size_;
            }

            void set_size(int size_x, int size_y) {
                size_[0] = size_x;
                size_[1] = size_y;
            }

            const int* get_resolution() const {
                return resolution_;
            }

            void set_resolution(int resolution_x, int resolution_y) {
                resolution_[0] = resolution_x;
                resolution_[1] = resolution_y;
            }

            const std::vector<GLfloat>& get_vertices() const {
                return vertices_;
            }

            void set_vertices(const std::vector<GLfloat>& vertices) {
                vertices_ = vertices;
            }

            const std::vector<GLubyte>& get_indices() const {
                return indices_;
            }

            void set_indices(const std::vector<GLubyte>& indices) {
                indices_ = indices;
            }

            const std::vector<GLfloat>& get_tex_coord() const {
                return tex_coord_;
            }

            void set_tex_coord(const std::vector<GLfloat>& tex_coord) {
                tex_coord_ = tex_coord;
            }

        private:
            // coordinates of displayed image are calculated based off of this value
            double coords_[2];

            // width and height of the displayed image (affected by mipmap)
            int size_[2];

            // resolution of original image. Usually obtained dynamically through stbi_load
            int resolution_[2];


            // Vertices, Indices and TexCoord by default set up a rectangle (displays the entire image)
            // potential points that may be used to display the image
            std::vector<GLfloat> vertices_;

            // actual points that are used. organised into groups of 3.
            std::vector<GLubyte> indices_ = {0,1,2,
                                             0,2,3};

            // where to display the texture based on Vertices and Indices
            std::vector<GLfloat> tex_coord_ = {0, 0,
                                              1, 0,
                                              1, 1,
                                              0, 1};

            std::vector<GLfloat> rectangular_vertices_(double coords[2], int size[2]) {
                return {(float)coords[0], (float)coords[1], 0,
                (float)coords[0] + size[0], (float)coords[1], 0,
                (float)coords[0] + (float)size[0], (float)coords[1] + (float)size[1], 0,
                (float)coords[0], (float)coords[1] + (float)size[1], 0};
            }
    };

    class ImageTexture{
        public:
            ImageTexture(const std::string& path, bool alpha_flag = true, const GLenum& min_filter = GL_LINEAR, const GLenum& mag_filter = GL_NEAREST) {
                unsigned char* image_data;
                int resolution[2];
                if(alpha_flag) {
                    image_data = stbi_load(path.c_str(), &resolution[0], &resolution[1], NULL, STBI_rgb_alpha);
                } else {
                    image_data = stbi_load(path.c_str(), &resolution[0], &resolution[1], NULL, STBI_rgb);
                }

                params_.set_resolution(resolution[0], resolution[1]);
                params_.set_size(resolution[0], resolution[1]);

                glGenTextures(1, &texture_);
                glActiveTexture(texture_);
                glBindTexture(GL_TEXTURE_2D, texture_);
                glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);


                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, resolution[0], resolution[1], 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);

                stbi_image_free(image_data);
            }

            ImageParams& get_params() {
                return params_; 
            }

            void set_params(const ImageParams& params) {
                params_ = params;
            }

            void set_texture(int input) {
                texture_ = input;
            }
            

            void draw(const GLFWwindow& window, bool blend_flag = true) {
                glEnable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, texture_);
                glActiveTexture(texture_);
                if (blend_flag) {
                    glEnable(GL_BLEND);
                    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                }

                glEnableClientState(GL_VERTEX_ARRAY);
                glVertexPointer(3, GL_FLOAT, 0, &params_.get_vertices()[0]);

                glEnableClientState(GL_TEXTURE_COORD_ARRAY);
                glTexCoordPointer(2, GL_FLOAT, 0, &params_.get_tex_coord()[0]);

                glDrawElements(GL_TRIANGLES, params_.get_indices().size(), GL_UNSIGNED_BYTE, &params_.get_indices()[0]);

                glDisableClientState(GL_TEXTURE_COORD_ARRAY);
                glDisableClientState(GL_VERTEX_ARRAY);

                if (blend_flag) glDisable(GL_BLEND);
                glDisable(GL_TEXTURE_2D);
            }

        private:
            unsigned int texture_;
            ImageParams params_;

    };
}

#endif // OPENGLTEXTURES_HPP_INCLUDED