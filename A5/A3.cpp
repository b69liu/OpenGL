/*
Reference:
texture mapping: http://blog.csdn.net/wangdingqiaoit/article/details/51457675
skybox: http://blog.csdn.net/wangdingqiaoit/article/details/52506893
        http://www.custommapmakers.org/skyboxes.php
image loader: https://github.com/nothings/stb
Sound system: https://www.ambiera.com/irrklang/tutorial-helloworld.html
Lara's model:
body
https://3dwarehouse.sketchup.com/model/d416450afcdee32f5cc46acc8b45df43/Lara-Croft-Tomb-Raider-Legend-Evening-Dress
arms
https://3dwarehouse.sketchup.com/model/2f704dbee89a45f3f89e7b86262150f/Lara-Croft-Underworld
*/


#include "A3.hpp"
#include "scene_lua.hpp"
#include <math.h>
//#include "SOIL.h"
using namespace std;
#include "cs488-framework/GlErrorCheck.hpp"
#include "cs488-framework/MathUtils.hpp"
#include "GeometryNode.hpp"
#include "JointNode.hpp"

#include <imgui/imgui.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/io.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <irrKlang.h>
#include "common/conio.h"
using namespace irrklang;
#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll


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

	GLfloat skyboxVertices[] = {
		-1.0f, 1.0f, -1.0f,		// A
		-1.0f, -1.0f, -1.0f,	// B
		1.0f, -1.0f, -1.0f,		// C
		1.0f, -1.0f, -1.0f,		// C
		1.0f, 1.0f, -1.0f,		// D
		-1.0f, 1.0f, -1.0f,		// A

		-1.0f, -1.0f, 1.0f,		// E
		-1.0f, -1.0f, -1.0f,	// B
		-1.0f, 1.0f, -1.0f,		// A
		-1.0f, 1.0f, -1.0f,		// A
		-1.0f, 1.0f, 1.0f,		// F
		-1.0f, -1.0f, 1.0f,		// E

		1.0f, -1.0f, -1.0f,		// C
		1.0f, -1.0f, 1.0f,		// G
		1.0f, 1.0f, 1.0f,		// H
		1.0f, 1.0f, 1.0f,		// H
		1.0f, 1.0f, -1.0f,		// D
		1.0f, -1.0f, -1.0f,		// C

		-1.0f, -1.0f, 1.0f,  // E
		-1.0f, 1.0f, 1.0f,  // F
		1.0f, 1.0f, 1.0f,  // H
		1.0f, 1.0f, 1.0f,  // H
		1.0f, -1.0f, 1.0f,  // G
		-1.0f, -1.0f, 1.0f,  // E

		-1.0f, 1.0f, -1.0f,  // A
		1.0f, 1.0f, -1.0f,  // D
		1.0f, 1.0f, 1.0f,  // H
		1.0f, 1.0f, 1.0f,  // H
		-1.0f, 1.0f, 1.0f,  // F
		-1.0f, 1.0f, -1.0f,  // A

		-1.0f, -1.0f, -1.0f,  // B
		-1.0f, -1.0f, 1.0f,   // E
		1.0f, -1.0f, 1.0f,    // G
		1.0f, -1.0f, 1.0f,    // G
		1.0f, -1.0f, -1.0f,   // C
		-1.0f, -1.0f, -1.0f,  // B
	};

int BGM( const char * mpath){
           ISoundEngine* engine = createIrrKlangDevice();
        if (!engine)
        {
                printf("Could not startup engine\n");
                return 0; // error starting up the engine
        }
        do
        {
                // play a single sound
                engine->play2D("MMExploreEvening16.wav");
        }
        while(false);


        return 0;


}

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
	  m_vbo_arcCircle(0),
	  m_vao_skybox(0),
	  m_vbo_skybox(0),
	  m_vao_City(0),
	  m_vbo_cityPositions(0)
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
	glGenVertexArrays(1, &m_vao_skybox);
	glGenVertexArrays(1, &m_vao_City);
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
                        getAssetFilePath("sword.obj"),
						getAssetFilePath("sw1.obj"),
						getAssetFilePath("sw2.obj"),
						getAssetFilePath("sw3.obj"),
						getAssetFilePath("sw4.obj"),
						getAssetFilePath("sw5.obj")





	});

    reset();
	// Acquire the BatchInfoMap from the MeshConsolidator.
	meshConsolidator->getBatchInfoMap(m_batchInfoMap);

	// Take all vertex data within the MeshConsolidator and upload it to VBOs on the GPU.
	uploadVertexDataToVbos(*meshConsolidator);

	uploadCityDataToVbo();

	mapVboDataToVertexShaderInputLocations();

	initPerspectiveMatrix();

	initViewMatrix();

	initLightSources();

    //load skybox textures
	vector<const char*> faces;
	
	faces.push_back("skybox/stormydays_rt.tga");
	faces.push_back("skybox/stormydays_lf.tga");
	faces.push_back("skybox/stormydays_up.tga");
	faces.push_back("skybox/stormydays_dn.tga");
	faces.push_back("skybox/stormydays_bk.tga");
	faces.push_back("skybox/stormydays_ft.tga");

