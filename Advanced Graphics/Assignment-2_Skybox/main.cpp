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
using namespace glm;

#include <shader.hpp>
#include <objloader.hpp>
#include <vboindexer.hpp>

#define __APP_TITLE__ "Skybox Test -James Zinger"
#define __LIGHTING_SHADER_VERT_PATH__ "StandardShading.vertexshader"
#define __LIGHTING_SHADER_FRAG_PATH__ "StandardShading.fragmentshader"
#define __SKYBOX_SHADER_VERT_PATH__ ""
#define __SKYBOX_SHADER_FRAG_PATH__ ""

int main( void )
{
	// Initialise GLFW
	if ( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		return -1;
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
		return -1;
	}

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if ( glewInit() != GLEW_OK )
	{
		fprintf( stderr, "Failed to initialize GLEW\n" );
		return -1;
	}

	glfwSetWindowTitle( __APP_TITLE__ );

	// Ensure we can capture the escape key being pressed below
	glfwEnable( GLFW_STICKY_KEYS );
	glfwSetMousePos( 1024 / 2, 768 / 2 );

	// Dark blue background
	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );

	// Enable depth test
	glEnable( GL_DEPTH_TEST );
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc( GL_LESS );

	// Cull triangles which normal is not towards the camera
	glEnable( GL_CULL_FACE );

	GLuint VertexArrayID;
	glGenVertexArrays( 1, &VertexArrayID );
	glBindVertexArray( VertexArrayID );

	GLuint LightingShaderID = LoadShaders( __LIGHTING_SHADER_VERT_PATH__, __LIGHTING_SHADER_FRAG_PATH__ );
	GLuint SkyboxShaderID = LoadShaders( __SKYBOX_SHADER_VERT_PATH__, __SKYBOX_SHADER_FRAG_PATH__ );

	// Get a handle for our "MVP" uniform
	GLuint MatrixID = glGetUniformLocation( LightingShaderID, "MVP" );
	GLuint ViewMatrixID = glGetUniformLocation( LightingShaderID, "V" );
	GLuint ModelMatrixID = glGetUniformLocation( LightingShaderID, "M" );
	GLuint NormalMatrixID = glGetUniformLocation( LightingShaderID, "NormalMatrix" );
	GLuint SpecularHardnessID = glGetUniformLocation( LightingShaderID, "SpecularHardness" );

	// Read our .obj file
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	bool res = loadOBJ( "sphere.obj", vertices, uvs, normals );

	// Load it into a VBO

	GLuint vertexbuffer;
	glGenBuffers( 1, &vertexbuffer );
	glBindBuffer( GL_ARRAY_BUFFER, vertexbuffer );
	glBufferData( GL_ARRAY_BUFFER, vertices.size() * sizeof( glm::vec3 ), &vertices[ 0 ], GL_STATIC_DRAW );

	GLuint uvbuffer;
	glGenBuffers( 1, &uvbuffer );
	glBindBuffer( GL_ARRAY_BUFFER, uvbuffer );
	glBufferData( GL_ARRAY_BUFFER, uvs.size() * sizeof( glm::vec2 ), &uvs[ 0 ], GL_STATIC_DRAW );

	GLuint normalbuffer;
	glGenBuffers( 1, &normalbuffer );
	glBindBuffer( GL_ARRAY_BUFFER, normalbuffer );
	glBufferData( GL_ARRAY_BUFFER, normals.size() * sizeof( glm::vec3 ), &normals[ 0 ], GL_STATIC_DRAW );

	// Get a handle for our "LightPosition" uniform
	glUseProgram( LightingShaderID );
	GLuint LightID = glGetUniformLocation( LightingShaderID, "LightPosition_worldspace" );
	GLuint LightColorID = glGetUniformLocation( LightingShaderID, "LightColor" );
	GLuint LightPowerID = glGetUniformLocation( LightingShaderID, "LightPower" );

	float theta = 0.0f;

	glm::mat4 ProjectionMatrix = glm::perspective( 40.0f, 4.0f / 3.0f, 0.1f, 1000.0f );
	glm::mat4 ViewMatrix = glm::lookAt( vec3( 0, 0, 10 ), vec3( 0, 0, 0 ), vec3( 0, 1, 0 ) );

	glUniform1f( SpecularHardnessID, 2.0f );
	glUniform1f( LightPowerID, 20.0f );
	glUniform3f( LightColorID, 1.0f, 1.0f, 1.0f );

	do
	{

		// Clear the screen
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		// Use our shader
		glUseProgram( LightingShaderID );

		// Compute the MVP matrix from keyboard and mouse input
		//computeMatricesFromInputs();

		glm::mat4 ModelMatrix = glm::mat4( 1.0 );
		ModelMatrix = glm::rotate( ModelMatrix, theta, vec3( 0, 1, 0 ) );
		ModelMatrix = glm::translate( ModelMatrix, vec3( 4, 0, 0 ) );
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		glUniformMatrix4fv( MatrixID, 1, GL_FALSE, &MVP[ 0 ][ 0 ] );
		glUniformMatrix4fv( ModelMatrixID, 1, GL_FALSE, &ModelMatrix[ 0 ][ 0 ] );
		glUniformMatrix4fv( ViewMatrixID, 1, GL_FALSE, &ViewMatrix[ 0 ][ 0 ] );

		bool isUniformScaling = true;

		if ( ModelMatrix[ 0 ][ 0 ] != ModelMatrix[ 1 ][ 1 ] || ModelMatrix[ 0 ][ 0 ] != ModelMatrix[ 2 ][ 2 ] || ModelMatrix[ 1 ][ 1 ] != ModelMatrix[ 2 ][ 2 ] )
		{
			isUniformScaling = false;
		}

		mat4 MV = ModelMatrix*ViewMatrix;

		if ( !isUniformScaling )
		{
			mat4 Norm = glm::transpose( MV );
			Norm = glm::inverse( Norm );
			glUniformMatrix4fv( NormalMatrixID, 1, GL_FALSE, &Norm[ 0 ][ 0 ] );
		}
		else
		{
			glUniformMatrix4fv( NormalMatrixID, 1, GL_FALSE, &MV[ 0 ][ 0 ] );
		}

		glm::vec3 lightPos = glm::vec3( 0, 2, 2 );
		glUniform3f( LightID, lightPos.x, lightPos.y, lightPos.z );


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

		// 2nd attribute buffer : UVs
		glEnableVertexAttribArray( 1 );
		glBindBuffer( GL_ARRAY_BUFFER, uvbuffer );
		glVertexAttribPointer(
			1,                                // attribute
			2,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
			);

		// 3rd attribute buffer : normals
		glEnableVertexAttribArray( 2 );
		glBindBuffer( GL_ARRAY_BUFFER, normalbuffer );
		glVertexAttribPointer(
			2,                                // attribute
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
			);

		// Draw the triangles !
		glDrawArrays( GL_TRIANGLES, 0, vertices.size() );

		glDisableVertexAttribArray( 0 );
		glDisableVertexAttribArray( 1 );
		glDisableVertexAttribArray( 2 );

		// Swap buffers
		glfwSwapBuffers();

		theta += 1.0f;

		if ( theta >= 360 )
			theta -= 360;

	} // Check if the ESC key was pressed or the window was closed
	while ( glfwGetKey( GLFW_KEY_ESC ) != GLFW_PRESS &&
	glfwGetWindowParam( GLFW_OPENED ) );

	// Cleanup VBO and shader
	glDeleteBuffers( 1, &vertexbuffer );
	glDeleteBuffers( 1, &uvbuffer );
	glDeleteBuffers( 1, &normalbuffer );
	glDeleteProgram( programID );
	glDeleteVertexArrays( 1, &VertexArrayID );

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}
