#include "A2.hpp"
#include "cs488-framework/GlErrorCheck.hpp"

#include <iostream>
using namespace std;

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/io.hpp>
using namespace glm;


vector<float> p_buffer; //store the eight points
mat4 Mats;
vec3 carm_pos,camNom;


//----------------------------------------------------------------------------------------
// Constructor
VertexData::VertexData()
	: numVertices(0),
	  index(0)
{
	positions.resize(kMaxVertices);
	colours.resize(kMaxVertices);
}


//----------------------------------------------------------------------------------------
// Constructor
A2::A2()
	: m_currentLineColour(vec3(0.0f))
{

}

//----------------------------------------------------------------------------------------
// Destructor
A2::~A2()
{

}

//----------------------------------------------------------------------------------------
/*
 * Called once, at program start.
 */
void A2::init()
{
	// Set the background colour.
	glClearColor(0.3, 0.5, 0.7, 1.0);

	createShaderProgram();

	glGenVertexArrays(1, &m_vao);

	enableVertexAttribIndices();

	generateVertexBuffers();

	mapVboDataToVertexAttributeLocation();


// The cube points
        p_buffer.push_back(-1.0f); //up
        p_buffer.push_back(1.0f);
        p_buffer.push_back(-1.0f);

        p_buffer.push_back(1.0f);
        p_buffer.push_back(1.0f);
        p_buffer.push_back(-1.0f);

        p_buffer.push_back(1.0f);
        p_buffer.push_back(1.0f);
        p_buffer.push_back(1.0f);

        p_buffer.push_back(-1.0f);
        p_buffer.push_back(1.0f);
        p_buffer.push_back(1.0f);

        p_buffer.push_back(-1.0f);  //down
        p_buffer.push_back(-1.0f);
        p_buffer.push_back(-1.0f);

        p_buffer.push_back(1.0f);
        p_buffer.push_back(-1.0f);
        p_buffer.push_back(-1.0f);

        p_buffer.push_back(1.0f);
        p_buffer.push_back(-1.0f);
        p_buffer.push_back(1.0f);

        p_buffer.push_back(-1.0f);
        p_buffer.push_back(-1.0f);
        p_buffer.push_back(1.0f);

// the modelling coordinates

        p_buffer.push_back(0.0f);
        p_buffer.push_back(0.0f);
        p_buffer.push_back(0.0f);

        p_buffer.push_back(10.0f);
        p_buffer.push_back(0.0f);
        p_buffer.push_back(0.0f);

        p_buffer.push_back(0.0f);
        p_buffer.push_back(10.0f);
        p_buffer.push_back(0.0f);

        p_buffer.push_back(0.0f);
        p_buffer.push_back(0.0f);
        p_buffer.push_back(1.0f);

// the modelling coordinates

        p_buffer.push_back(0.0f);
        p_buffer.push_back(0.0f);
        p_buffer.push_back(0.0f);

        p_buffer.push_back(10.0f);
        p_buffer.push_back(0.0f);
        p_buffer.push_back(0.0f);

        p_buffer.push_back(0.0f);
        p_buffer.push_back(10.0f);
        p_buffer.push_back(0.0f);

        p_buffer.push_back(0.0f);
        p_buffer.push_back(0.0f);
        p_buffer.push_back(1.0f);





       vport_w = 6;
       vport_h = 5;
       near = 1.5;
       far = 9;
       right = 2;
       top = 2;
       vtran_x = 0.0f;
       vtran_y = 0.0f;
       vtran_z = 3.0f;

       tran_x = -1.0f;
       tran_y = 0.3f;
       tran_z = 0.0f;

       rot_x = 0.0f;
       rot_y = 0.0f;
       rot_z = 0.0f;

       vrot_x = 0.0f;
       vrot_y = 0.0f;
       vrot_z = 0.0f;

       carm_pos = vec3(0.0f,0.0f,0.0f);
       camNom = vec3(0.0f,1.0f,0.0f);

}



//----------------------------------------------------------------------------------------

mat4 trans(float x,float y,float z){
     return mat4(vec4(1,0,0,0),
                 vec4(0,1,0,0),
                 vec4(0,0,1,0),
                 vec4(x,y,z,1));
}

mat4 sca(float x,float y,float z){
     return mat4(vec4(x,0,0,0),
                 vec4(0,y,0,0),
                 vec4(0,0,z,0),
                 vec4(0,0,0,1));
}


mat4 rotx(float sita){
     return mat4(vec4(1,0,0,0),
                 vec4(0,cos(sita),sin(sita),0),
                 vec4(0,-sin(sita),cos(sita),0),
                 vec4(0,0,0,1));
}

