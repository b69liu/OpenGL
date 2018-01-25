#pragma once

#include "cs488-framework/CS488Window.hpp"
#include "cs488-framework/OpenGLImport.hpp"
#include "cs488-framework/ShaderProgram.hpp"
#include "cs488-framework/MeshConsolidator.hpp"

#include "SceneNode.hpp"

#include <glm/glm.hpp>
#include <memory>
#include <vector>

struct LightSource {
	glm::vec3 position;
	glm::vec3 rgbIntensity;
};


class A3 : public CS488Window {
public:
	A3(const std::string & luaSceneFile);
	virtual ~A3();

protected:
	virtual void init() override;
	virtual void appLogic() override;
	virtual void guiLogic() override;
	virtual void draw() override;
	virtual void cleanup() override;

	//-- Virtual callback methods
	virtual bool cursorEnterWindowEvent(int entered) override;
	virtual bool mouseMoveEvent(double xPos, double yPos) override;
	virtual bool mouseButtonInputEvent(int button, int actions, int mods) override;
	virtual bool mouseScrollEvent(double xOffSet, double yOffSet) override;
	virtual bool windowResizeEvent(int width, int height) override;
	virtual bool keyInputEvent(int key, int action, int mods) override;

	//-- One time initialization methods:
	void processLuaSceneFile(const std::string & filename);
	void createShaderProgram();
	void enableVertexShaderInputSlots();
	void uploadVertexDataToVbos(const MeshConsolidator & meshConsolidator);
	void mapVboDataToVertexShaderInputLocations();
	void initViewMatrix();
	void initLightSources();

	void initPerspectiveMatrix();
	void uploadCommonSceneUniforms();
	void renderSceneGraph(SceneNode &node);
	void renderArcCircle();

    void vCalcRotVec(float fNewX, float fNewY,
                    float fOldX, float fOldY,
                    float fDiameter,
                    float *fVecX, float *fVecY, float *fVecZ);

    void vAxisRotMatrix(float fVecX, float fVecY, float fVecZ,
                        glm::mat4 &mNewMat);


	glm::mat4 m_perpsective;
	glm::mat4 m_view;

    //camera parameters
	float y_angle;     //ratation angle around y axis
	float cam_distance;
   glm::vec3 eye,look,puppet_pos; 
   glm::mat4 VRotMat, nVRotMat;


	LightSource m_light;


	//Ciry resource
	GLuint m_vao_City;
	GLuint m_vbo_cityPositions;
	GLint m_City_positionAttribLocation;
    GLint m_City_textAttribLocation;
	GLuint CityTexture;

	void uploadCityDataToVbo();
    void uploadCityUniformsToShader();
	GLuint LoadTexture( const char * imagepath);
    void renderCity();

	//-- GL resources for mesh geometry data:
	GLuint m_vao_meshData;
	GLuint m_vbo_vertexPositions;
	GLuint m_vbo_vertexNormals;
	GLint m_positionAttribLocation;
	GLint m_normalAttribLocation;
	ShaderProgram m_shader;

	//-- GL resources for trackball circle geometry:
	GLuint m_vbo_arcCircle;
	GLuint m_vao_arcCircle;
	GLint m_arc_positionAttribLocation;
	ShaderProgram m_shader_arcCircle;

	// BatchInfoMap is an associative container that maps a unique MeshId to a BatchInfo
	// object. Each BatchInfo object contains an index offset and the number of indices
	// required to render the mesh with identifier MeshId.
	BatchInfoMap m_batchInfoMap;

	std::string m_luaSceneFile;

	std::shared_ptr<SceneNode> m_rootNode;

        // rotate picked component
        glm::mat4 ObjRotMat, nObjRotMat,RotMat,nRotMat;
        float ntran_x,ntran_y,ntran_z,tran_x,tran_y,tran_z;
        float mouse_x =0;
        float mouse_y = 0;
        void reset();
        int justreset = false;
        int shortcut = 0;

        //mouse
        bool m_mouseLeftAct = false;
        bool m_mouseRightAct = false;
        bool m_mouseMidAct = false;
        bool Frontface_culling = false;
        bool Backface_culling = false;
        int BorF = 0;  // 0= both off 1 =Front 2 = Back
        int zbuf = 1;
        bool zb = true;        //switch of z buffer
        bool dc = true; //draw cycle
        float timer;
        bool walk,turn_left,turn_right;
        float walking_speed;

		GLuint m_vbo_skybox;
        GLuint m_vao_skybox;
        GLint m_skybox_positionAttribLocation;
        ShaderProgram m_shader_skybox; 
		GLuint skyboxTexture;

		//load bmp to stack
        GLuint LoadTextureRAW( const char * filename );
        GLuint LoadCubeMapTexture(std::vector<const char *> imagepath);
};
