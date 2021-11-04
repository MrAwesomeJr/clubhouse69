namespace tex {
    class imageParams{
        public:
            // coordinates of displayed image are calculated based off of this value
            double x = 0;
            double y = 0;

            // width and height of the displayed image (affected by mipmap)
            int width = 10;
            int height = 10;

            // resolution of original image. Usually obtained dynamically through stbi_load
            int resolutionX = 0;
            int resolutionY = 0;


            // Vertices, Indices and TexCoord by default set up a square (displays the entire image)
            // potential points that may be used to display the image
            std::vector<GLfloat> Vertices = {(float)x, (float)y, 0,
                                             (float)x + width, (float)y, 0,
                                             (float)x + (float)width, (float)y + (float)height, 0,
                                             (float)x, (float)y + (float)height, 0};

            // actual points that are used. organised into groups of 3.
            std::vector<GLubyte> Indices = {0,1,2,
                                            0,2,3};

            // where to display the texture based on Vertices and Indices
            std::vector<GLfloat> TexCoord = {0, 0,
                                             1, 0,
                                             1, 1,
                                             0, 1};

            void setParams(double inputX, double inputY, int inputHeight, int inputWidth, bool squareFlag = true){
                x = inputX;
                y = inputY;
                height = inputHeight;
                width = inputWidth;
                if (squareFlag) autoSquare();
            }

            void autoSquare(){
                Vertices = {(float)x, (float)y, 0,
                            (float)x + width, (float)y, 0,
                            (float)x + (float)width, (float)y + (float)height, 0,
                            (float)x, (float)y + (float)height, 0};
            }
    };

    void loadTexture(unsigned char *file, unsigned int texture, imageParams params, GLenum minFilter = GL_LINEAR, GLenum magFilter = GL_NEAREST){
        GLint oldTexture;
        glGetIntegerv(GL_ACTIVE_TEXTURE, &oldTexture);

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glActiveTexture(texture);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, params.resolutionX, params.resolutionY, 0, GL_RGBA, GL_UNSIGNED_BYTE, file);

        glActiveTexture(oldTexture);
    }

    void drawTexture(unsigned int texture, imageParams params, bool blendFlag = true){
    /*need to input:
    texture
    position
    area that texture covers (texcoord)
    area that shape covers (vertices + indices)
    blendflag
    */
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture);
        if (blendFlag)
        {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }

        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, &params.Vertices[0]);

        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glTexCoordPointer(2, GL_FLOAT, 0, &params.TexCoord[0]);

        glDrawElements(GL_TRIANGLES, params.Indices.size(), GL_UNSIGNED_BYTE, &params.Indices[0]);

        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);

        if (blendFlag) glDisable(GL_BLEND);
        glDisable(GL_TEXTURE_2D);
    }
}