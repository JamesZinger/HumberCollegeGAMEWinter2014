#pragma region Defines

#define __APP_TITLE__ "Tetrahedron VAO example -James Zinger"
#define __STANDARD_SHADER_VERT_PATH__ "StandardShading.vertexshader"
#define __STANDARD_SHADER_FRAG_PATH__ "StandardShading.fragmentshader"

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


struct StandardShader
{
	GLuint ShaderID;
	GLuint MVPID;
	GLuint VID;
	GLuint MID;
	GLuint LightPositionID;
	GLuint NormalMatrixID;
	GLuint MVID;
	GLuint SpecularHardnessID;
	GLuint LightColorID;
	GLuint LightPowerID;
};

struct Tetrahedron
{
	StandardShader shader;
	GLuint VertexBufferLoc;
	GLuint NormalBufferLoc;
	GLuint UVBufferLoc;
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

	// Using what I already have (Slightly redundant)
	vector<vec3> TetVerts;
	vector<vec2> uvs;
	vector<vec3> norms;

	bool res = loadOBJ( "Tetrahedron.obj", TetVerts, uvs, norms );

	Tetrahedron tetrahedron;

	{
		StandardShader colorShader;

		colorShader.ShaderID = LoadShaders( __STANDARD_SHADER_VERT_PATH__, __STANDARD_SHADER_FRAG_PATH__ );
		colorShader.MVPID = glGetUniformLocation( colorShader.ShaderID, "MVP" );
		colorShader.VID = glGetUniformLocation( colorShader.ShaderID, "V" );
		colorShader.MID = glGetUniformLocation( colorShader.ShaderID, "M" );
		colorShader.LightPositionID = glGetUniformLocation( colorShader.ShaderID, "LightPosition_wodspace" );
		colorShader.NormalMatrixID = glGetUniformLocation( colorShader.ShaderID, "NormalMatrix" );
		colorShader.MVID = glGetUniformLocation( colorShader.ShaderID, "MV" );
		colorShader.SpecularHardnessID = glGetUniformLocation( colorShader.ShaderID, "SpecularHardness" );
		colorShader.LightColorID = glGetUniformLocation( colorShader.ShaderID, "LightColor" );
		colorShader.LightPowerID = glGetUniformLocation( colorShader.ShaderID, "LightPower" );
		
		tetrahedron.shader = colorShader;
	}



	// Sending Data to the card

	glGenBuffers( 1, &(tetrahedron.VertexBufferLoc) );
	glBindBuffer( GL_ARRAY_BUFFER, tetrahedron.VertexBufferLoc );
	glBufferData( GL_ARRAY_BUFFER, TetVerts.size() * sizeof( vec3 ), &TetVerts[ 0 ], GL_STATIC_DRAW );
	
	glGenBuffers( 1, &( tetrahedron.UVBufferLoc ) );
	glBindBuffer( GL_ARRAY_BUFFER, tetrahedron.UVBufferLoc );
	glBufferData( GL_ARRAY_BUFFER, uvs.size() * sizeof( vec2 ), &uvs[ 0 ], GL_STATIC_DRAW );

	glGenBuffers( 1, &( tetrahedron.NormalBufferLoc ) );
	glBindBuffer( GL_ARRAY_BUFFER, tetrahedron.NormalBufferLoc );
	glBufferData( GL_ARRAY_BUFFER, norms.size() * sizeof( vec3 ), &norms[ 0 ], GL_STATIC_DRAW );

	vec3 Cam_Pos = vec3( 0, 0, 5 );
	vec3 Light_Color = vec3( 1, 1, 1 );
	vec3 Light_Pos = vec3( 3, 3, 0 );
	float Light_Power = 1;
	float SpecHard = 10;

	

