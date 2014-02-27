#pragma region Defines

#define __APP_TITLE__ "Mirror Test -James Zinger"
#define __SKYBOX_SHADER_VERT_PATH__ "SkyboxVertShader.vertexshader"
#define __SKYBOX_SHADER_FRAG_PATH__ "SkyboxFragShader.fragmentshader"
#define __MIRROR_SHADER_VERT_PATH__ "MirrorShading.vertexshader"
#define __MIRROR_SHADER_FRAG_PATH__ "MirrorShading.fragmentshader"

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


struct SkyboxShader
{
	GLuint ShaderID;
	GLuint VID;
	GLuint MID;
	GLuint CubeSamplerID;
};

struct Skybox
{
	SkyboxShader Shader;
	GLuint VertexBufferLoc;
	GLuint TextureBufferLoc;
};

struct MirrorShader
{
	GLuint ShaderID;
	GLuint MVPID;
	GLuint MID;
	GLuint VID;
	GLuint CubeSamplerID;
	GLuint CameraPositionID;
};

struct Sphere
{
	MirrorShader Shader;
	GLuint VertexBufferLoc;
	GLuint NormalBufferLoc;
	GLuint TextureBufferLoc;
};

float cameraTheta = 0.0f;

void glfwInitalization();

void glInitalization();