/*	
	faces.push_back("skybox/sky_rt.jpg");
	faces.push_back("skybox/sky_lf.jpg");
	faces.push_back("skybox/sky_up.jpg");
	faces.push_back("skybox/sky_dn.jpg");
	faces.push_back("skybox/sky_bk.jpg");
	faces.push_back("skybox/sky_ft.jpg");
*/
/*
	faces.push_back("skybox/posx.jpg");
	faces.push_back("skybox/negx.jpg");
	faces.push_back("skybox/posy.jpg");
	faces.push_back("skybox/negy.jpg");
	faces.push_back("skybox/posz.jpg");
	faces.push_back("skybox/negz.jpg");
*/
	skyboxTexture = LoadCubeMapTexture(faces);

    CityTexture = LoadTexture("skybox/brick_pavement.jpg");




	// Exiting the current scope calls delete automatically on meshConsolidator freeing
	// all vertex data resources.  This is fine since we already copied this data to
	// VBOs on the GPU.  We have no use for storing vertex data on the CPU side beyond
	// this point.

        do_picking = false;
        for(int i=0;i<100;i++){
           selected[i] = false;
        }//end fori 
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
     VRotMat = mat4(1.0f,0.0f,0.0f,0.0f,
                   0.0f,1.0f,0.0f,0.0f,
                   0.0f,0.0f,1.0f,0.0f,
                   0.0f,0.0f,0.0f,1.0f);
     nVRotMat = mat4(1.0f,0.0f,0.0f,0.0f,
                   0.0f,1.0f,0.0f,0.0f,
                   0.0f,0.0f,1.0f,0.0f,
                   0.0f,0.0f,0.0f,1.0f);
 

     eye = vec3(0.0f,0.0f,0.0f);
	 look = vec3(0.0f,0.0f,1.0f);
     puppet_pos = vec3(0.0f,0.0f,0.0f);
     y_angle = 0.0f;
     cam_distance = 10;

     mouse_x = 0.0;
     mouse_y = 0.0;
     offset_x =0.0;
     offset_y =0.0;
     ntran_x = 0.0;
     ntran_y = 0.0;
     ntran_z = 0.0;
     tran_x = 0.0;
     tran_y = 0.0;
     tran_z = 0.0;
     Frontface_culling = false;
     Backface_culling = false;
     zbuf = 1;
     zb = true;
     BorF = 0;
     dc = false;
     justreset = true;
	 timer = 0.0f;
     walk = false;
	 turn_left = false;
	 turn_right = false;
     walking_speed = 10.0;
     BGM("MMExploreEvening16.wav");
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
	m_shader_arcCircle.attachVertexShader( getAssetFilePath("CityShader.vs").c_str() );
	m_shader_arcCircle.attachFragmentShader( getAssetFilePath("CityShader.fs").c_str() );
	m_shader_arcCircle.link();

	m_shader_skybox.generateProgramObject();
	m_shader_skybox.attachVertexShader( getAssetFilePath("skyboxShader.vs").c_str() );
	m_shader_skybox.attachFragmentShader( getAssetFilePath("skyboxShader.fs").c_str() );
	m_shader_skybox.link();


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

		CHECK_GL_ERRORS;
	}


	//-- Enable input slots for m_vao_arcCircle:
	{
		glBindVertexArray(m_vao_City);

		// Enable the vertex shader attribute location for "position" when rendering.
		m_City_positionAttribLocation = m_shader_arcCircle.getAttribLocation("position");
		glEnableVertexAttribArray(m_City_positionAttribLocation);

		// Enable the attribute index location for "textCoord" when rendering.
        m_City_textAttribLocation = m_shader_arcCircle.getAttribLocation( "textCoord" );
        glEnableVertexAttribArray(m_City_textAttribLocation);

		CHECK_GL_ERRORS;
	}

	//-- Enable input slots for m_vao_skybox:
	{
		glBindVertexArray(m_vao_skybox);

		// Enable the vertex shader attribute location for "position" when rendering.
		m_skybox_positionAttribLocation = m_shader_skybox.getAttribLocation("position");
		glEnableVertexAttribArray(m_skybox_positionAttribLocation);

		CHECK_GL_ERRORS;
	}


	// Restore defaults
	glBindVertexArray(0);
}

