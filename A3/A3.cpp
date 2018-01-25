#include "A3.hpp"
#include "scene_lua.hpp"
using namespace std;

#include "cs488-framework/GlErrorCheck.hpp"
#include "cs488-framework/MathUtils.hpp"
#include "GeometryNode.hpp"
#include "JointNode.hpp"

#include <imgui/imgui.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/io.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

using namespace glm;

static bool show_gui = true;

const size_t CIRCLE_PTS = 48;

int id = 0; //used to konw which component drawing
bool do_picking;
bool selected[100];
bool IsRoot = true;
double offset_x = 0.0;
double offset_y = 0.0;
bool JustRelease = false;
//----------------------------------------------------------------------------------------
// Constructor
A3::A3(const std::string & luaSceneFile)
	: m_luaSceneFile(luaSceneFile),
	  m_positionAttribLocation(0),
	  m_normalAttribLocation(0),
	  m_vao_meshData(0),
	  m_vbo_vertexPositions(0),
	  m_vbo_vertexNormals(0),
	  m_vao_arcCircle(0),
	  m_vbo_arcCircle(0)
{

}

//----------------------------------------------------------------------------------------
// Destructor
A3::~A3()
{

}

//----------------------------------------------------------------------------------------
/*
 * Called once, at program start.
 */
void A3::init()
{
	// Set the background colour.
	glClearColor(0.35, 0.35, 0.35, 1.0);

	createShaderProgram();

	glGenVertexArrays(1, &m_vao_arcCircle);
	glGenVertexArrays(1, &m_vao_meshData);
	enableVertexShaderInputSlots();

	processLuaSceneFile(m_luaSceneFile);

	// Load and decode all .obj files at once here.  You may add additional .obj files to
	// this list in order to support rendering additional mesh types.  All vertex
	// positions, and normals will be extracted and stored within the MeshConsolidator
	// class.
	unique_ptr<MeshConsolidator> meshConsolidator (new MeshConsolidator{
			getAssetFilePath("cube.obj"),
			getAssetFilePath("sphere.obj"),
			getAssetFilePath("suzanne.obj"),
                        getAssetFilePath("head.obj"),
                        getAssetFilePath("neck.obj"),
                        getAssetFilePath("tor.obj"),
                        getAssetFilePath("leftshoder.obj"),
                        getAssetFilePath("leftarm.obj"),
                        getAssetFilePath("lefthand.obj"),
                        getAssetFilePath("rightshoulder.obj"),
                        getAssetFilePath("rightarm.obj"),
                        getAssetFilePath("righthand.obj"),
                        getAssetFilePath("leftbigleg.obj"),
                        getAssetFilePath("leftsmalllge.obj"),
                        getAssetFilePath("leftshoe.obj"),
                        getAssetFilePath("rightbigleg.obj"),
                        getAssetFilePath("rightsmalllge.obj"),
                        getAssetFilePath("rightshoe.obj"),

	});


	// Acquire the BatchInfoMap from the MeshConsolidator.
	meshConsolidator->getBatchInfoMap(m_batchInfoMap);

	// Take all vertex data within the MeshConsolidator and upload it to VBOs on the GPU.
	uploadVertexDataToVbos(*meshConsolidator);

	mapVboDataToVertexShaderInputLocations();

	initPerspectiveMatrix();

	initViewMatrix();

	initLightSources();


	// Exiting the current scope calls delete automatically on meshConsolidator freeing
	// all vertex data resources.  This is fine since we already copied this data to
	// VBOs on the GPU.  We have no use for storing vertex data on the CPU side beyond
	// this point.

        do_picking = false;
        for(int i=0;i<100;i++){
           selected[i] = false;
        }//end fori 
        reset();
}

