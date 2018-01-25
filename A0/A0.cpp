#include "A0.hpp"
#include "cs488-framework/GlErrorCheck.hpp"
//#include "src/SOIL.hpp"
#include <iostream>

#include <imgui/imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/io.hpp>


using namespace glm;
using namespace std;

const float PI = 3.14159265f;
GLuint textureId;

GLuint loadDDS(const char * imagepath);

//----------------------------------------------------------------------------------------
// Constructor
A0::A0()
	: m_shape_color(glm::vec3(1.0f, 1.0f, 1.0f)),
	  m_shape_translation(vec2(0.0f)),
	  m_shape_size(1.0f),
	  m_shape_rotation(0.0f),
	  m_mouse_GL_coordinate(dvec2(0.0)),
	  m_mouseButtonActive(false)
{

}

//----------------------------------------------------------------------------------------
// Destructor
A0::~A0()
{

}


//----------------------------------------------------------------------------------------
GLuint A0::LoadTextureRAW( const char * imagepath )
{


/*
    int width, height;
    unsigned char * data;      //where to store data
    FILE * file;
    file = fopen(filename, "rb");
    if( NULL == file){
       cerr << "No such bmp file"<<endl;
       return 0;
    }//end if null
    width = 256;
    height = 256;
    data = (unsigned char *)malloc( width * height * 3);
    fread( data, width*height*3,1,file );
    fclose( file );
*/


//http://www.opengl-tutorial.org/beginners-tutorials/tutorial-5-a-textured-cube/

    // Data read from the header of the BMP file
    unsigned char header[54]; // Each BMP file begins by a 54-bytes header
    unsigned int dataPos;     // Position in the file where the actual data begins
    int width, height,format;
    unsigned int imageSize;   // = width*height*3
    // Actual RGB data
    unsigned char * data;
//    data =  (GLbyte *)SOIL_load_image(imagepath, &width,&height,&format,SOIL_LOAD_RGB);




    // Open the file
    FILE * file = fopen(imagepath,"rb");
    if (!file){cout << "Image could not be opened"<<endl; return 0;}


    if ( fread(header, 1, 54, file)!=54 ){ // If not 54 bytes read : problem
        cout << "Not a correct BMP file1"<<endl;
        return false;
    }//end if fread

    if ( header[0]!='B' || header[1]!='M' ){
        cout << "Not a correct BMP file2"<<endl;
        return 0;
    }//end if header

    // Read ints from the byte array
    dataPos    = *(int*)&(header[0x0A]);
    imageSize  = *(int*)&(header[0x22]);
    width      = *(int*)&(header[0x12]);
    height     = *(int*)&(header[0x16]);

    // Some BMP files are misformatted, guess missing information
    if (imageSize==0)    imageSize=width*height*3; // 3 : one byte for each Red, Green and Blue component
    if (dataPos==0)      dataPos=54; // The BMP header is done that way

    // Create a buffer
    data = new unsigned char [imageSize];

    // Read the actual data from the file into the buffer
    fread(data,1,imageSize,file);

    //Everything is in memory now, the file can be closed
    fclose(file);
cout << "size is  " << imageSize <<endl;
//---------------------------



/*
    for(int i=0; i<width * height; ++i){
         int index = i*3;
cout <<"("<<*(short int*)&data[index]<<","<<data[index+1]<<","<<data[index+2]<<")"<<endl;
    }//end for i

*/
    glGenTextures(1, &textureId);    
   // glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D,textureId);

//    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_NEAREST );
//    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT );

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width , height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

  


    free( data );

    glBindTexture(GL_TEXTURE_2D, 0);
    

}// end LoadText


//----------------------------------------------------------------------------------------
/*
 * Called once, at program start.
 */
void A0::init()
{
	// Set the background colour.
	glClearColor(0.3, 0.5, 0.7, 1.0);

	createShaderProgram();

	enableVertexAttribIndices();

	uploadTriangleDataToVbo();

	mapVboDataToShaderAttributeLocation();

        //LoadTextureRAW("uvtemplate.bmp");
        //loadDDS("wood.dds");
        LoadTextureRAW("urbansp_ft.bmp");

}