mat4 roty(float sita){
     return mat4(vec4(cos(sita),0,-sin(sita),0),
                 vec4(0,1,0,0),
                 vec4(sin(sita),0,cos(sita),0),
                 vec4(0,0,0,1));
}
mat4 rotz(float sita){
     return mat4(vec4(cos(sita),sin(sita),0,0),
                 vec4(-sin(sita),cos(sita),0,0),
                 vec4(0,0,1,0),
                 vec4(0,0,0,1));
}

//suppose I did not record the rotation of carmera
//
mat4  viewMat(vec3 eye, vec3 point, vec3 camNom){
   vec3 f = normalize(eye-point);
   vec3 s = normalize(cross(camNom,f));
   vec3 u = normalize(cross(f,s));
   return mat4(vec4(s[0],u[0],f[0],0.0f),
               vec4(s[1],u[1],f[1],0.0f),
               vec4(s[2],u[2],f[2],0.0f),
               vec4(-dot(s,eye),-dot(u,eye),-dot(f,eye),1.0f));

}

//generate the camera position
vec3 CamPos(float vrot_x,float vrot_y){
     float r = 5;
     float x,y,z;
    // x = r * sin(vrot_y) * cos(vrot_x);
    // y = r * sin(vrot_y) * sin(vrot_x);
    // z = r * cos(vrot_y);
     y = r * sin(vrot_x);
     x = r * cos(vrot_x) * cos(vrot_y);
     z = r * cos(vrot_x) * sin(vrot_y);
     return vec3(x,y,z); 
}


//----------------------------------------------------------------------------------------
void A2::createShaderProgram()
{
	m_shader.generateProgramObject();
	m_shader.attachVertexShader( getAssetFilePath("VertexShader.vs").c_str() );
	m_shader.attachFragmentShader( getAssetFilePath("FragmentShader.fs").c_str() );
	m_shader.link();
}

//----------------------------------------------------------------------------------------
void A2::enableVertexAttribIndices()
{
	glBindVertexArray(m_vao);

	// Enable the attribute index location for "position" when rendering.
	GLint positionAttribLocation = m_shader.getAttribLocation( "position" );
	glEnableVertexAttribArray(positionAttribLocation);

	// Enable the attribute index location for "colour" when rendering.
	GLint colourAttribLocation = m_shader.getAttribLocation( "colour" );
	glEnableVertexAttribArray(colourAttribLocation);

	// Restore defaults
	glBindVertexArray(0);

	CHECK_GL_ERRORS;
}

//----------------------------------------------------------------------------------------
void A2::generateVertexBuffers()
{
	// Generate a vertex buffer to store line vertex positions
	{
		glGenBuffers(1, &m_vbo_positions);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo_positions);

		// Set to GL_DYNAMIC_DRAW because the data store will be modified frequently.
		glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * kMaxVertices, nullptr,
				GL_DYNAMIC_DRAW);


		// Unbind the target GL_ARRAY_BUFFER, now that we are finished using it.
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		CHECK_GL_ERRORS;
	}

	// Generate a vertex buffer to store line colors
	{
		glGenBuffers(1, &m_vbo_colours);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo_colours);

		// Set to GL_DYNAMIC_DRAW because the data store will be modified frequently.
		glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * kMaxVertices, nullptr,
				GL_DYNAMIC_DRAW);


		// Unbind the target GL_ARRAY_BUFFER, now that we are finished using it.
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		CHECK_GL_ERRORS;
	}
}

//----------------------------------------------------------------------------------------
void A2::mapVboDataToVertexAttributeLocation()
{
	// Bind VAO in order to record the data mapping.
	glBindVertexArray(m_vao);

	// Tell GL how to map data from the vertex buffer "m_vbo_positions" into the
	// "position" vertex attribute index for any bound shader program.
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_positions);
	GLint positionAttribLocation = m_shader.getAttribLocation( "position" );
	glVertexAttribPointer(positionAttribLocation, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

	// Tell GL how to map data from the vertex buffer "m_vbo_colours" into the
	// "colour" vertex attribute index for any bound shader program.
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_colours);
	GLint colorAttribLocation = m_shader.getAttribLocation( "colour" );
	glVertexAttribPointer(colorAttribLocation, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	//-- Unbind target, and restore default values:
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	CHECK_GL_ERRORS;
}

//---------------------------------------------------------------------------------------
void A2::initLineData()
{
	m_vertexData.numVertices = 0;
	m_vertexData.index = 0;
}