//----------------------------------------------------------------------------------------
void A3::uploadCityDataToVbo(){
	GLfloat he = 0.0f;
	//ground
	GLfloat cityVertices[] = {
		-10.0f, he, -10.0f, 0.0f,10.0f,  //lu
		10.0f,  he, -10.0f, 10.0f,10.0f,   //ru
		-10.0f, he, 10.0f, 0.0f, 0.0f,   //ld
        -10.0f, he, 10.0f, 0.0f, 0.0f,   //ld
        10.0f, he, 10.0f,  10.0f, 0.0f,   //rd
		10.0f, he, -10.0f,  10.0f, 10.0f,   //ru

			//box
		
		-1.0f, 1.0f, -1.0f,0.0f,0.0f,		// A
		-1.0f, -1.0f, -1.0f,0.0f,0.0f,	// B
		1.0f, -1.0f, -1.0f,0.0f,0.0f,		// C
		1.0f, -1.0f, -1.0f,0.0f,0.0f,		// C
		1.0f, 1.0f, -1.0f,0.0f,0.0f,		// D
		-1.0f, 1.0f, -1.0f,0.0f,0.0f,		// A

		// left
		-1.0f, -1.0f, 1.0f,0.0f,0.0f,		// E
		-1.0f, -1.0f, -1.0f,0.0f,0.0f,	// B
		-1.0f, 1.0f, -1.0f,0.0f,0.0f,		// A
		-1.0f, 1.0f, -1.0f,0.0f,0.0f,		// A
		-1.0f, 1.0f, 1.0f,0.0f,0.0f,		// F
		-1.0f, -1.0f, 1.0f,0.0f,0.0f,		// E

		// right
		1.0f, -1.0f, -1.0f,0.0f,0.0f,		// C
		1.0f, -1.0f, 1.0f,0.0f,0.0f,		// G
		1.0f, 1.0f, 1.0f,0.0f,0.0f,		// H
		1.0f, 1.0f, 1.0f,0.0f,0.0f,		// H
		1.0f, 1.0f, -1.0f,0.0f,0.0f,		// D
		1.0f, -1.0f, -1.0f,0.0f,0.0f,		// C

		// front
		-1.0f, -1.0f, 1.0f,0.0f,0.0f,  // E
		-1.0f, 1.0f, 1.0f,0.0f,0.0f,  // F
		1.0f, 1.0f, 1.0f,0.0f,0.0f,  // H
		1.0f, 1.0f, 1.0f,0.0f,0.0f,  // H
		1.0f, -1.0f, 1.0f,0.0f,0.0f,  // G
		-1.0f, -1.0f, 1.0f,0.0f,0.0f,  // E

		// up
		-1.0f, 1.0f, -1.0f,0.0f,0.0f,  // A
		1.0f, 1.0f, -1.0f,0.0f,0.0f,  // D
		1.0f, 1.0f, 1.0f,0.0f,0.0f,  // H
		1.0f, 1.0f, 1.0f,0.0f,0.0f,  // H
		-1.0f, 1.0f, 1.0f,0.0f,0.0f,  // F
		-1.0f, 1.0f, -1.0f,0.0f,0.0f,  // A

		// down
		-1.0f, -1.0f, -1.0f,0.0f,0.0f,  // B
		-1.0f, -1.0f, 1.0f,0.0f,0.0f,   // E
		1.0f, -1.0f, 1.0f,0.0f,0.0f,    // G
		1.0f, -1.0f, 1.0f,0.0f,0.0f,    // G
		1.0f, -1.0f, -1.0f,0.0f,0.0f,   // C
		-1.0f, -1.0f, -1.0f,0.0f,0.0f  // B

	};
	         float half_sqrt3 = float(std::sqrt(3.0)) * 0.5f;


    glGenBuffers(1,&m_vbo_cityPositions);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_cityPositions);
    glBufferData(GL_ARRAY_BUFFER, 
			     7*6*5*sizeof(GLfloat), cityVertices,
                        GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	CHECK_GL_ERRORS;


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

	// Generate VBO to store all skybox position data
	{
		glGenBuffers(1, &m_vbo_skybox);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo_skybox);

		glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices),
				skyboxVertices, GL_STATIC_DRAW);

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
	glBindVertexArray(m_vao_City);

	// Tell GL how to map data from the vertex buffer "m_vbo_arcCircle" into the
	// "position" vertex attribute location for any bound vertex shader program.
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_cityPositions);
//	glVertexAttribPointer(m_arc_positionAttribLocation, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    glVertexAttribPointer(m_City_positionAttribLocation, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), nullptr);
	glVertexAttribPointer( m_City_textAttribLocation, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (GLvoid*)(3*sizeof(GL_FLOAT)));
	//-- Unbind target, and restore default values:
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	CHECK_GL_ERRORS;



	// Bind VAO in order to record the data mapping.
	glBindVertexArray(m_vao_skybox);

	// Tell GL how to map data from the vertex buffer "m_vbo_skybox" into the
	// "position" vertex attribute location for any bound vertex shader program.
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_skybox);
	glVertexAttribPointer(m_skybox_positionAttribLocation, 3, GL_FLOAT, GL_FALSE,
			3*sizeof(GL_FLOAT), nullptr);

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
	m_view = glm::lookAt(eye, 
			//eye+ vec3(nVRotMat * vec4(look,1.0f)),
            eye - vec3( nVRotMat * vec4(look,1.0)),
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
//	look = vec3(0,0,10);
	//eye = puppet_pos + vec3( nVRotMat * vec4(look,1.0));
	if(walk){
		float ws = walking_speed / ImGui::GetIO().Framerate;
        float xoff = (puppet_pos.x - eye.x)/cam_distance * ws;
		float zoff = (puppet_pos.z - eye.z)/cam_distance * ws;
        if(puppet_pos.x + xoff < 99 && puppet_pos.x + xoff >-99){
            if(!(puppet_pos.x + xoff < -2 && puppet_pos.x + xoff > -10 &&
				puppet_pos.z  < -6 && puppet_pos.z > -14))
				    puppet_pos.x += xoff;
		}
		if(puppet_pos.z + zoff < 99 && puppet_pos.z + zoff >-99)
             if(!(puppet_pos.x  < -2 && puppet_pos.x  > -10 &&
				puppet_pos.z + zoff < -6 && puppet_pos.z + zoff > -14))
		  	    puppet_pos.z += zoff;
			//puppet_pos.y += (puppet_pos.y - eye.y)/cam_distance * ws;
	}//end if walk
	if(turn_left){
		y_angle += 0.03;
	}else if(turn_right){
		y_angle -= 0.03;
	}//end if turn
	eye.x = puppet_pos.x + cam_distance * sin(y_angle);
	eye.z = puppet_pos.z + cam_distance * cos(y_angle);
	m_view = glm::lookAt(eye, 
			//eye+ vec3(nVRotMat * vec4(look,1.0f)),
            puppet_pos,
			vec3(0.0f, 1.0f, 0.0f));
	timer += 1.2/ImGui::GetIO().Framerate;

	uploadCommonSceneUniforms();
	uploadCityUniformsToShader();
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


    ImGui::BeginMainMenuBar();
    if(ImGui::BeginMenu("File")){
        if(ImGui::MenuItem("Reset",NULL)) {reset();}
        if(ImGui::MenuItem("Quit",NULL)){glfwSetWindowShouldClose(m_window, GL_TRUE);
};
        ImGui::EndMenu();
    }//end if Bg

    if(ImGui::BeginMenu("Z-Buf")){
        if(ImGui::MenuItem("Z-buf",NULL,&zb)) {
             zbuf = (zbuf + 1)%2;
        }
        if(ImGui::MenuItem("BackFace",NULL,&Backface_culling)){
            if(2 == BorF){
              Backface_culling = false;
              BorF = 0;
            }else{
              Backface_culling = true;
              Frontface_culling = false;
              BorF = 2;
           }
        };
        if(ImGui::MenuItem("FrontFace",NULL,&Frontface_culling)){
            if(1 == BorF){
              Frontface_culling = false;
              BorF = 0;
            }else{
             Backface_culling = false;
             Frontface_culling = true;
             BorF = 1;
            }
        };
        ImGui::MenuItem("DrawCircle",NULL,&dc);

        ImGui::EndMenu();
    }//end if Bg
    ImGui::EndMainMenuBar();


	ImGui::Begin("Properties", &showDebugWindow, ImVec2(100,100), opacity,
			windowFlags);


		// Add more gui elements here here ...


		// Create Button, and check if it was clicked:
		if( ImGui::Button( "Quit Application" ) ) {
			glfwSetWindowShouldClose(m_window, GL_TRUE);
		}


		if( ImGui::Button( "Reset Application" ) ) {
			reset();
		}

        ImGui::PushID(0);
        if( ImGui::RadioButton( "Position(P)",&shortcut,GLFW_KEY_P)){}
        ImGui::PopID();

        ImGui::PushID(1);
        if( ImGui::RadioButton( "Joints(J)",&shortcut,GLFW_KEY_J)){}
        ImGui::PopID();

        ImGui::PushID(2);
        if( ImGui::RadioButton( "Front_cull(F)",&BorF,1)){
                 Frontface_culling = true;
                 Backface_culling = false;
        }
        ImGui::PopID();

        ImGui::PushID(3);
        if( ImGui::RadioButton( "Back_cull(B)",&BorF,2)){
                 Backface_culling = true;
                 Frontface_culling = false;
        }
        ImGui::PopID();

        ImGui::PushID(6);
        if( ImGui::RadioButton( "No cull",&BorF,0)){
                 Backface_culling = false;
                 Frontface_culling = false;
        }
        ImGui::PopID();

        ImGui::PushID(4);
        if( ImGui::RadioButton( "Z-Buffer On(Z)",&zbuf,1)){
            zb = true;
        }
        ImGui::PopID();

        ImGui::PushID(5);
        if( ImGui::RadioButton( "Z-Buffer Off(Z)",&zbuf,0)){
            zb = false;
        }
        ImGui::PopID();
        
        
        ImGui::Text( "Framerate: %.1f FPS", ImGui::GetIO().Framerate );
		ImGui::Text( "Timer: %.01f Ms", timer );
		ImGui::Text( "x,y,z: (%.01f,%.01f,%.01f)",puppet_pos.x,puppet_pos.y,puppet_pos.z);

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
	renderCity();
	renderSceneGraph(*m_rootNode);


//	glDisable( GL_DEPTH_TEST );
	   renderArcCircle();
}