void A3::reset(){


     RotMat = mat4(1.0f,0.0f,0.0f,0.0f,
                   0.0f,1.0f,0.0f,0.0f,
                   0.0f,0.0f,1.0f,0.0f,
                   0.0f,0.0f,0.0f,1.0f);
     nRotMat = mat4(1.0f,0.0f,0.0f,0.0f,
                   0.0f,1.0f,0.0f,0.0f,
                   0.0f,0.0f,1.0f,0.0f,
                   0.0f,0.0f,0.0f,1.0f);
     ObjRotMat = mat4(1.0f,0.0f,0.0f,0.0f,
                   0.0f,1.0f,0.0f,0.0f,
                   0.0f,0.0f,1.0f,0.0f,
                   0.0f,0.0f,0.0f,1.0f);
     nObjRotMat = mat4(1.0f,0.0f,0.0f,0.0f,
                   0.0f,1.0f,0.0f,0.0f,
                   0.0f,0.0f,1.0f,0.0f,
                   0.0f,0.0f,0.0f,1.0f);


     mouse_x = 0.0;
     mouse_y = 0.0;
     offset_x =0.0;
     offset_y =0.0;


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



//----------------------------------------------------------------------------------------
void A3::processLuaSceneFile(const std::string & filename) {
	// This version of the code treats the Lua file as an Asset,
	// so that you'd launch the program with just the filename
	// of a puppet in the Assets/ directory.
	// std::string assetFilePath = getAssetFilePath(filename.c_str());
	// m_rootNode = std::shared_ptr<SceneNode>(import_lua(assetFilePath));

	// This version of the code treats the main program argument
	// as a straightforward pathname.
	m_rootNode = std::shared_ptr<SceneNode>(import_lua(filename));
	if (!m_rootNode) {
		std::cerr << "Could not open " << filename << std::endl;
	}
}

//----------------------------------------------------------------------------------------
void A3::createShaderProgram()
{
	m_shader.generateProgramObject();
	m_shader.attachVertexShader( getAssetFilePath("VertexShader.vs").c_str() );
	m_shader.attachFragmentShader( getAssetFilePath("FragmentShader.fs").c_str() );
	m_shader.link();

	m_shader_arcCircle.generateProgramObject();
	m_shader_arcCircle.attachVertexShader( getAssetFilePath("arc_VertexShader.vs").c_str() );
	m_shader_arcCircle.attachFragmentShader( getAssetFilePath("arc_FragmentShader.fs").c_str() );
	m_shader_arcCircle.link();
}

//----------------------------------------------------------------------------------------
void A3::enableVertexShaderInputSlots()
{
	//-- Enable input slots for m_vao_meshData:
	{
		glBindVertexArray(m_vao_meshData);

		// Enable the vertex shader attribute location for "position" when rendering.
		m_positionAttribLocation = m_shader.getAttribLocation("position");
		glEnableVertexAttribArray(m_positionAttribLocation);

		// Enable the vertex shader attribute location for "normal" when rendering.
		m_normalAttribLocation = m_shader.getAttribLocation("normal");
		glEnableVertexAttribArray(m_normalAttribLocation);

                // Enable the vertex shader attribut location for textCoord when rendering.
//                glEnableVertexAttribArray(2);


		CHECK_GL_ERRORS;
	}


	//-- Enable input slots for m_vao_arcCircle:
	{
		glBindVertexArray(m_vao_arcCircle);

		// Enable the vertex shader attribute location for "position" when rendering.
		m_arc_positionAttribLocation = m_shader_arcCircle.getAttribLocation("position");
		glEnableVertexAttribArray(m_arc_positionAttribLocation);

		CHECK_GL_ERRORS;
	}

	// Restore defaults
	glBindVertexArray(0);
}

//----------------------------------------------------------------------------------------
void A3::uploadVertexDataToVbos (
		const MeshConsolidator & meshConsolidator
) {
	// Generate VBO to store all vertex position data
	{
		glGenBuffers(1, &m_vbo_vertexPositions);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo_vertexPositions);

		glBufferData(GL_ARRAY_BUFFER, meshConsolidator.getNumVertexPositionBytes(),
				meshConsolidator.getVertexPositionDataPtr(), GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		CHECK_GL_ERRORS;
	}

	// Generate VBO to store all vertex normal data
	{
		glGenBuffers(1, &m_vbo_vertexNormals);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo_vertexNormals);

		glBufferData(GL_ARRAY_BUFFER, meshConsolidator.getNumVertexNormalBytes(),
				meshConsolidator.getVertexNormalDataPtr(), GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		CHECK_GL_ERRORS;
	}

	// Generate VBO to store the trackball circle.
	{
		glGenBuffers( 1, &m_vbo_arcCircle );
		glBindBuffer( GL_ARRAY_BUFFER, m_vbo_arcCircle );

		float *pts = new float[ 2 * CIRCLE_PTS ];
		for( size_t idx = 0; idx < CIRCLE_PTS; ++idx ) {
			float ang = 2.0 * M_PI * float(idx) / CIRCLE_PTS;
			pts[2*idx] = cos( ang );
			pts[2*idx+1] = sin( ang );
		}

		glBufferData(GL_ARRAY_BUFFER, 2*CIRCLE_PTS*sizeof(float), pts, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		CHECK_GL_ERRORS;
	}
}

//----------------------------------------------------------------------------------------
void A3::mapVboDataToVertexShaderInputLocations()
{
	// Bind VAO in order to record the data mapping.
	glBindVertexArray(m_vao_meshData);

	// Tell GL how to map data from the vertex buffer "m_vbo_vertexPositions" into the
	// "position" vertex attribute location for any bound vertex shader program.
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_vertexPositions);
	glVertexAttribPointer(m_positionAttribLocation, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	// Tell GL how to map data from the vertex buffer "m_vbo_vertexNormals" into the
	// "normal" vertex attribute location for any bound vertex shader program.
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_vertexNormals);
	glVertexAttribPointer(m_normalAttribLocation, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	//-- Unbind target, and restore default values:
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	CHECK_GL_ERRORS;

	// Bind VAO in order to record the data mapping.
	glBindVertexArray(m_vao_arcCircle);

	// Tell GL how to map data from the vertex buffer "m_vbo_arcCircle" into the
	// "position" vertex attribute location for any bound vertex shader program.
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_arcCircle);
	glVertexAttribPointer(m_arc_positionAttribLocation, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

	//-- Unbind target, and restore default values:
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	CHECK_GL_ERRORS;
}

//----------------------------------------------------------------------------------------
void A3::initPerspectiveMatrix()
{
	float aspect = ((float)m_windowWidth) / m_windowHeight;
	m_perpsective = glm::perspective(degreesToRadians(60.0f), aspect, 0.1f, 100.0f);
}


//----------------------------------------------------------------------------------------
void A3::initViewMatrix() {
	m_view = glm::lookAt(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f),
			vec3(0.0f, 1.0f, 0.0f));
}

//----------------------------------------------------------------------------------------
void A3::initLightSources() {
	// World-space position
	m_light.position = vec3(-2.0f, 5.0f, 0.5f);
	m_light.rgbIntensity = vec3(0.8f); // White light
}

//----------------------------------------------------------------------------------------
void A3::uploadCommonSceneUniforms() {
	m_shader.enable();
	{
		//-- Set Perpsective matrix uniform for the scene:
		GLint location = m_shader.getUniformLocation("Perspective");
		glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(m_perpsective));
		CHECK_GL_ERRORS;


                //if picking
                location = m_shader.getUniformLocation("picking");
                glUniform1i( location, do_picking ? 1 : 0);

		//-- Set LightSource uniform for the scene:
		if(!do_picking){
			location = m_shader.getUniformLocation("light.position");
			glUniform3fv(location, 1, value_ptr(m_light.position));
			location = m_shader.getUniformLocation("light.rgbIntensity");
			glUniform3fv(location, 1, value_ptr(m_light.rgbIntensity));
			CHECK_GL_ERRORS;
		}

		//-- Set background light ambient intensity
		if(!do_picking){
			location = m_shader.getUniformLocation("ambientIntensity");
			vec3 ambientIntensity(0.05f);
			glUniform3fv(location, 1, value_ptr(ambientIntensity));
			CHECK_GL_ERRORS;
		}
	}
	m_shader.disable();
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, before guiLogic().
 */
void A3::appLogic()
{
	// Place per frame, application logic here ...

	uploadCommonSceneUniforms();
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, after appLogic(), but before the draw() method.
 */
void A3::guiLogic()
{
	if( !show_gui ) {
		return;
	}

	static bool firstRun(true);
	if (firstRun) {
		ImGui::SetNextWindowPos(ImVec2(50, 50));
		firstRun = false;
	}

	static bool showDebugWindow(true);
	ImGuiWindowFlags windowFlags(ImGuiWindowFlags_AlwaysAutoResize);
	float opacity(0.5f);

	ImGui::Begin("Properties", &showDebugWindow, ImVec2(100,100), opacity,
			windowFlags);


		// Add more gui elements here here ...


		// Create Button, and check if it was clicked:
		if( ImGui::Button( "Quit Application" ) ) {
			glfwSetWindowShouldClose(m_window, GL_TRUE);
		}

		ImGui::Text( "Framerate: %.1f FPS", ImGui::GetIO().Framerate );

	ImGui::End();
}

//----------------------------------------------------------------------------------------
// Update mesh specific shader uniforms:
static void updateShaderUniforms(
		const ShaderProgram & shader,
		const GeometryNode & node,
		const glm::mat4 & viewMatrix,
                const glm::mat4 & RotMatrix
) {
        glm::vec3 col;

        if(selected[id]){
             col = glm::vec3(1.0,1.0,0.0);    //change selected color
        }//end if selec

	shader.enable();
	{
		//-- Set ModelView matrix:
		GLint location = shader.getUniformLocation("ModelView");
                

                mat4 modelView =  viewMatrix  *  node.father_trans *node.trans;
  //              if(IsRoot){
  //                   modelView = viewMatrix * node.father_trans *node.trans * RotMatrix;
 //                    IsRoot = false;
 //               }

//		mat4 modelView = viewMatrix * node.trans;
		glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(modelView));
		CHECK_GL_ERRORS;


             if(do_picking) {

                float r = float(id&0xff) / 255.0f;
                float g = float((id>>8)&0xff) / 255.0f;
                float b = float((id>>16)&0xff) / 255.0f;

                location = shader.getUniformLocation("material.kd");
                glUniform3f(location, r,g,b);
                CHECK_GL_ERRORS;

             }else{
		//-- Set NormMatrix:

		location = shader.getUniformLocation("NormalMatrix");
		mat3 normalMatrix = glm::transpose(glm::inverse(mat3(modelView)));
		glUniformMatrix3fv(location, 1, GL_FALSE, value_ptr(normalMatrix));
		CHECK_GL_ERRORS;


		//-- Set Material values:
		location = shader.getUniformLocation("material.kd");
		vec3 kd = node.material.kd;
                if(selected[id]){
                    kd = vec3(0.5,0.5,0.2);    //change the color of selected
                }
		glUniform3fv(location, 1, value_ptr(kd));
		CHECK_GL_ERRORS;
		location = shader.getUniformLocation("material.ks");
		vec3 ks = node.material.ks;
		glUniform3fv(location, 1, value_ptr(ks));
		CHECK_GL_ERRORS;
		location = shader.getUniformLocation("material.shininess");
		glUniform1f(location, node.material.shininess);
		CHECK_GL_ERRORS;
            }//end do_picking

	}
	shader.disable();

}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, after guiLogic().
 */
void A3::draw() {

	glEnable( GL_DEPTH_TEST );
	renderSceneGraph(*m_rootNode);


	glDisable( GL_DEPTH_TEST );
	renderArcCircle();
}

//----------------------------------------------------------------------------------------
void A3::renderSceneGraph(SceneNode & root) {

	// Bind the VAO once here, and reuse for all GeometryNode rendering below.
	glBindVertexArray(m_vao_meshData);

	// This is emphatically *not* how you should be drawing the scene graph in
	// your final implementation.  This is a non-hierarchical demonstration
	// in which we assume that there is a list of GeometryNodes living directly
	// underneath the root node, and that we can draw them in a loop.  It's
	// just enough to demonstrate how to get geometry and materials out of
	// a GeometryNode and onto the screen.

	// You'll want to turn this into recursive code that walks over the tree.
	// You can do that by putting a method in SceneNode, overridden in its
	// subclasses, that renders the subtree rooted at every node.  Or you
	// could put a set of mutually recursive functions in this class, which
	// walk down the tree from nodes of different types.


        mat4 father_tans = mat4(1,0,0,0,
                                0,1,0,0,
                                0,0,1,0,
                                0,0,0,1);
        SceneNode * st_ptr;
        vector<SceneNode *> stack;
        stack.push_back(&root);
        IsRoot = true;
     while(!stack.empty()){
        st_ptr = stack.back();
        stack.pop_back();


                
               //rotate the root 
                if(IsRoot){
                  father_tans = st_ptr->father_trans * st_ptr->get_transform()*nObjRotMat;
                  IsRoot = false;
                }else{//end isroot
                  father_tans = st_ptr->father_trans * st_ptr->get_transform();
                }

                

                //Joint
                if(st_ptr->m_nodeType == NodeType::JointNode){
                     JointNode * Jp = (JointNode*)st_ptr;
//                     if(selected[id]){
//                        Jp->cur_y += 0.1;
//                        cout <<"y="<<Jp->cur_y<<endl;}
                     float angx = Jp->cur_x;
                     float angy = Jp->cur_y;

                     if(selected[id]){
                         angx += offset_x;
                         angy += offset_y;
//cout <<"angx is "<<angx<<endl;
//cout <<"angy is "<<angy<<endl;
                     }//end if sel

                     //check rotate range
                     if(angx > Jp->m_joint_x.max){
                         angx = Jp->m_joint_x.max;
                     }else if(angx < Jp->m_joint_x.min) {
                         angx = Jp->m_joint_x.min;
                     }//end if angx

                     if(angy > Jp->m_joint_y.max){
                         angy = Jp->m_joint_y.max;
                     }else if(angy < Jp->m_joint_y.min) {
                         angy = Jp->m_joint_y.min;
                     }//end if angy

                     angx = degreesToRadians(angx);
                     angy = degreesToRadians(angy);


   //                  mat4 rotmat_y = mat4(cos(ang),0,-sin(ang),0,
   //                                       0,1,0,0,
   //                                       sin(ang),0,cos(ang),0,
   //                                       0,0,0,1); 
                     father_tans = father_tans * roty(angy)* rotx(angx);
                     if(JustRelease&& selected[id]){
                        Jp->move_x(offset_x);     //set cur to cur+offset
                        Jp->move_y(offset_y);
                     }//not m
                }//end if st



        //add children
        for(SceneNode * ch : st_ptr->children){
              
             ch->father_trans = father_tans; 
             stack.push_back(ch);
        }//end for ch



		if (st_ptr->m_nodeType != NodeType::GeometryNode)
			continue;

                

//if(st_ptr->m_name == "br-gauche" || st_ptr->m_name == "ep-gauche") cout << st_ptr->father_trans * st_ptr->trans<<endl;
//cout<<"drawing "<<  st_ptr->m_name<<endl;
		const GeometryNode * geometryNode = static_cast<const GeometryNode *>(st_ptr);

                
		updateShaderUniforms(m_shader, *geometryNode, m_view, nObjRotMat);


		// Get the BatchInfo corresponding to the GeometryNode's unique MeshId.
		BatchInfo batchInfo = m_batchInfoMap[geometryNode->meshId];

		//-- Now render the mesh:
		m_shader.enable();
		glDrawArrays(GL_TRIANGLES, batchInfo.startIndex, batchInfo.numIndices);
		m_shader.disable();
	       
                ++id; // increment id for next Geo component 
                if(id>= 100){cout << "more than 100 Geo, explode!"<<endl;}
     }//end while emp

        id = 0; //reset current drawing id

        if(JustRelease){
          offset_x = 0.0;
          offset_y = 0.0;
          JustRelease = false; //first loop after released mouse finished
        }


	glBindVertexArray(0);
	CHECK_GL_ERRORS;
}



/*
        for (const SceneNode * node : root.children) {

                if (node->m_nodeType != NodeType::GeometryNode)
                        continue;

                const GeometryNode * geometryNode = static_cast<const GeometryNode *>(node);

                updateShaderUniforms(m_shader, *geometryNode, m_view);


                // Get the BatchInfo corresponding to the GeometryNode's unique MeshId.
                BatchInfo batchInfo = m_batchInfoMap[geometryNode->meshId];

                //-- Now render the mesh:
                m_shader.enable();
                glDrawArrays(GL_TRIANGLES, batchInfo.startIndex, batchInfo.numIndices);
                m_shader.disable();
        }

*/




//----------------------------------------------------------------------------------------
// Draw the trackball circle.
void A3::renderArcCircle() {
	glBindVertexArray(m_vao_arcCircle);

	m_shader_arcCircle.enable();
		GLint m_location = m_shader_arcCircle.getUniformLocation( "M" );
		float aspect = float(m_framebufferWidth)/float(m_framebufferHeight);
		glm::mat4 M;
		if( aspect > 1.0 ) {
			M = glm::scale( glm::mat4(), glm::vec3( 0.5/aspect, 0.5, 1.0 ) );
		} else {
			M = glm::scale( glm::mat4(), glm::vec3( 0.5, 0.5*aspect, 1.0 ) );
		}
		glUniformMatrix4fv( m_location, 1, GL_FALSE, value_ptr( M ) );
		glDrawArrays( GL_LINE_LOOP, 0, CIRCLE_PTS );
	m_shader_arcCircle.disable();

	glBindVertexArray(0);
	CHECK_GL_ERRORS;
}

//----------------------------------------------------------------------------------------
/*
 * Called once, after program is signaled to terminate.
 */
void A3::cleanup()
{

}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles cursor entering the window area events.
 */
bool A3::cursorEnterWindowEvent (
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
bool A3::mouseMoveEvent (
		double xPos,
		double yPos
) {
	bool eventHandled(false);

	// Fill in with event handling code...


        if( GLFW_KEY_R == shortcut){   //model rotation
            if (m_mouseMidAct ) {
                 nObjRotMat = roty(0.1*(xPos - mouse_x)) * ObjRotMat;
            }else if(m_mouseRightAct){
                 nObjRotMat = rotz(0.1*(xPos - mouse_x)) * ObjRotMat;
            }else{
                 mouse_x = xPos;
            }

            if (m_mouseLeftAct) {
                 nObjRotMat = rotx(0.1*(yPos - mouse_y)) * ObjRotMat;
            }else{
                 mouse_y = yPos;
            }
        }else if( GLFW_KEY_J == shortcut){    //part rotation
              if (m_mouseLeftAct ) {
              }else{
//                 mouse_x = xPos;
              }

              if (m_mouseMidAct) {
                   offset_x = (mouse_x - xPos)/1;
cout <<"assigned offset x="<<offset_x<<endl;
                   offset_y = (mouse_y - yPos)/0.5;
              }else if(m_mouseRightAct){
              }else{
                 mouse_x = xPos;
                 mouse_y = yPos;
              }
            

        }//end if GLFW


	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse button events.
 */
bool A3::mouseButtonInputEvent (
		int button,
		int actions,
		int mods
) {
	bool eventHandled(false);

	// Fill in with event handling code...

        if(button == GLFW_MOUSE_BUTTON_LEFT && actions == GLFW_PRESS){

               double xpos,ypos;
               glfwGetCursorPos( m_window, &xpos,&ypos);

               do_picking = true;
               
               uploadCommonSceneUniforms();
               glClearColor(1.0,1.0,1.0,1.0);
               glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
               glClearColor(0.35,0.35,0.35,1.0);

               draw();

               CHECK_GL_ERRORS;

               xpos *= double(m_framebufferWidth) / double(m_windowWidth);
               ypos = m_windowHeight - ypos;
               ypos *= double(m_framebufferHeight) / double(m_windowHeight);

               GLubyte buffer[4] = { 0,0,0,0 };
               glReadBuffer( GL_BACK );
  
               glReadPixels( int(xpos), int(ypos), 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, buffer );
               CHECK_GL_ERRORS;

               // Reassemble the object ID.
               unsigned int what = buffer[0] + (buffer[1] << 8) + (buffer[2] << 16);

               cout << what <<endl;
               if( what < 100 ) {
                       selected[what] = !selected[what];
               }
               do_picking = false;             
               CHECK_GL_ERRORS;
        }//end if button
        if (actions == GLFW_PRESS) {
                if (!ImGui::IsMouseHoveringAnyWindow()) {
                     if( GLFW_MOUSE_BUTTON_LEFT == button){
                        m_mouseLeftAct = true;


                     }else if( GLFW_MOUSE_BUTTON_MIDDLE == button){
                        m_mouseMidAct = true;

                     }else if( GLFW_MOUSE_BUTTON_RIGHT == button){
                        m_mouseRightAct = true;

                     }

                }


        }//end if act

        if (actions == GLFW_RELEASE) {
                m_mouseLeftAct = false;
                m_mouseRightAct = false;
                m_mouseMidAct = false;
                RotMat = nRotMat;
                ObjRotMat = nObjRotMat;
                JustRelease = true;
cout <<"-------relsase--------"<<endl;
        }








	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse scroll wheel events.
 */
bool A3::mouseScrollEvent (
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
bool A3::windowResizeEvent (
		int width,
		int height
) {
	bool eventHandled(false);
	initPerspectiveMatrix();
	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles key input events.
 */
bool A3::keyInputEvent (
		int key,
		int action,
		int mods
) {
	bool eventHandled(false);

	if( action == GLFW_PRESS ) {
		if( key == GLFW_KEY_M ) {
			show_gui = !show_gui;
			eventHandled = true;
		}else{
                        shortcut = key;
                }

	}
	// Fill in with event handling code...

	return eventHandled;
}