//----------------------------------------------------------------------------------------
void A0::createShaderProgram()
{
	m_shader.generateProgramObject();
	m_shader.attachVertexShader( getAssetFilePath("VertexShader.vs").c_str() );
	m_shader.attachFragmentShader( getAssetFilePath("FragmentShader.fs").c_str() );
	m_shader.link();
}

//----------------------------------------------------------------------------------------
void A0::enableVertexAttribIndices()
{
	glGenVertexArrays(1, &m_vao_triangle);
	glBindVertexArray(m_vao_triangle);

	// Enable the attribute index location for "position" when rendering.
	GLint positionAttribLocation = m_shader.getAttribLocation( "position" );
	glEnableVertexAttribArray(positionAttribLocation);

// Enable the attribute index location for "textCoord" when rendering.
        GLint textAttribLocation = m_shader.getAttribLocation( "textCoord" );
        glEnableVertexAttribArray(textAttribLocation);


	// Restore defaults
	glBindVertexArray(0);

	CHECK_GL_ERRORS;
}

//----------------------------------------------------------------------------------------
void A0::uploadTriangleDataToVbo()
{
	// Vertex positions for triangle vertices.
	// Default OpenGL coordinates range from -1 to 1 in all directions (x,y,z).
	float half_sqrt3 = float(std::sqrt(3.0)) * 0.5f;

/*
	vec3 triangleVertices[] = {
		// Construct equalaterial triangle
		vec3(0.0f, 0.0f, 0.0f),
		vec3(0.25f, half_sqrt3 * 0.5f, 0.0),
		vec3(0.5f, 0.0f, 0.0f)
	};
*/
        GLfloat triangleVertices[] = {
              0.0f, 0.0f, 0.0f,
              0.0f, 0.0f,
              0.25f, half_sqrt3 * 0.5f, 0.0,
//              0.25f, half_sqrt3 *0.5f,
              0.25f, half_sqrt3 *0.5f,
              0.5f, 0.0f, 0.0f,
              0.5f, 0.0f
        };

	// Construct triangle centroid and move vertices so triangle is centered at origin
	vec3 centroid(0.0f);
//	for(const vec3 & v : triangleVertices) {
//		centroid += v;
//	}
	centroid /= 3.0f;
//	for(vec3 & v : triangleVertices) {
//		v -= centroid;
//	}


	// Generate a vertex buffer object to hold the triangle's vertex data.
	glGenBuffers(1, &m_vbo_triangle);

	//-- Upload triangle vertex data to the vertex buffer:
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_triangle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices,
			GL_STATIC_DRAW);


	// Unbind the target GL_ARRAY_BUFFER, now that we are finished using it.
	glBindBuffer(GL_ARRAY_BUFFER, 0);


        




	CHECK_GL_ERRORS;


}

//----------------------------------------------------------------------------------------
void A0::mapVboDataToShaderAttributeLocation()
{
	glBindVertexArray(m_vao_triangle);

	// Tell GL how to map data from the vertex buffer "m_vbo_triangle" into the
	// "position" vertex attribute index of our shader program.
	// This mapping information is stored in the Vertex Array Object "m_vao_triangle".
	// That is why we must bind "m_vao_triangle" first in the line above, so
	// that "m_vao_triangle" captures the data mapping issued by the call to
	// glVertexAttribPointer(...).
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_triangle);
	GLint positionAttribLocation = m_shader.getAttribLocation( "position" );
	glVertexAttribPointer(positionAttribLocation, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), nullptr);
//assign texture
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (GLvoid*)(3*sizeof(GL_FLOAT)));


//------------

	//-- Unbind target, and restore default values:
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	CHECK_GL_ERRORS;
}