//----------------------------------------------------------------------------------------
void A3::renderSceneGraph(SceneNode & root) {

    if(1==zbuf){
       glEnable(GL_DEPTH_TEST);
    }else{
       glDisable( GL_DEPTH_TEST );
    }

    if(Frontface_culling){
       glEnable(GL_CULL_FACE);
       glCullFace(GL_FRONT);
    }else if(Backface_culling){
       glEnable(GL_CULL_FACE);
       glCullFace(GL_BACK);
    }




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
                  mat4 tran_mat = mat4(1,0,0,0,
                                   0,1,0,0,
                                   0,0,1,0,
                                   puppet_pos.x,
								   puppet_pos.y,
								   puppet_pos.z,1);
                  father_tans = st_ptr->father_trans *
					  tran_mat *
					  st_ptr->get_transform()*
					  roty(y_angle)*
					  nObjRotMat;
                  IsRoot = false;
                }else{//end isroot
                  father_tans = st_ptr->father_trans * st_ptr->get_transform();
                }

                

                //Joint
                if(st_ptr->m_nodeType == NodeType::JointNode){
                     JointNode * Jp = (JointNode*)st_ptr;
                     if(justreset){
                         Jp->cur_x = Jp->m_joint_x.init;
                         Jp->cur_y = Jp->m_joint_y.init;
                     }
                     float angx = Jp->cur_x;
                     float angy = Jp->cur_y;

                     if(selected[id]){
                         angx += offset_x;
                         angy += offset_y;
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

					 float tax = angx;
					 float tay = angy;
                     float speed = 8;
					 if(!walk){
						  if("sword-joint"==Jp->m_name){
							  tax = angx + 3.14159*cos(timer);
                              tay = angy + 4.5* 3.14159*sin(timer *3/5);
						  }
					 } else if(Jp->m_name == "neck-joint"){
                          tay = angy + 0.02*sin(timer*speed);
	                 }else if("sword-joint"==Jp->m_name){
						  tay = angy + 3*3.14159*(cos(timer*0.7)) + 3.14159/2;
                     }else if("ep-gauche-jointure"==Jp->m_name){
                           tax = angx + 0.45* (sin(-timer*speed) - 0.5 );
                     }else if("ep-droit-jointure"==Jp->m_name){
                           tax = angx + 0.45* (sin(timer*speed) - 0.5 );
				     }else if("cuisse_g_jointure"==Jp->m_name){
						  tax = angx + 0.3*(sin(timer*speed) + 0.1);
					 }else if("cuisse_d_jointure"==Jp->m_name){
						  tax = angx + 0.3*(sin(-timer*speed) + 0.1);
                     }else if("jambe_g_jointure"==Jp->m_name){
						  tax = angx + 0.2*(sin(timer*speed+1) + 1);
                     }else if("jambe_d_jointure"==Jp->m_name){
						  tax = angx + 0.2*(sin(-timer*speed-1)+ 1);


					 }//end if m_name
   //                  mat4 rotmat_y = mat4(cos(ang),0,-sin(ang),0,
   //                                       0,1,0,0,
   //                                       sin(ang),0,cos(ang),0,
   //                                       0,0,0,1); 
                     father_tans = father_tans * roty(tay)* rotx(tax);
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
        justreset = false;

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

/*	
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

*/

	glBindVertexArray(m_vao_skybox);
    glDepthFunc(GL_LEQUAL);

	glm::mat4 model;
    model = glm::translate(mat4(),eye + vec3(0.0f,0.0f,0.0f));
	model = glm::scale(model,vec3(2.0f,2.0f,2.0f));
	m_shader_skybox.enable();
		glUniformMatrix4fv(m_shader_skybox.getUniformLocation( "projection"),
            1, GL_FALSE, glm::value_ptr(m_perpsective));
	    glUniformMatrix4fv(m_shader_skybox.getUniformLocation( "model"),
            1, GL_FALSE, glm::value_ptr(model));
    	glUniformMatrix4fv(m_shader_skybox.getUniformLocation( "view"),
			1, GL_FALSE, glm::value_ptr(m_view));
	    glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
        glUniform1i(m_shader_skybox.getUniformLocation("skybox"),0);
		glDrawArrays(GL_TRIANGLES,0,36);
	m_shader_skybox.disable();
	glBindVertexArray(0);
	glDepthFunc(GL_LESS);
	CHECK_GL_ERRORS;
}
//---------------------------------------------------------------------------------------
void A3::renderCity() {

	//drawing the ground
    glBindVertexArray(m_vao_City);
	m_shader_arcCircle.enable();
	    glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D,CityTexture);
		glUniform1i(m_shader_arcCircle.getUniformLocation("tex"),0);
		glDrawArrays(GL_TRIANGLES, 0, 6);
        GLint M_uni = m_shader_arcCircle.getUniformLocation( "M" );

				vec3 y_axis(0.0f,1.0f,0.0f);
				mat4 W;
				int nrot = 0;
				W *= glm::scale(mat4(), vec3(2,2,2));
                W = glm::translate( W, vec3( -3,0.0,-5.0 ) );
                glUniformMatrix4fv( M_uni, 1, GL_FALSE, value_ptr( W ) );
                glDrawArrays(GL_TRIANGLES, 6, 36); 




	m_shader_arcCircle.disable();
    glBindVertexArray(0);
	CHECK_GL_ERRORS;       

}//end renderCity

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


        if( GLFW_KEY_T == shortcut){   //model rotation
            if (m_mouseRightAct ) {
                float vx = 0.0;
                float vy = 0.0;
                float vz = 0.0;
                float fDiameter;
                int iCenterX, iCenterY;
                float fNewModX, fNewModY, fOldModX,fOldModY;
                fDiameter = (m_windowWidth < m_windowHeight) ? m_windowWidth * 0.5: m_windowHeight * 0.5;

                iCenterX = m_windowWidth / 2;
                iCenterY = m_windowHeight / 2;
                fOldModX = mouse_x - iCenterX;
                fOldModY = mouse_y - iCenterY;
                fNewModX = xPos - iCenterX;
                fNewModY = yPos - iCenterX;

                mat4 temprot = mat4(1,0,0,0,
                                    0,1,0,0,
                                    0,0,1,0,
                                    0,0,0,1);
                vCalcRotVec(fNewModX,fNewModY,fOldModX,fOldModY,fDiameter,&vx,&vy,&vz);
                
                vAxisRotMatrix(vx,vy,vz,temprot);
//                nObjRotMat = temprot * ObjRotMat;
                nObjRotMat = roty(0.1*(xPos - mouse_x)) * ObjRotMat;
//                nObjRotMat = rotz(0.1*(xPos - mouse_x)) * ObjRotMat;
            }else{
                 mouse_x = xPos;
                 mouse_y = yPos;

            }

            if (m_mouseLeftAct) {
//                 nObjRotMat = rotx(0.1*(yPos - mouse_y)) * ObjRotMat;
            }else{
//                 mouse_y = yPos;
            }
        }else if( GLFW_KEY_J == shortcut){    //part rotation
              if (m_mouseLeftAct ) {
              }else{
//                 mouse_x = xPos;
              }

              if (m_mouseMidAct) {
                   offset_x = (mouse_x - xPos)/1;
                   offset_y = (mouse_y - yPos)/0.5;
              }else if(m_mouseRightAct){
              }else{
                 mouse_x = xPos;
                 mouse_y = yPos;
              }
            

        }else if(GLFW_KEY_P == shortcut){


             if (m_mouseLeftAct){
                 ntran_x = tran_x + (xPos - mouse_x)*0.1;
                 ntran_y = tran_y - (yPos - mouse_y)*0.1;
             }else if(m_mouseMidAct){
                 ntran_z = ntran_z + (yPos - mouse_y)*0.01;

             }else if (m_mouseRightAct ) {
                float vx = 0.0;
                float vy = 0.0;
                float vz = 0.0;
                float fDiameter;
                int iCenterX, iCenterY;
                float fNewModX, fNewModY, fOldModX,fOldModY;
                fDiameter = (m_windowWidth < m_windowHeight) ? m_windowWidth * 0.5: m_windowHeight * 0.5;

                iCenterX = m_windowWidth / 2;
                iCenterY = m_windowHeight / 2;
                fOldModX = mouse_x - iCenterX;
                fOldModY = mouse_y - iCenterY;
                fNewModX = xPos - iCenterX;
                fNewModY = yPos - iCenterX;

                mat4 temprot = mat4(1,0,0,0,
                                    0,1,0,0,
                                    0,0,1,0,
                                    0,0,0,1);
                vCalcRotVec(fNewModX,fNewModY,fOldModX,fOldModY,fDiameter,&vx,&vy,&vz);
                
                vAxisRotMatrix(vx,vy,vz,temprot);
                nObjRotMat = temprot * ObjRotMat;
 

             }else{
                 mouse_x = xPos;
                 mouse_y = yPos;
             }//end m left
			 
         }else if(GLFW_KEY_V == shortcut){ //rotate view
             if(m_mouseMidAct){
				 nVRotMat = roty(0.1*(xPos - mouse_x)) * VRotMat;
			 }else if(m_mouseRightAct){
				 nVRotMat = rotz(0.1*(xPos - mouse_x)) * VRotMat;
			 }else{
				 mouse_x = xPos;
			 }//end m_m

			 if(m_mouseLeftAct){
				 nVRotMat = rotx(0.1*(yPos - mouse_y)) * VRotMat;
			 }else{
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
				VRotMat = nVRotMat;
                ObjRotMat = nObjRotMat;
                JustRelease = true;
                tran_x = ntran_x;
                tran_y = ntran_y;
                tran_z = ntran_z;
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
        }else if(key == GLFW_KEY_Q){
              glfwSetWindowShouldClose(m_window, GL_TRUE);
        }else if(key == GLFW_KEY_R){
              reset();
        }else if(GLFW_KEY_B == key){
            if(2 == BorF){
              Backface_culling = false;
              BorF = 0;
            }else{
              Backface_culling = true;
              Frontface_culling = false;
              BorF = 2;
           }
        }else if(GLFW_KEY_F == key){
            if(1 == BorF){
              Frontface_culling = false;
              BorF = 0;
            }else{
             Backface_culling = false;
             Frontface_culling = true;
             BorF = 1;
            }
         }else if(GLFW_KEY_Z == key){
             zbuf = (zbuf + 1)%2;
                 zb = !zb;
 
        }else if(key == GLFW_KEY_C){
              dc = !dc;
		}else if(key == GLFW_KEY_UP){  //step forward
              walk = true;
	 		  //eye += vec3(nVRotMat*vec4(look,1.0f));
			  //puppet_pos -= look;
	    }else if(key == GLFW_KEY_LEFT){
			turn_left = true;
		}else if(key == GLFW_KEY_RIGHT){
			turn_right = true;
		}else{
            
			shortcut = key;
        }//end if key

	}else if(action == GLFW_RELEASE ) {
		if(key == GLFW_KEY_UP){
			walk = false;
		}//end if up
		if(key == GLFW_KEY_LEFT){
			turn_left = false;
		}//end if up
		if(key == GLFW_KEY_RIGHT){
			turn_right = false;
		}//end if up



	}

	// Fill in with event handling code...

	return eventHandled;
}

