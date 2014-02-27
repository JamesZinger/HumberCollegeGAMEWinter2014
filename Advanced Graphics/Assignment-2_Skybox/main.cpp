#pragma region Defines

#define __APP_TITLE__ "Skybox Test -James Zinger"
#define __LIGHTING_SHADER_VERT_PATH__ "StandardShading.vertexshader"
#define __LIGHTING_SHADER_FRAG_PATH__ "StandardShading.fragmentshader"
#define __SKYBOX_SHADER_VERT_PATH__ "SkyboxVertShader.vertexshader"
#define __SKYBOX_SHADER_FRAG_PATH__ "SkyboxFragShader.fragmentshader"

#define WIN32
#define _WINDOWS
#define TW_STATIC 
#define TW_NO_LIB_PRAGMA
#define TW_NO_DIRECT3D 
#define GLEW_STATIC

#pragma endregion


#pragma region Includes

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GL/glfw.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>



#include <shader.hpp>
#include <texture.hpp>
#include <objloader.hpp>
#include <vboindexer.hpp>
#include <controls.hpp>

#pragma endregion


#pragma region Libs

#pragma comment (lib, "kernel32.lib")
#pragma comment (lib, "user32.lib")
#pragma comment (lib, "gdi32.lib")
#pragma comment (lib, "winspool.lib")
#pragma comment (lib, "shell32.lib")
#pragma comment (lib, "ole32.lib")
#pragma comment (lib, "oleaut32.lib")
#pragma comment (lib, "uuid.lib")
#pragma comment (lib, "comdlg32.lib")
#pragma comment (lib, "advapi32.lib")
#pragma comment (lib, "glu32.lib")
#pragma comment (lib, "opengl32.lib")

#pragma comment (lib, "../external/lib/GLFW_276.lib")
#pragma comment (lib, "../external/lib/GLEW_190.lib")

#pragma endregion


using namespace glm;
using std::vector;

void glfwInitalization();

void glInitalization();

int main( void )
{

	glfwInitalization();

	glInitalization();

	GLuint VertexArrayID;
	glGenVertexArrays( 1, &VertexArrayID );
	glBindVertexArray( VertexArrayID );

	GLuint SkyboxShaderID = LoadShaders( __SKYBOX_SHADER_VERT_PATH__, __SKYBOX_SHADER_FRAG_PATH__ );

	GLuint u_viewMat = glGetUniformLocation( SkyboxShaderID, "V" );
	GLuint u_projMat = glGetUniformLocation( SkyboxShaderID, "P" );
	GLuint u_modlMat = glGetUniformLocation( SkyboxShaderID, "M" );
	GLuint u_mvpMat = glGetUniformLocation( SkyboxShaderID, "MVP" );

	GLuint Texture = loadRAW_Cube( "Images/posx.raw", "Images/posy.raw", "Images/posz.raw", 
								   "Images/negx.raw", "Images/negy.raw", "Images/negz.raw", 
								   2048, 2048 );

	// Get a handle for our "myTextureSampler" uniform
	GLuint TextureID = glGetUniformLocation( SkyboxShaderID, "cubeTextureSampler" );

	// Programatically make a square
	vector<vec3> vertices;

	vertices.push_back( vec3( -1, -1, -1 ) );
	vertices.push_back( vec3( 1, -1, -1 ) );
	vertices.push_back( vec3( -1, 1, -1 ) );

	vertices.push_back( vec3( 1, -1, -1 ) );
	vertices.push_back( vec3( 1, 1, -1 ) );
	vertices.push_back( vec3( -1, 1, -1 ) );

	
	GLuint vertexbuffer;
	glGenBuffers( 1, &vertexbuffer );
	glBindBuffer( GL_ARRAY_BUFFER, vertexbuffer );
	glBufferData( GL_ARRAY_BUFFER, vertices.size() * sizeof( glm::vec3 ), &vertices[ 0 ], GL_STATIC_DRAW );

	do
	{

		// Clear the screen
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		// Use our shader
		glUseProgram( SkyboxShaderID );

		// Compute the MVP matrix from keyboard and mouse input
		computeMatricesFromInputs();
		mat4 ProjectionMatrix = getProjectionMatrix();
		mat4 ViewMatrix = getViewMatrix();
		mat4 ModelMatrix = mat4( 1.0 );
		mat4 MVP = ProjectionMatrix * ViewMatrix;

		glActiveTexture( GL_TEXTURE0 );
		glBindTexture( GL_TEXTURE_CUBE_MAP, Texture );

		glUniform1i( TextureID, 0 );

		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		glUniformMatrix4fv( u_projMat, 1, GL_FALSE, &ProjectionMatrix[ 0 ][ 0 ] );
		glUniformMatrix4fv( u_viewMat, 1, GL_FALSE, &ViewMatrix[ 0 ][ 0 ] );
		glUniformMatrix4fv( u_modlMat, 1, GL_FALSE, &ModelMatrix[ 0 ][ 0 ] );
		glUniformMatrix4fv( u_mvpMat, 1, GL_FALSE, &MVP[ 0 ][ 0 ] );

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray( 0 );
		glBindBuffer( GL_ARRAY_BUFFER, vertexbuffer );
		glVertexAttribPointer(
			0,                  // attribute
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
			);

		// Draw the triangles !
		glDrawArrays( GL_TRIANGLES, 0, vertices.size() );

		glDisableVertexAttribArray( 0 );

		// Swap buffers
		glfwSwapBuffers();


	} // Check if the ESC key was pressed or the window was closed
	while ( glfwGetKey( GLFW_KEY_ESC ) != GLFW_PRESS &&
	glfwGetWindowParam( GLFW_OPENED ) );

	// Cleanup VBO and shader
	glDeleteBuffers( 1, &vertexbuffer );
	glDeleteProgram( SkyboxShaderID );
	glDeleteVertexArrays( 1, &VertexArrayID );

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

void glfwInitalization()
{
	// Initialise GLFW
	if ( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		system( "pause" );
		exit( -1 );
	}

	glfwOpenWindowHint( GLFW_FSAA_SAMPLES, 4 );
	glfwOpenWindowHint( GLFW_OPENGL_VERSION_MAJOR, 3 );
	glfwOpenWindowHint( GLFW_OPENGL_VERSION_MINOR, 3 );
	glfwOpenWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

	// Open a window and create its OpenGL context
	if ( !glfwOpenWindow( 1024, 768, 0, 0, 0, 0, 32, 0, GLFW_WINDOW ) )
	{
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		glfwTerminate();
		system( "pause" );
		exit( -1 );
	}

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if ( glewInit() != GLEW_OK )
	{
		fprintf( stderr, "Failed to initialize GLEW\n" );
		system( "pause" );
		exit(-1);
	}

	glfwSetWindowTitle( __APP_TITLE__ );

	// Ensure we can capture the escape key being pressed below
	glfwEnable( GLFW_STICKY_KEYS );
	glfwSetMousePos( 1024 / 2, 768 / 2 );
}

void glInitalization()
{
	// Dark blue background
	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );

	// Enable depth test
	glEnable( GL_DEPTH_TEST );

	// Accept fragment if it closer to the camera than the former one
	glDepthFunc( GL_LESS );

	// Cull triangles which normal is not towards the camera
	glEnable( GL_CULL_FACE );
}