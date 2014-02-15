#ifndef TEXTURE_HPP
#define TEXTURE_HPP

// Load a .BMP file using our custom loader
GLuint loadBMP_custom(const char * imagepath);

// Load a .TGA file using GLFW's own loader
GLuint loadTGA_glfw(const char * imagepath);

// Load a .DDS file using GLFW's own loader
GLuint loadDDS(const char * imagepath);

GLuint loadRAW_Cube( const char* posxPath, const char* posyPath, const char* poszPath,
					 const char* negxPath, const char* negyPath, const char* negzPath,
					 const int width, const int height);

#endif