int main( void )
{

	glfwInitalization();

	glInitalization();

	GLuint VertexArrayID;
	glGenVertexArrays( 1, &VertexArrayID );
	glBindVertexArray( VertexArrayID );

	// Using what I already have (Slightly redudant)
	vector<vec3> BallVerts;
	vector<vec2> uvs;
	vector<vec3> norms;

	bool res = loadOBJ( "sphere.obj", BallVerts, uvs, norms );

	Skybox sky;

	{
		SkyboxShader skyshader;

		skyshader.ShaderID = LoadShaders( __SKYBOX_SHADER_VERT_PATH__, __SKYBOX_SHADER_FRAG_PATH__ );
		skyshader.VID = glGetUniformLocation( skyshader.ShaderID, "V" );
		skyshader.MID = glGetUniformLocation( skyshader.ShaderID, "M" );
		skyshader.CubeSamplerID = glGetUniformLocation( skyshader.ShaderID, "cubeTextureSampler" );

		sky.Shader = skyshader;
	}

	Sphere ball;

	{
		MirrorShader shader;
		shader.ShaderID = LoadShaders( __MIRROR_SHADER_VERT_PATH__, __MIRROR_SHADER_FRAG_PATH__ );
		shader.MVPID = glGetUniformLocation( shader.ShaderID, "MVP" );
		shader.MID = glGetUniformLocation( shader.ShaderID, "M" );
		shader.VID = glGetUniformLocation( shader.ShaderID, "V" );
		shader.CubeSamplerID = glGetUniformLocation( shader.ShaderID, "cubeTextureSampler" );
		shader.CameraPositionID = glGetUniformLocation( shader.ShaderID, "Camera_Position" );

		ball.Shader = shader;
	}



	sky.TextureBufferLoc = loadRAW_Cube( "Images/posx.raw", "Images/posy.raw", "Images/posz.raw",
										 "Images/negx.raw", "Images/negy.raw", "Images/negz.raw",
										 2048, 2048 );

	ball.TextureBufferLoc = sky.TextureBufferLoc;



	// Programatically make a square
	vector<vec3> SkyVerts;

	SkyVerts.push_back( vec3( -1, -1, -1 ) );
	SkyVerts.push_back( vec3( 1, -1, -1 ) );
	SkyVerts.push_back( vec3( -1, 1, -1 ) );

	SkyVerts.push_back( vec3( 1, -1, -1 ) );
	SkyVerts.push_back( vec3( 1, 1, -1 ) );
	SkyVerts.push_back( vec3( -1, 1, -1 ) );


	// Sending Data to the card

	glGenBuffers( 1, &(sky.VertexBufferLoc) );
	glBindBuffer( GL_ARRAY_BUFFER, sky.VertexBufferLoc );
	glBufferData( GL_ARRAY_BUFFER, SkyVerts.size() * sizeof( vec3 ), &SkyVerts[ 0 ], GL_STATIC_DRAW );
	
	glGenBuffers( 1, &ball.VertexBufferLoc );
	glBindBuffer( GL_ARRAY_BUFFER, ball.VertexBufferLoc );
	glBufferData( GL_ARRAY_BUFFER, BallVerts.size() * sizeof( vec3 ), &BallVerts[ 0 ], GL_STATIC_DRAW );

	glGenBuffers( 1, &ball.NormalBufferLoc );
	glBindBuffer( GL_ARRAY_BUFFER, ball.NormalBufferLoc );
	glBufferData( GL_ARRAY_BUFFER, norms.size() * sizeof( vec3 ), &norms[ 0 ], GL_STATIC_DRAW );

	vec3 Cam_Pos = vec3( 0, 0, 10 );

	do
	{

		// Clear the screen
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		

		mat4 ProjectionMatrix = perspective( 45.0f, 1024.0f / 768.0f, 0.1f, 1000.0f );
		mat4 ViewMatrix = lookAt( Cam_Pos, vec3( 0, 0, 0 ), vec3( 0, 1, 0 ) );
		
		ViewMatrix = rotate( ViewMatrix, cameraTheta, vec3( 0, 1, 0 ) );

		glDisable( GL_DEPTH_TEST );

		// Draw the Skybox
		{
			glUseProgram( sky.Shader.ShaderID );

			// Set the Texture to use
			glActiveTexture( 0 );
			glBindTexture( GL_TEXTURE_CUBE_MAP, sky.TextureBufferLoc );

			// Set the samplers texture to use
			glUniform1i( sky.Shader.CubeSamplerID, 0 );

			mat4 ModelMat = mat4( 1.0f );
			
			//Set the "V" Matrix for the skybox
			glUniformMatrix4fv( sky.Shader.VID, 1, GL_FALSE, &ViewMatrix[ 0 ][ 0 ] );
			glUniformMatrix4fv( sky.Shader.MID, 1, GL_FALSE, &ModelMat[ 0 ][ 0 ] );

			// 1rst attribute buffer : vertices
			glEnableVertexAttribArray( 0 );
			glBindBuffer( GL_ARRAY_BUFFER, sky.VertexBufferLoc );
			glVertexAttribPointer(
				0,                  // attribute
				3,                  // size
				GL_FLOAT,           // type
				GL_FALSE,           // normalized?
				0,                  // stride
				(void*)0            // array buffer offset
				);


			//glDrawArrays( GL_TRIANGLES, 0, SkyVerts.size() );

			glDisableVertexAttribArray( 0 );

		}

		glEnable( GL_DEPTH_TEST );

		{

			mat4 ModelMat = mat4( 1.0f );
			mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMat;
			glUseProgram( ball.Shader.ShaderID );

			// Set the Texture to use
			glActiveTexture( 0 );
			glBindTexture( GL_TEXTURE_CUBE_MAP, ball.TextureBufferLoc );

			// Set the samplers texture to use
			glUniform1i( ball.Shader.CubeSamplerID, 0 );

			// Set the Camera Position Uniform

			glUniform3fv( ball.Shader.CameraPositionID, 1, &Cam_Pos[ 0 ] );

			// Set the "MVP" "M" and "V" uniforms
			glUniformMatrix4fv( ball.Shader.VID, 1, GL_FALSE, &ViewMatrix[ 0 ][ 0 ] );
			glUniformMatrix4fv( ball.Shader.MID, 1, GL_FALSE, &ModelMat[ 0 ][ 0 ] );
			glUniformMatrix4fv( ball.Shader.MVPID, 1, GL_FALSE, &MVP[ 0 ][ 0 ] );

			glEnableVertexAttribArray( 0 );
			glBindBuffer( GL_ARRAY_BUFFER, ball.VertexBufferLoc );
			glVertexAttribPointer(
				0,
				3,
				GL_FLOAT,
				GL_FALSE,
				0,
				(void*)0
				);

			glEnableVertexAttribArray( 1 );
			glBindBuffer( GL_ARRAY_BUFFER, ball.NormalBufferLoc );
			glVertexAttribPointer(
				1,
				3,
				GL_FLOAT,
				GL_FALSE,
				0,
				(void*)0
				);

			glDrawArrays( GL_TRIANGLES, 0, BallVerts.size() );

			glDisableVertexAttribArray( 1 );
			glDisableVertexAttribArray( 0 );
			
		}

		// Swap buffers
		glfwSwapBuffers();

		cameraTheta += 0.50f;
		if ( cameraTheta > 360.0f )
			cameraTheta -= 360.0f;


	} // Check if the ESC key was pressed or the window was closed
	while ( glfwGetKey( GLFW_KEY_ESC ) != GLFW_PRESS &&
	glfwGetWindowParam( GLFW_OPENED ) );

	// Cleanup VBO and shader
	glDeleteBuffers( 1, &sky.VertexBufferLoc );
	glDeleteBuffers( 1, &ball.VertexBufferLoc );
	glDeleteBuffers( 1, &ball.NormalBufferLoc );
	glDeleteProgram( sky.Shader.ShaderID );
	glDeleteProgram( ball.Shader.ShaderID );
	glDeleteTextures( 1, &sky.TextureBufferLoc );

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
		exit( -1 );
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