//--------------------------------------
//3d rotation
void A3:: vCalcRotVec(float fNewX, float fNewY,
                float fOldX, float fOldY,
                float fDiameter,
                float *fVecX, float *fVecY, float *fVecZ){

   long  nXOrigin, nYOrigin;
   float fNewVecX, fNewVecY, fNewVecZ,        /* Vector corresponding to new mouse location */
         fOldVecX, fOldVecY, fOldVecZ,        /* Vector corresponding to old mouse location */
         fLength;

   fNewVecX    = fNewX * 2.0 / fDiameter;
   fNewVecY    = fNewY * 2.0 / fDiameter;
   fNewVecZ    = (1.0 - fNewVecX * fNewVecX - fNewVecY * fNewVecY);

   if (fNewVecZ < 0.0) {
         fLength = sqrt(1.0 - fNewVecZ);
      fNewVecZ  = 0.0; 
      fNewVecX /= fLength;
      fNewVecY /= fLength;
   } else {
       fNewVecZ = sqrt(fNewVecZ);
   }
   fOldVecX    = fOldX * 2.0 / fDiameter;
   fOldVecY    = fOldY * 2.0 / fDiameter;
   fOldVecZ    = (1.0 - fOldVecX * fOldVecX - fOldVecY * fOldVecY);

   if (fOldVecZ < 0.0) {
       fLength = sqrt(1.0 - fOldVecZ);
       fOldVecZ  = 0.0;
       fOldVecX /= fLength;
       fOldVecY /= fLength;
   } else {
       fOldVecZ = sqrt(fOldVecZ);
   }

   *fVecX = fOldVecY * fNewVecZ - fNewVecY * fOldVecZ;
   *fVecY = fOldVecZ * fNewVecX - fNewVecZ * fOldVecX;
   *fVecZ = fOldVecX * fNewVecY - fNewVecX * fOldVecY;



}//end vCalcR