//---------------------------------------------------------------------------------------
void A2::setLineColour (
		const glm::vec3 & colour
) {
	m_currentLineColour = colour;
}

//---------------------------------------------------------------------------------------
void A2::drawLine(
		const glm::vec2 & v0,   // Line Start (NDC coordinate)
		const glm::vec2 & v1    // Line End (NDC coordinate)
) {

	m_vertexData.positions[m_vertexData.index] = v0;
	m_vertexData.colours[m_vertexData.index] = m_currentLineColour;
	++m_vertexData.index;
	m_vertexData.positions[m_vertexData.index] = v1;
	m_vertexData.colours[m_vertexData.index] = m_currentLineColour;
	++m_vertexData.index;

	m_vertexData.numVertices += 2;
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, before guiLogic().
 */
void A2::appLogic()
{
	// Place per frame, application logic here ...


        float aspect = vport_w/vport_h;
       // Mats = mat4(vec4(near/right,0,0,0),vec4(0,near/top,0,0)
       //              ,vec4(0,0,-(far+near)/far-near,-2*far*near/(far-near)),vec4(0,0,-1,0));
        Mats = mat4(vec4(1/tan(3.14/8)/aspect,0,0,0),vec4(0,1/tan(3.14/8),0,0)
                     ,vec4(0,0,-(far+near)/(far-near),-1),vec4(0,0,-2*far*near/(far-near),0));



        vec4 tempvec;
        vector<vec4> v4;
        for(int i=0;i<p_buffer.size();i+=3){
          tempvec = vec4(p_buffer[i],p_buffer[i+1],p_buffer[i+2],1);
          //rotation
          if( i<12*3 || i>15*3 ){  //12,13,14,15 world frame
            tempvec = rotx(rot_x)* tempvec;
            tempvec = roty(rot_y)* tempvec;
            tempvec = rotz(rot_z)* tempvec;
          }

          //scale and trans to the front of camera
          tempvec =sca(0.5,0.5,0.5) * tempvec;
          //transportation cube
          if( i<12*3 || i>15*3 ){  //12,13,14,15 world frame
             tempvec = trans(tran_x,tran_y,tran_z)*tempvec;
          }
    
           vec4 newcam = rotx(vrot_x)*
                         roty(vrot_y)*
                         rotz(vrot_z)*
                         trans(vtran_x,vtran_y,vtran_z)*
                         vec4(carm_pos,1.0f);
          //camera view trans
          tempvec = viewMat(CamPos(vrot_x,vrot_y), vec3(0.0f,0.0f,-1.0f), camNom)*tempvec;

	  //projection
          tempvec =  Mats * tempvec;
          v4.push_back(tempvec);
        }//end for i

        //normalization
        for(int j=0;j<v4.size();++j){
            float w = v4[j][3];
            v4[j][0] = v4[j][0]/w;
            v4[j][1] = v4[j][1]/w;
            v4[j][2] = v4[j][2]/w;


        }//end for j

        initLineData();

        /*draw cubes*/
        setLineColour(vec3(1.0f, 0.7f, 0.8f));

        drawLine(vec2(v4[0]), vec2(v4[1]));
        drawLine(vec2(v4[1]), vec2(v4[2]));
        drawLine(vec2(v4[2]), vec2(v4[3]));
        drawLine(vec2(v4[3]), vec2(v4[0]));

        drawLine(vec2(v4[0]), vec2(v4[4]));
        drawLine(vec2(v4[1]), vec2(v4[5]));
        drawLine(vec2(v4[2]), vec2(v4[6]));
        drawLine(vec2(v4[3]), vec2(v4[7]));

        drawLine(vec2(v4[4]), vec2(v4[5]));
        drawLine(vec2(v4[5]), vec2(v4[6]));
        drawLine(vec2(v4[6]), vec2(v4[7]));
        drawLine(vec2(v4[7]), vec2(v4[4]));


        /*draw modelling coordinates*/
        setLineColour(vec3(1.0f, 0.0f, 0.0f));
        drawLine(vec2(v4[8]), vec2(v4[9]));
        setLineColour(vec3(0.0f, 1.0f, 0.0f));
        drawLine(vec2(v4[8]), vec2(v4[10]));
        setLineColour(vec3(0.0f, 0.0f, 0.1f));
        drawLine(vec2(v4[8]), vec2(v4[11]));


        /*draw world coordinates*/
        setLineColour(vec3(0.6f, 0.0f, 0.0f));
        drawLine(vec2(v4[12]), vec2(v4[13]));
        setLineColour(vec3(0.0f, 0.6f, 0.0f));
        drawLine(vec2(v4[12]), vec2(v4[14]));
        setLineColour(vec3(0.0f, 0.0f, 0.6f));
        drawLine(vec2(v4[12]), vec2(v4[15]));



	// Call at the beginning of frame, before drawing lines:
//	initLineData();
/*
	// Draw outer square:
	setLineColour(vec3(1.0f, 0.7f, 0.8f));
	drawLine(vec2(-0.5f, -0.5f), vec2(0.5f, -0.5f));
	drawLine(vec2(0.5f, -0.5f), vec2(0.5f, 0.5f));
	drawLine(vec2(0.5f, 0.5f), vec2(-0.5f, 0.5f));
	drawLine(vec2(-0.5f, 0.5f), vec2(-0.5f, -0.5f));


	// Draw inner square:
	setLineColour(vec3(0.2f, 1.0f, 1.0f));
	drawLine(vec2(-0.25f, -0.25f), vec2(0.25f, -0.25f));
	drawLine(vec2(0.25f, -0.25f), vec2(0.25f, 0.25f));
	drawLine(vec2(0.25f, 0.25f), vec2(-0.25f, 0.25f));
	drawLine(vec2(-0.25f, 0.25f), vec2(-0.25f, -0.25f));
*/
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, after appLogic(), but before the draw() method.
 */
void A2::guiLogic()
{
	static bool firstRun(true);
	if (firstRun) {
		ImGui::SetNextWindowPos(ImVec2(50, 50));
		firstRun = false;
	}

	static bool showDebugWindow(true);
	ImGuiWindowFlags windowFlags(ImGuiWindowFlags_AlwaysAutoResize);
	float opacity(0.5f);
  vec3 posit = CamPos(vrot_x,vrot_y);
	ImGui::Begin("Properties", &showDebugWindow, ImVec2(100,100), opacity,
			windowFlags);


		// Add more gui elements here here ...
                  
                ImGui::SliderFloat("x Channel", &vrot_x, 0.0f, 6.0f);
                ImGui::SliderFloat("y Channel", &vrot_y, 0.0f, 6.0f);

		// Create Button, and check if it was clicked:
		if( ImGui::Button( "Quit Application" ) ) {
			glfwSetWindowShouldClose(m_window, GL_TRUE);
		}

		ImGui::Text( "Framerate: %.1f FPS", ImGui::GetIO().Framerate );
         
ImGui::Text( "x: %.2f, %.2f,%.2f", posit.x,posit.y,posit.z );


	ImGui::End();
}

//----------------------------------------------------------------------------------------
void A2::uploadVertexDataToVbos() {

	//-- Copy vertex position data into VBO, m_vbo_positions:
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo_positions);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec2) * m_vertexData.numVertices,
				m_vertexData.positions.data());
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		CHECK_GL_ERRORS;
	}

	//-- Copy vertex colour data into VBO, m_vbo_colours:
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo_colours);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec3) * m_vertexData.numVertices,
				m_vertexData.colours.data());
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		CHECK_GL_ERRORS;
	}
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, after guiLogic().
 */
