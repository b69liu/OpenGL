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

        p_buffer.push_back(1.0f);
        p_buffer.push_back(0.0f);
        p_buffer.push_back(0.0f);

        p_buffer.push_back(0.0f);
        p_buffer.push_back(1.0f);
        p_buffer.push_back(0.0f);

        p_buffer.push_back(0.0f);
        p_buffer.push_back(0.0f);
        p_buffer.push_back(1.0f);

// the modelling coordinates

        p_buffer.push_back(0.0f);
        p_buffer.push_back(0.0f);
        p_buffer.push_back(0.0f);

        p_buffer.push_back(1.0f);
        p_buffer.push_back(0.0f);
        p_buffer.push_back(0.0f);

        p_buffer.push_back(0.0f);
        p_buffer.push_back(1.0f);
        p_buffer.push_back(0.0f);

        p_buffer.push_back(0.0f);
        p_buffer.push_back(0.0f);
        p_buffer.push_back(1.0f);


       

       reset();

}


void A2:: reset(){


     VRotMat = mat4(1.0f,0.0f,0.0f,0.0f,
                   0.0f,1.0f,0.0f,0.0f,
                   0.0f,0.0f,1.0f,0.0f,
                   0.0f,0.0f,0.0f,1.0f);
     nVRotMat = mat4(1.0f,0.0f,0.0f,0.0f,
                   0.0f,1.0f,0.0f,0.0f,
                   0.0f,0.0f,1.0f,0.0f,
                   0.0f,0.0f,0.0f,1.0f);

     VTranMat = mat4(1.0f,0.0f,0.0f,0.0f,
                   0.0f,1.0f,0.0f,0.0f,
                   0.0f,0.0f,1.0f,0.0f,
                   0.0f,0.0f,0.0f,1.0f);
     nVTranMat = mat4(1.0f,0.0f,0.0f,0.0f,
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

     ObjScaMat  = mat4(1.0f,0.0f,0.0f,0.0f,
                   0.0f,1.0f,0.0f,0.0f,
                   0.0f,0.0f,1.0f,0.0f,
                   0.0f,0.0f,0.0f,1.0f);

     nObjScaMat  = mat4(1.0f,0.0f,0.0f,0.0f,
                   0.0f,1.0f,0.0f,0.0f,
                   0.0f,0.0f,1.0f,0.0f,
                   0.0f,0.0f,0.0f,1.0f);


     mouse_x = 0;
     mouse_y = 0;



       near = -1.5;
       far = -10;
       onear = -1.5;
       ofar = -10;
//       vport_w = 6;
       theta = 3.14159f/6;
       otheta = 3.14159f/6;
       vport_h = 2* near* tan(theta/2);
       vport_w = vport_h;
       right = 2;
       top = 2;
       vtran_x = 0.0f;
       vtran_y = 0.0f;
       vtran_z = 3.0f;

       tran_x = 0.0f;
       tran_y = 0.0f;
       tran_z = 3.0f;

       ntran_x = 0.0f;
       ntran_y = 0.0f;
       ntran_z = 3.0f;

       rot_x = 0.0f;
       rot_y = 0.0f;
       rot_z = 0.0f;

       vrot_x = 0.0f;
       vrot_y = 0.0f;
       vrot_z = 0.0f;

       vport_centre_x = 0.0f;
       vport_centre_y = 0.0f;

       camNom = vec4(0.0f,1.0f,0.0f,0.0f);


       m_mouse1 = vec2(-0.9,-0.9);
       m_mouse2 = vec2(0.9,0.9);

       shortcut = GLFW_KEY_R;

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


mat4 ViewPort(float w, float h, float x, float y, float n, float f){
       return mat4(w/2,0,0,0,
                   0,h/2,0,0,
                   0,0,(f-n)/2,0,
                   x+w/2,y+h/2,(n+f)/2,1);

}


//return intersection point
/* lr = -1 or 1, xposition of line, point a, point b*/
vec4 vcut(float line, vec4 a,vec4 b, bool &work){
       vec4 left;
       vec4 right;
       if((line-a.x)*(line-b.x)>0 || line == b.x || line == a.x){ //check both side
         work = false;
         return vec4(0,0,0,0);
       }

       if(a.x > b.x){
          left = a;
          right = b;  
       }else{
          left = b;
          right = a;  
       }
       float dx = right.x - left.x;
       float dy = right.y - left.y;
       float sx = line - left.x;
       float slot = dy/dx;
       work = true;
       return vec4(line,left.y+sx*slot,0.0f,1.0f);

}



vec4 hcut(float line, vec4 a,vec4 b, bool &work){
       vec4 up;
       vec4 down;
       if((line-a.y)*(line-b.y)>0 || line == b.y || line == a.y){ //check both side
         work = false;
         return vec4(0,0,0,0);
       }

       if(a.y > b.y){
          up = a;
          down = b;
       }else{
          up = b;
          down = a;
       }
       float dx = up.x - down.x;
       float dy = up.y - down.y;
       float sy = up.y-line;
       float slop = dx/dy;
       work = true;
       return vec4(up.x-slop*sy,line,0.0f,1.0f);

}

vec4 dcut(float line, vec4 a,vec4 b, bool &work){
       vec4 n;
       vec4 f;
       if((line-a.z)*(line-b.z)>0 || line == b.z || line == a.z){ //check both side
         work = false;
         return vec4(0,0,0,0);
       }

       if(a.z < b.z){
          n = a;
          f = b;
       }else{
          n = b;
          f = a;
       }
       float dx =  f.x - n.x;
       float dz = f.z - n.z;
       float sz = f.z - line;
       float dy = f.y - n.y;
       work = true;
       return vec4(f.x - dx * sz/dz, f.y - dy*sz/dz, line,1.0f);

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
        Mats = mat4(vec4(1/tan(theta/2)/aspect,0,0,0),vec4(0,1/tan(theta/2),0,0)
                     ,vec4(0,0,-(far+near)/(far-near),-1),vec4(0,0,-2*far*near/(far-near),0));



        vec4 tempvec;
        vector<vec4> v4;
        vector<vec4> cp_lines;  //lines to be clipped
        vector<vec4> nlines;
        vector<vec4> flines;

        for(int i=0;i<p_buffer.size();i+=3){
          tempvec = vec4(p_buffer[i],p_buffer[i+1],p_buffer[i+2],1);
          //rotation
          if( i<12*3 || i>15*3 ){  //12,13,14,15 world frame
            tempvec = nObjRotMat * tempvec;
            tempvec = rotx(rot_x)* tempvec;
            tempvec = roty(rot_y)* tempvec;
            tempvec = rotz(rot_z)* tempvec;
          }

          //scale and trans to the front of camera
          tempvec =nObjScaMat * tempvec;
          //transportation cube
          if( i<12*3 || i>15*3 ){  //12,13,14,15 world frame
             tempvec = trans(ntran_x,ntran_y,ntran_z)*tempvec;
          }
    
          //camera view trans
          tempvec = viewMat(vec3(nVTranMat*vec4(0.0f,0.0f,5.0f,1.0f)), vec3(nVTranMat * nVRotMat*vec4(0.0f,0.0f,-1.0f,1.0f)),
                                vec3(nVRotMat* camNom))*tempvec;

          //projection
          tempvec =  Mats * tempvec;
          tempvec = ViewPort(vport_w, vport_h, vport_centre_x-vport_w/2, vport_centre_y-vport_h/2, near, far) * tempvec;

          v4.push_back(tempvec);

        } //end for i
        cp_lines.push_back(v4[0]);
        cp_lines.push_back(v4[1]);
        cp_lines.push_back(v4[1]);
        cp_lines.push_back(v4[2]);
        cp_lines.push_back(v4[2]);
        cp_lines.push_back(v4[3]);
        cp_lines.push_back(v4[3]);
        cp_lines.push_back(v4[0]);

        cp_lines.push_back(v4[0]);
        cp_lines.push_back(v4[4]);
        cp_lines.push_back(v4[1]);
        cp_lines.push_back(v4[5]);
        cp_lines.push_back(v4[2]);
        cp_lines.push_back(v4[6]);
        cp_lines.push_back(v4[3]);
        cp_lines.push_back(v4[7]);

        cp_lines.push_back(v4[4]);
        cp_lines.push_back(v4[5]);
        cp_lines.push_back(v4[5]);
        cp_lines.push_back(v4[6]);
        cp_lines.push_back(v4[6]);
        cp_lines.push_back(v4[7]);
        cp_lines.push_back(v4[7]);
        cp_lines.push_back(v4[4]);


        //model coor 24-29
        cp_lines.push_back(v4[8]);
        cp_lines.push_back(v4[9]);
        cp_lines.push_back(v4[8]);
        cp_lines.push_back(v4[10]);
        cp_lines.push_back(v4[8]);
        cp_lines.push_back(v4[11]);

        //world coor 30-35
        cp_lines.push_back(v4[12]);
        cp_lines.push_back(v4[13]);
        cp_lines.push_back(v4[12]);
        cp_lines.push_back(v4[14]);
        cp_lines.push_back(v4[12]);
        cp_lines.push_back(v4[15]);








        //near far clipping       
        for(int i=0;i<cp_lines.size();i+=2){
            bool work = false;

            //near

            float fakenear = -1;
            float fakefar = -50;
            vec4 newp;
            if(cp_lines[i].z > fakenear && cp_lines[i+1].z > fakenear){

                      cp_lines[i][3] = 0;
                      cp_lines[i+1][3] = 0;
                      cp_lines[i][0] = 0;
                      cp_lines[i][1] = 0;
                      cp_lines[i][2] = 0;
                      cp_lines[i+1][0] = 0;
                      cp_lines[i+1][1] = 0;
                      cp_lines[i+1][2] = 0;

                      continue;
            }//end if cp

            newp =  dcut(fakenear, cp_lines[i],cp_lines[i+1], work);
            if(work){
              if(cp_lines[i].z > fakenear){
                 cp_lines[i] = newp;
              }else{
                 cp_lines[i+1] = newp;
              }//end if cp
              nlines.push_back(newp);
            }//end if work            

            //far
            if(cp_lines[i].z < fakefar && cp_lines[i+1].z <fakefar){
                      cp_lines[i][3] = 0;
                      cp_lines[i+1][3] = 0;
                      cp_lines[i][0] = 0;
                      cp_lines[i][1] = 0;
                      cp_lines[i][2] = 0;
                      cp_lines[i+1][0] = 0;
                      cp_lines[i+1][1] = 0;
                      cp_lines[i+1][2] = 0;

                      continue;
            }//end if cp
            work = false;
            newp =  dcut(fakefar, cp_lines[i],cp_lines[i+1], work);
            if(work){
              if(cp_lines[i].z < fakefar){
                 cp_lines[i] = newp;
              }else{
                 cp_lines[i+1] = newp;
              }//end if cp
              flines.push_back(newp);
            }//end if work
 
       }//end for i




        for(int j=0;j<cp_lines.size();++j){
            //projection
            tempvec =  Mats * cp_lines[j];
            cp_lines[j] = ViewPort(vport_w, vport_h, vport_centre_x-vport_w/2, vport_centre_y-vport_h/2, near, far) * tempvec;
            //normalization
            float w = cp_lines[j][3];
            if(0==w) continue;
            cp_lines[j] = cp_lines[j]/w;
        }//end for j
        //normalize v4
        for(int j=0;j<v4.size();++j){
            float w = v4[j][3];
            if(0==w) continue;
            v4[j] = v4[j]/w;
        }//end for j

        //deal with segment

        for(int j=0;j<nlines.size();++j){
            //projection
            tempvec =  Mats * nlines[j];
            nlines[j] = ViewPort(vport_w, vport_h, vport_centre_x-vport_w/2, vport_centre_y-vport_h/2, near, far) * tempvec;
            //normalization
            float w = nlines[j][3];
            if(0==w) continue;
            nlines[j] = nlines[j]/w;
        }//end for j





        /* clipping lines*/
        vector<vec4> llines;
        vector<vec4> rlines;
        vector<vec4> ulines;
        vector<vec4> dlines;


        for(int i=0;i<cp_lines.size();i+=2){
            bool work = false;
            
            if(cp_lines[i][3] == 0) continue;
            //left
            float left = vport_centre_x - vport_w/2;
            vec4 newp;
            if(cp_lines[i].x< left && cp_lines[i+1].x<left){
                      cp_lines[i][3] = 0;
                      cp_lines[i+1][3] = 0;
                      continue;
            }
           
            newp =  vcut(left, cp_lines[i],cp_lines[i+1], work);
            if(work){
              if(cp_lines[i].x < left){
                 cp_lines[i] = newp;
              }else{
                 cp_lines[i+1] = newp;
              }//end if cp
              llines.push_back(newp);
            }//end if work            


            //right
            float right = vport_centre_x + vport_w/2;
            work = false;
            if(cp_lines[i].x> right && cp_lines[i+1].x>right){
                      cp_lines[i][3] = 0;
                      cp_lines[i+1][3] = 0;
                      continue;
            }

            newp =  vcut(right, cp_lines[i],cp_lines[i+1], work);
            if(work){
              if(cp_lines[i].x > right){
                   cp_lines[i] = newp;
              }else{
                   cp_lines[i+1] = newp;
              }//end if cp
              rlines.push_back(newp);
            }//end if work
        
            //down
            float down = vport_centre_y - vport_h/2;
            work = false;
            if(cp_lines[i].y< down && cp_lines[i+1].y<down){
                      cp_lines[i][3] = 0;
                      cp_lines[i+1][3] = 0;
                      continue;
            }

            newp =  hcut(down, cp_lines[i],cp_lines[i+1], work);
            if(work){
              if(cp_lines[i].y < down){
                   cp_lines[i] = newp;
              }else{
                   cp_lines[i+1] = newp;
              }//end if cp
              dlines.push_back(newp);
            }//end if work


            //up
            float up = vport_centre_y + vport_h/2;
            work = false;
            if(cp_lines[i].y> up && cp_lines[i+1].y>up){
                      cp_lines[i][3] = 0;
                      cp_lines[i+1][3] = 0;
                      continue;
            }

            newp =  hcut(up, cp_lines[i],cp_lines[i+1], work);
            if(work){
              if(cp_lines[i].y > up){
                   cp_lines[i] = newp;
              }else{
                   cp_lines[i+1] = newp;
              }//end if cp
              ulines.push_back(newp);
            }//end if work


    
                   
        }//end for i

        initLineData();

        /*draw cubes*/

        setLineColour(vec3(1.0f, 0.7f, 0.8f));
/*
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
*/

        /* draw cube */
        for(int i=0;i<cp_lines.size();i+=2){
            if(0==cp_lines[i][3])continue;
            drawLine(vec2(cp_lines[i]), vec2(cp_lines[i+1]));
 
        }//end for i



        for(int i=1;i<llines.size();i++){
            drawLine(vec2(llines[i-1]), vec2(llines[i]));
        }
        for(int i=1;i<rlines.size();i++){
            drawLine(vec2(rlines[i-1]), vec2(rlines[i]));
        }
        for(int i=1;i<ulines.size();i++){
            drawLine(vec2(ulines[i-1]), vec2(ulines[i]));
        }
        for(int i=1;i<dlines.size();i++){
            drawLine(vec2(dlines[i-1]), vec2(dlines[i]));
        }


        /*draw modelling coordinates*/
        setLineColour(vec3(1.0f, 0.0f, 0.0f));
        drawLine(vec2(cp_lines[24]), vec2(cp_lines[25]));
        setLineColour(vec3(0.0f, 1.0f, 0.0f));
        drawLine(vec2(cp_lines[26]), vec2(cp_lines[27]));
        setLineColour(vec3(0.0f, 0.0f, 0.1f));
        drawLine(vec2(cp_lines[28]), vec2(cp_lines[29]));


        /*draw world coordinates*/
        setLineColour(vec3(0.6f, 0.0f, 0.0f));
        drawLine(vec2(cp_lines[30]), vec2(cp_lines[31]));
        setLineColour(vec3(0.0f, 0.6f, 0.0f));
        drawLine(vec2(cp_lines[32]), vec2(cp_lines[33]));
        setLineColour(vec3(0.0f, 0.0f, 0.6f));
        drawLine(vec2(cp_lines[34]), vec2(cp_lines[35]));




        float dis_x = m_mouse1.x - m_mouse2.x;
        if (dis_x < 0) dis_x = -dis_x;
        float dis_y = m_mouse1.y - m_mouse2.y;
        if (dis_y < 0) dis_y = -dis_y;
        vport_w = dis_x;
        vport_h = dis_y;
        vport_centre_x = (m_mouse1.x + m_mouse2.x)/2;
        vport_centre_y = (m_mouse1.y + m_mouse2.y)/2;



        //draw viewport
        setLineColour(vec3(0.9f, 0.9f, 0.9f));
        drawLine(vec2(vport_centre_x-vport_w/2,vport_centre_y+vport_h/2 ), vec2( vport_centre_x+vport_w/2,vport_centre_y+vport_h/2 ));
        drawLine(vec2(vport_centre_x+vport_w/2,vport_centre_y+vport_h/2 ), vec2( vport_centre_x+vport_w/2,vport_centre_y-vport_h/2 ));
        drawLine(vec2(vport_centre_x+vport_w/2,vport_centre_y-vport_h/2 ), vec2( vport_centre_x-vport_w/2,vport_centre_y-vport_h/2 ));
        drawLine(vec2(vport_centre_x-vport_w/2,vport_centre_y-vport_h/2 ), vec2( vport_centre_x-vport_w/2,vport_centre_y+vport_h/2 ));


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
  vec4 posit = nVTranMat*vec4(0.0f,0.0f,-5.0f,1.0f);
	ImGui::Begin("Properties", &showDebugWindow, ImVec2(100,100), opacity,
			windowFlags);


		// Add more gui elements here here ...
                  



		// Create Button, and check if it was clicked:
		if( ImGui::Button( "Quit Application" ) ) {
			glfwSetWindowShouldClose(m_window, GL_TRUE);
		}
                // Create Button, and check if it was clicked:
                if( ImGui::Button( "Reset Application" ) ) {
                        reset();
                }


		ImGui::Text( "Framerate: %.1f FPS", ImGui::GetIO().Framerate );
         
                ImGui::Text( "mouse: x: %.2f y: %.2f", m_mouse_GL_coordinate.x,m_mouse_GL_coordinate.y );


                ImGui::PushID( 0 );
                if( ImGui::RadioButton( "Rotate View(o)", &shortcut, GLFW_KEY_O ) ) {
                }
                ImGui::PopID();

                ImGui::PushID( 1 );
                if( ImGui::RadioButton( "Translate View(n)", &shortcut, GLFW_KEY_N ) ) {
                }
                ImGui::PopID();


                ImGui::PushID( 2 );
                if( ImGui::RadioButton( "Adjust Perspective(p)", &shortcut, GLFW_KEY_P ) ) {
                }
                ImGui::PopID();

                ImGui::PushID( 3 );
                if( ImGui::RadioButton( "Rotate Model(r)", &shortcut, GLFW_KEY_R ) ) {
                }
                ImGui::PopID();


                ImGui::PushID( 4 );
                if( ImGui::RadioButton( "Translate Model(t)", &shortcut, GLFW_KEY_T ) ) {
                }
                ImGui::PopID();

                ImGui::PushID( 5 );
                if( ImGui::RadioButton( "Scale Model(s)", &shortcut, GLFW_KEY_S ) ) {
                }
                ImGui::PopID();

                ImGui::PushID( 6 );
                if( ImGui::RadioButton( "Draw Viewport(v)", &shortcut, GLFW_KEY_V ) ) {
                }
                ImGui::PopID();


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
        }else if ( GLFW_KEY_T == shortcut){ //model translation
            if (m_mouseLeftAct ) {
                 ntran_x = tran_x + (xPos - mouse_x)*0.1;
            }else{
                 mouse_x = xPos;
            }

            if (m_mouseMidAct) {
                 ntran_y = tran_y +  (mouse_y - yPos)*0.1;
            }else if(m_mouseRightAct){
                 ntran_z = tran_z + (yPos - mouse_y)*0.1;
            }else{
                 mouse_y = yPos;
            }
        }else if( GLFW_KEY_O == shortcut){    //view rotation
            if (m_mouseMidAct ) {
                 nVRotMat = roty(0.1*(xPos - mouse_x)) * VRotMat;
            }else if(m_mouseRightAct){
                 nVRotMat = rotz(0.1*(xPos - mouse_x)) * VRotMat;
            }else{
                 mouse_x = xPos;
            }

            if (m_mouseLeftAct) {
                 nVRotMat = rotx(0.1*(yPos - mouse_y)) * VRotMat;
            }else{
                 mouse_y = yPos;
            }
        }else if( GLFW_KEY_N == shortcut){    //view tranlation
            if (m_mouseLeftAct ) {
                 nVTranMat = trans(0.1*(xPos - mouse_x),0.0f,0.0f) * VTranMat;
            }else{
                 mouse_x = xPos;
            }

            if (m_mouseMidAct) {
                 nVTranMat = trans(0.0f,0.1*(mouse_y - yPos),0.0f) * VTranMat;
            }else if(m_mouseRightAct){
                 nVTranMat = trans(0.0f,0.0f,0.1*(yPos - mouse_y)) * VTranMat;
            }else{
                 mouse_y = yPos;
            }
        }else if( GLFW_KEY_S == shortcut){    //model scale
            if (m_mouseLeftAct ) {
                 nObjScaMat = sca(1.0f + 0.001f*(xPos - mouse_x),1.0f,1.0f) * ObjScaMat;
            }else{
                 mouse_x = xPos;
            }

            if (m_mouseMidAct) {
                 nObjScaMat = sca(1.0f,1.0f +0.001f* (mouse_y - yPos),1.0f) * ObjScaMat;
            }else if(m_mouseRightAct){
                 nObjScaMat = sca(1.0f,1.0f,1.0f + 0.001f*(yPos - mouse_y)) * ObjScaMat;
            }else{
                 mouse_y = yPos;
            }
        }else if( GLFW_KEY_V == shortcut){    //draw view port
            if (m_mouseLeftAct ) {
                 m_mouse2 = m_mouse_GL_coordinate;
            }
            
        }else if( GLFW_KEY_P == shortcut){    //model scale
            if (m_mouseLeftAct) {
                 float offset  = 0.001f*(xPos - mouse_x);
                 if(otheta + offset > 3.1415926f*1.0f/36.0f && otheta + offset < 3.1415926f*8.0f/9.0f){
                        theta =  otheta + offset;
                 }
            }else{
                 mouse_x = xPos;
            }

            if (m_mouseMidAct) {
                 float offset  = 0.001f*(yPos - mouse_y);
                 if(onear + offset > -80 && onear + offset < 1 && onear + offset > far){
                        near =  onear + offset;
                 }

            }else if(m_mouseRightAct){
                 float offset  = 0.001f*(yPos - mouse_y);
                 if(ofar + offset > -80 && ofar + offset < 1 && ofar + offset < near){
                        near =  onear + offset;
                 }

            }else{
                 mouse_y = yPos;
            }
        }



        m_mouse_GL_coordinate = vec2 (
                        (2.0f * xPos) / m_windowWidth - 1.0f,
                        1.0f - ( (2.0f * yPos) / m_windowHeight)
        );



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

        if (actions == GLFW_PRESS) {
                if (!ImGui::IsMouseHoveringAnyWindow()) {
                     if( GLFW_MOUSE_BUTTON_LEFT == button){
                        if(GLFW_KEY_V ==shortcut){
                            m_mouse1 = m_mouse_GL_coordinate;
                        }//end if GLFW
                        m_mouseLeftAct = true;
                        

                     }else if( GLFW_MOUSE_BUTTON_MIDDLE == button){
                        m_mouseMidAct = true;

                     }else if( GLFW_MOUSE_BUTTON_RIGHT == button){
                        m_mouseRightAct = true;

                     }

                }

        }

        if (actions == GLFW_RELEASE) {
                m_mouseLeftAct = false;
                m_mouseRightAct = false;
                m_mouseMidAct = false;
                ObjRotMat = nObjRotMat;
                tran_x = ntran_x;
                tran_y = ntran_y;
                tran_z = ntran_z;
                VRotMat = nVRotMat;
                VTranMat = nVTranMat;
                ObjScaMat = nObjScaMat;       
                otheta = theta;         
                onear = near;
                ofar = far;

        }


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

        if( action == GLFW_PRESS ) {
                // Respond to some key events.

                if (key == GLFW_KEY_Q) {
//                        cout << "Q key pressed" << endl;
                        glfwSetWindowShouldClose(m_window, GL_TRUE);
                        eventHandled = true;
                }else if(key == GLFW_KEY_A){
                        reset();
                }else{
                        shortcut = key;
                }
 
        }else if ( action == GLFW_RELEASE ){
                     eventHandled = true;
        }

	return eventHandled;
}