//-------------------------------------
void A3::vAxisRotMatrix(float fVecX, float fVecY, float fVecZ, mat4 &mNewMat) {
    float fRadians, fInvLength, fNewVecX, fNewVecY, fNewVecZ;
    fRadians = sqrt(fVecX * fVecX + fVecY * fVecY + fVecZ * fVecZ);

    /* If the vector has zero length - return the identity matrix */
   if (fRadians > -0.000001 && fRadians < 0.000001) {
       mNewMat = mat4(1,0,0,0,
                      0,1,0,0,
                      0,0,1,0,
                      0,0,0,1);
       return;
   }

    /* Normalize the rotation vector now in preparation for making
     *      * rotation matrix. 
     *           */
   fInvLength = 1 / fRadians;
   fNewVecX   = fVecX * fInvLength;
   fNewVecY   = fVecY * fInvLength;
   fNewVecZ   = fVecZ * fInvLength;
    /* Create the arbitrary axis rotation matrix */
   double dSinAlpha = sin(fRadians);
   double dCosAlpha = cos(fRadians);
   double dT = 1 - dCosAlpha;

       mNewMat[0][0] = dCosAlpha + fNewVecX*fNewVecX*dT;
       mNewMat[0][1] = fNewVecX*fNewVecY*dT + fNewVecZ*dSinAlpha;
       mNewMat[0][2] = fNewVecX*fNewVecZ*dT - fNewVecY*dSinAlpha;
       mNewMat[0][3] = 0;

       mNewMat[1][0] = fNewVecX*fNewVecY*dT - dSinAlpha*fNewVecZ;
       mNewMat[1][1] = dCosAlpha + fNewVecY*fNewVecY*dT;
       mNewMat[1][2] = fNewVecY*fNewVecZ*dT + dSinAlpha*fNewVecX;
       mNewMat[1][3] = 0;

       mNewMat[2][0] = fNewVecZ*fNewVecX*dT + dSinAlpha*fNewVecY;
       mNewMat[2][1] = fNewVecZ*fNewVecY*dT - dSinAlpha*fNewVecX;
       mNewMat[2][2] = dCosAlpha + fNewVecZ*fNewVecZ*dT;
       mNewMat[2][3] = 0;

       mNewMat[3][0] = 0;
       mNewMat[3][1] = 0;
       mNewMat[3][2] = 0;
       mNewMat[3][3] = 1;


}//end of vAxis