	do
	{

		// Clear the screen
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		mat4 ProjectionMatrix = perspective( 45.0f, 1024.0f / 768.0f, 0.1f, 1000.0f );
		mat4 ViewMatrix = lookAt( Cam_Pos, vec3( 0, 0, 0 ), vec3( 0, 1, 0 ) );
		
		ViewMatrix = rotate( ViewMatrix, cameraTheta, vec3( 0, 1, 0 ) );

		{
			
			mat4 ModelMat = mat4( 1.0f );
			mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMat;
			mat3 MVP3 = mat3( 1.0f );

			for ( int i = 0; i < 3; i++ )
			{
				for ( int j = 0; j < 3; j++ )
				{
					MVP3[ i ][ j ] = MVP[ i ][ j ];
				}
			}



			mat3 NormalMat = inverse( MVP3 );
			NormalMat = transpose( NormalMat );
			
			mat4 MV = ViewMatrix * ModelMat;

			glUseProgram( tetrahedron.shader.ShaderID );

			// Set the Floats in the shader
			glUniform1f( tetrahedron.shader.LightPowerID, Light_Power );
			glUniform1f( tetrahedron.shader.SpecularHardnessID, SpecHard );

			// Set the Vectors in the shader
			glUniform3fv( tetrahedron.shader.LightColorID, 1, &Light_Color[ 0 ] );
			glUniform3fv( tetrahedron.shader.LightPositionID, 1, &Light_Pos[ 0 ] );

			
			// Set the Matrix in the shader
			glUniformMatrix4fv( tetrahedron.shader.MVPID, 1, GL_FALSE, &MVP[ 0 ][ 0 ] );
			glUniformMatrix4fv( tetrahedron.shader.MID, 1, GL_FALSE, &ModelMat[ 0 ][ 0 ] );
			glUniformMatrix4fv( tetrahedron.shader.VID, 1, GL_FALSE, &ViewMatrix[ 0 ][ 0 ] );
			glUniformMatrix4fv( tetrahedron.shader.NormalMatrixID, 1, GL_FALSE, &NormalMat[ 0 ][ 0 ] );
			glUniformMatrix4fv( tetrahedron.shader.MVID, 1, GL_FALSE, &MV[ 0 ][ 0 ] );

			glEnableVertexAttribArray( 0 );
			glBindBuffer( GL_ARRAY_BUFFER, tetrahedron.VertexBufferLoc );
			glVertexAttribPointer(
				0,
				3,
				GL_FLOAT,
				GL_FALSE,
				0,
				(void*)0
				);

			glEnableVertexAttribArray( 1 );
			glBindBuffer( GL_ARRAY_BUFFER, tetrahedron.UVBufferLoc );
			glVertexAttribPointer(
				1,
				3,
				GL_FLOAT,
				GL_FALSE,
				0,
				(void*)0
				);

			glEnableVertexAttribArray( 2 );
			glBindBuffer( GL_ARRAY_BUFFER, tetrahedron.NormalBufferLoc );
			glVertexAttribPointer(
				2,
				3,
				GL_FLOAT,
				GL_FALSE,
				0,
				(void*)0
				);

			glDrawArrays( GL_TRIANGLES, 0, TetVerts.size() );

			glDisableVertexAttribArray( 2 );
			glDisableVertexAttribArray( 1 );
			glDisableVertexAttribArray( 0 );
			
		}

		// Swap buffers
		glfwSwapBuffers();

		cameraTheta += 0.80f;
		if ( cameraTheta > 360.0f )
			cameraTheta -= 360.0f;


	} // Check if the ESC key was pressed or the window was closed
	while ( glfwGetKey( GLFW_KEY_ESC ) != GLFW_PRESS &&
	glfwGetWindowParam( GLFW_OPENED ) );

	// Cleanup VBO and shader
	glDeleteBuffers( 1, &tetrahedron.VertexBufferLoc );
	glDeleteProgram( tetrahedron.shader.ShaderID );
	
	glDeleteVertexArrays( 1, &VertexArrayID );

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

void glfwInitalization()
{
	// Initialize GLFW
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