//----------------------------------------------------------------------------------------
void A0::uploadUniformsToShader()
{
	m_shader.enable();
		// Query the shader program for the location number of the uniform named "color".
		GLint uniformLocation_colour = m_shader.getUniformLocation("colour");

		// Set the uniform's value.
		glUniform3f(uniformLocation_colour, m_shape_color.r, m_shape_color.g,
				m_shape_color.b);

		vec3 z_axis(0.0f,0.0f,1.0f);
		mat4 transform = glm::translate(mat4(), vec3(m_shape_translation, 0.0f));
		transform *= glm::scale(mat4(), vec3(m_shape_size));
		transform *= glm::rotate(mat4(), m_shape_rotation, z_axis);

		GLint uniformLocation_modelMatrix = m_shader.getUniformLocation("transform");
		glUniformMatrix4fv(uniformLocation_modelMatrix, 1, GL_FALSE,
				glm::value_ptr(transform));

	m_shader.disable();

	CHECK_GL_ERRORS;
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, before guiLogic().
 */
void A0::appLogic()
{
	// Place per frame, application logic here ...


	// Clamp shape size to be within a reasonable range
	m_shape_size = glm::clamp(m_shape_size, 0.0f, 10.0f);

	uploadUniformsToShader();
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, after appLogic(), but before the draw() method.
 */
void A0::guiLogic()
{
	static bool firstRun(true);
	if (firstRun) {
		ImGui::SetNextWindowPos(ImVec2(50, 50));
		firstRun = false;
	}

	static bool showDebugWindow(true);
	ImGuiWindowFlags windowFlags(ImGuiWindowFlags_AlwaysAutoResize);
	float opacity(0.5f);

	ImGui::Begin("Shape Properties", &showDebugWindow, ImVec2(100,100), opacity,
			windowFlags);

                // Create Button, and check if it was clicked:
                if( ImGui::Button( "Quit Application" ) ) {
                        glfwSetWindowShouldClose(m_window, GL_TRUE);
                }


                // Add more gui elements here here ...
                if( ImGui::Button( "Reset" ) ) {
                       m_shape_color = glm::vec3(1.0f, 1.0f, 1.0f);
                       m_shape_translation = vec2(0.0f);
                       m_shape_size = 1.0f;
                       m_shape_rotation = 0.0f;

                }

		// Retrieve red color component from slider and store in the first element of
		// m_shape_color.
		ImGui::SliderFloat("Red Channel", &m_shape_color.r, 0.0f, 1.0f);
                ImGui::SliderFloat("Green Channel", &m_shape_color.g, 0.0f, 1.0f);
                ImGui::SliderFloat("Blue Channel", &m_shape_color.b, 0.0f, 1.0f);
                ImGui::SliderFloat("Rotation Channel", &m_shape_rotation, 0.0f, 2 * 3.14159265358979323846f);




		ImGui::Text( "Framerate: %.1f FPS", ImGui::GetIO().Framerate );

	ImGui::End();
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, after guiLogic().
 */
void A0::draw()
{
	glBindVertexArray(m_vao_triangle);

	m_shader.enable();
//baind texture
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D,textureId);
             //   glUniform1i(glGetUniformLocation(m_shader.programId,"tex"),0);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	m_shader.disable();


	// Restore defaults
	glBindVertexArray(0);

	CHECK_GL_ERRORS;
}

//----------------------------------------------------------------------------------------
/*
 * Called once, after program is signaled to terminate.
 */
void A0::cleanup()
{

}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles cursor entering the window area events.
 */
bool A0::cursorEnterWindowEvent (
		int entered
) {
	bool eventHandled(false);

	// Fill in with event handling code...

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse cursor movement events.
 */
bool A0::mouseMoveEvent(double xPos, double yPos) {
	bool eventHandled(false);

	// Fill in with event handling code...

	// Translate current mouse position to GL coordinates:
	m_mouse_GL_coordinate = vec2 (
			(2.0f * xPos) / m_windowWidth - 1.0f,
			1.0f - ( (2.0f * yPos) / m_windowHeight)
	);

	if (m_mouseButtonActive) {
		m_shape_translation = m_mouse_GL_coordinate;
	}

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse button events.
 */
bool A0::mouseButtonInputEvent(int button, int actions, int mods) {
	bool eventHandled(false);

	// Fill in with event handling code...

	if (actions == GLFW_PRESS) {
		if (!ImGui::IsMouseHoveringAnyWindow()) {
			m_mouseButtonActive = true;
			m_shape_translation = m_mouse_GL_coordinate;
		}
	}

	if (actions == GLFW_RELEASE) {
		m_mouseButtonActive = false;
	}

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse scroll wheel events.
 */
bool A0::mouseScrollEvent(double xOffSet, double yOffSet) {
	bool eventHandled(false);

	// Fill in with event handling code...

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles window resize events.
 */
bool A0::windowResizeEvent(int width, int height) {
	bool eventHandled(false);

	// Fill in with event handling code...

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles key input events.
 */
bool A0::keyInputEvent(int key, int action, int mods) {
	bool eventHandled(false);

	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_EQUAL) {
			cout << "+ key pressed" << endl;

			// TODO - increase shape size.
                        if(m_shape_size < 5) 
                           m_shape_size += 0.2;
			eventHandled = true;
		}
		if (key == GLFW_KEY_MINUS) {
			cout << "- key pressed" << endl;

			// TODO - decrease shape size.
                        if(m_shape_size > 0.4) 
                           m_shape_size -= 0.2;
			eventHandled = true;
		}

                if (key == GLFW_KEY_Q) {
                        cout << "Q key pressed" << endl;
                        glfwSetWindowShouldClose(m_window, GL_TRUE);
                        eventHandled = true;
                }

                if (key == GLFW_KEY_R) {
                        cout << "R key pressed" << endl;
                        m_shape_color = glm::vec3(1.0f, 1.0f, 1.0f);
                        m_shape_translation = vec2(0.0f);
                        m_shape_size = 1.0f;
                        m_shape_rotation = 0.0f;
                        eventHandled = true;
                }

	}


	return eventHandled;
}

////////////////////////////////////


#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

GLuint loadDDS(const char * imagepath){

	unsigned char header[124];

	FILE *fp; 
 
	/* try to open the file */ 
	fp = fopen(imagepath, "rb"); 
	if (fp == NULL){
		printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", imagepath); getchar(); 
		return 0;
	}
   
	/* verify the type of file */ 
	char filecode[4]; 
	fread(filecode, 1, 4, fp); 
	if (strncmp(filecode, "DDS ", 4) != 0) { 
		fclose(fp); 
		return 0; 
	}
	
	/* get the surface desc */ 
	fread(&header, 124, 1, fp); 

	unsigned int height      = *(unsigned int*)&(header[8 ]);
	unsigned int width	     = *(unsigned int*)&(header[12]);
	unsigned int linearSize	 = *(unsigned int*)&(header[16]);
	unsigned int mipMapCount = *(unsigned int*)&(header[24]);
	unsigned int fourCC      = *(unsigned int*)&(header[80]);

 
	unsigned char * buffer;
	unsigned int bufsize;
	/* how big is it going to be including all mipmaps? */ 
	bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize; 
	buffer = (unsigned char*)malloc(bufsize * sizeof(unsigned char)); 
	fread(buffer, 1, bufsize, fp); 
	/* close the file pointer */ 
	fclose(fp);

	unsigned int components  = (fourCC == FOURCC_DXT1) ? 3 : 4; 
	unsigned int format;
	switch(fourCC) 
	{ 
	case FOURCC_DXT1: 
		format = 33777; 
		break; 
	case FOURCC_DXT3: 
		format = 33778; 
		break; 
	case FOURCC_DXT5: 
		format = 33779; 
		break; 
	default: 
		free(buffer); 
		return 0; 
	}

	// Create one OpenGL texture
	glGenTextures(1, &textureId);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureId);
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);	
	
	unsigned int blockSize = (format == 33777) ? 8 : 16; 
	unsigned int offset = 0;


	/* load the mipmaps */ 
	for (unsigned int level = 0; level < mipMapCount && (width || height); ++level) 
	{ 
		unsigned int size = ((width+3)/4)*((height+3)/4)*blockSize; 
		glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height,  
			0, size, buffer + offset); 
	 
		offset += size; 
		width  /= 2; 
		height /= 2; 

		// Deal with Non-Power-Of-Two textures. This code is not included in the webpage to reduce clutter.
		if(width < 1) width = 1;
		if(height < 1) height = 1;

	} 

	free(buffer); 

	return textureId;


}