void A2::draw()
{
	uploadVertexDataToVbos();

	glBindVertexArray(m_vao);

	m_shader.enable();
		glDrawArrays(GL_LINES, 0, m_vertexData.numVertices);
	m_shader.disable();

	// Restore defaults
	glBindVertexArray(0);

	CHECK_GL_ERRORS;
}

//----------------------------------------------------------------------------------------
/*
 * Called once, after program is signaled to terminate.
 */
void A2::cleanup()
{

}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles cursor entering the window area events.
 */
bool A2::cursorEnterWindowEvent (
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
bool A2::mouseMoveEvent (
		double xPos,
		double yPos
) {
	bool eventHandled(false);

	// Fill in with event handling code...

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse button events.
 */
bool A2::mouseButtonInputEvent (
		int button,
		int actions,
		int mods
) {
	bool eventHandled(false);

	// Fill in with event handling code...

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse scroll wheel events.
 */
bool A2::mouseScrollEvent (
		double xOffSet,
		double yOffSet
) {
	bool eventHandled(false);

	// Fill in with event handling code...

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles window resize events.
 */
bool A2::windowResizeEvent (
		int width,
		int height
) {
	bool eventHandled(false);

	// Fill in with event handling code...

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles key input events.
 */
bool A2::keyInputEvent (
		int key,
		int action,
		int mods
) {
	bool eventHandled(false);

	// Fill in with event handling code...

	return eventHandled;
}