GLuint A3::LoadTextureRAW( const char * imagepath)
{
    GLuint textureId;
    unsigned char header[54];
	unsigned int dataPos;
	int width,height,format;
	unsigned int imageSize;
	unsigned char * data;

	//open file
	FILE * file = fopen(imagepath,"rb");
	if(!file){cout << "Image could not be opend"<<endl; return 0;}
	if( fread(header,1,54,file)!=54){
        cout << "Not a correct BMP file1"<<endl;
		return 0;
	}//endfread
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
    return textureId;

}


#define GL_TEXTURE_CUBE_MAP_POSITIVE_X 0x8515
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_X 0x8516
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Y 0x8517
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y 0x8518
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Z 0x8519
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z 0x851A
GLuint A3::LoadCubeMapTexture(vector<const char *> imagepath){
    GLuint textId;
    glGenTextures(1,&textId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textId);
	unsigned char header[54];
	unsigned int dataPos;
	int width,height,format;
	unsigned int imageSize;
//	unsigned char * data;
    GLubyte *data = NULL;

	//read filds for each face and send it to buffer
    for(vector<const char*>::size_type i=0; i < imagepath.size();++i ){

/*		
	    //open file
    	FILE * file = fopen(imagepath[i],"rb");
    	if(!file){cout << "Image could not be opend"<<endl; return 0;}
	    if( fread(header,1,54,file)!=54){
            cout << "Not a correct BMP file1"<<endl;
	    	return 0;
       	}//endfread
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
*/
		int channels = 0;
//		data = SOIL_load_image(imagepath[i],&width,&height,&channels,SOIL_LOAD_RGB);
        data = stbi_load(imagepath[i],&width,&height,&channels,3);
		if (data == NULL)
		{
				std::cerr << "Error::loadCubeMapTexture could not load texture file:"
					<< imagepath[i] << std::endl;
				return 0;
		}
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,0,
				GL_RGB, width , height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
//		SOIL_free_image_data(data);
        stbi_image_free(data);  
	}//end for vec
    glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_CUBE_MAP,0);


    return textId;

}//end load cube

GLuint A3::LoadTexture( const char * imagepath)
{
    GLuint textureId;
    unsigned char header[54];
	unsigned int dataPos;
	int width,height,format;
	unsigned int imageSize;
	unsigned char * data;
	int channels = 0;

	data = stbi_load(imagepath,&width,&height,&channels,3);
	
	glGenTextures(1, &textureId);
   // glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D,textureId);

	//filter
//    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_NEAREST );
//    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    //wrap
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT );

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width , height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, 0);
    return textureId;

}

void A3::uploadCityUniformsToShader()
{
        m_shader_arcCircle.enable();
                // Query the shader program for the location number of the uniform named "color".
               // GLint uniformLocation_colour = m_shader_arcCircle.getUniformLocation("colour");
                GLint P_uni = m_shader_arcCircle.getUniformLocation( "P" );
                GLint V_uni = m_shader_arcCircle.getUniformLocation( "V" );
                GLint M_uni = m_shader_arcCircle.getUniformLocation( "M" );

                // Set the uniform's value.
               // glUniform3f(uniformLocation_colour, 0.2f, 0.5f,
               //                 0.5f);

               // vec3 z_axis(0.0f,0.0f,1.0f);
                //mat4 transform = glm::translate(mat4(), vec3(0.0f,0.0f, 0.0f));
                //transform *= glm::scale(mat4(), vec3(10.0f,10.0f,10.0f));
                //mat4 transform =  m_view;

				vec3 y_axis(0.0f,1.0f,0.0f);
				mat4 W;
				int nrot = 0;
				W *= glm::rotate(mat4(), ((float) (nrot / 100)), y_axis);
				W *= glm::scale(mat4(), vec3(10,1,10));
                W = glm::translate( W, vec3( 0,-5.1,0 ) );
                //GLint uniformLocation_modelMatrix = m_shader_arcCircle.getUniformLocation("transform");
                glUniformMatrix4fv( P_uni, 1, GL_FALSE, value_ptr( m_perpsective ) );
                glUniformMatrix4fv( V_uni, 1, GL_FALSE, value_ptr( m_view ) );
                glUniformMatrix4fv( M_uni, 1, GL_FALSE, value_ptr( W ) );
				//glUniformMatrix4fv(uniformLocation_modelMatrix, 1, GL_FALSE,
                 //               glm::value_ptr(transform));

        m_shader_arcCircle.disable();

        CHECK_GL_ERRORS;
}

