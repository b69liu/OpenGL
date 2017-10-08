#include "A1.hpp"
#include "cs488-framework/GlErrorCheck.hpp"

#include <iostream>
#include  <math.h>

#include <imgui/imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;
using namespace std;

static const size_t DIM = 16;
int *board = new int [(DIM*DIM)]; //height for each bar
int c_board[DIM*DIM]; //colour index for each bar; 8 colours in total


//----------------------------------------------------------------------------------------
// Constructor
A1::A1()
	: current_col( 0 ),
          m_mouseButtonActive(false),
          m_shape_translation(vec2(0.0f))
{
	colour[0] = 0.0f;
	colour[1] = 0.0f;
	colour[2] = 0.0f;
}

//----------------------------------------------------------------------------------------
// Destructor
A1::~A1()
{}

//----------------------------------------------------------------------------------------
/*
 * Called once, at program start.
 */
void A1::init()
{
	// Set the background colour.
	glClearColor( 0.3, 0.5, 0.7, 1.0 );

	// Build the shader
	m_shader.generateProgramObject();
	m_shader.attachVertexShader(
		getAssetFilePath( "VertexShader.vs" ).c_str() );
	m_shader.attachFragmentShader(
		getAssetFilePath( "FragmentShader.fs" ).c_str() );
	m_shader.link();

	// Set up the uniforms
	P_uni = m_shader.getUniformLocation( "P" );
	V_uni = m_shader.getUniformLocation( "V" );
	M_uni = m_shader.getUniformLocation( "M" );
	col_uni = m_shader.getUniformLocation( "colour" );

	initGrid();

	// Set up initial view and projection matrices (need to do this here,
	// since it depends on the GLFW window being set up correctly).
	view = glm::lookAt( 
		glm::vec3( 0.0f, float(DIM)*2.0*M_SQRT1_2, float(DIM)*2.0*M_SQRT1_2 ),
		glm::vec3( 0.0f, 0.0f, 0.0f ),
		glm::vec3( 0.0f, 1.0f, 0.0f ) );

	proj = glm::perspective( 
		glm::radians( 45.0f ),
		float( m_framebufferWidth ) / float( m_framebufferHeight ),
		1.0f, 1000.0f );

        //init the board
        for(int i=0;i<DIM*DIM;++i){
            board[i]=0;   //init height
            c_board[i] = 0;  //init colour

            //init cubes
            float *ver = CreateBar(i % DIM, i/DIM, 1);
            glBindBuffer(GL_ARRAY_BUFFER, m_grid_vbo);
            glBufferSubData(GL_ARRAY_BUFFER, (DIM+3)*4*3*4+ i*36*3*sizeof(float) , 108*sizeof(float), ver);
            delete [] ver;
        }

        //init selection
        float *ver = CreateLoop(0, 0, 0);
        glBindBuffer(GL_ARRAY_BUFFER, m_grid_vbo);
        glBufferSubData(GL_ARRAY_BUFFER, (DIM+3)*4*3*4+ DIM*DIM*36*3*sizeof(float) , 4*3*sizeof(float), ver);
        delete [] ver;
 
        //set the radio colours
        ResetColours();


}

void A1::initGrid()
{
	size_t sz = 3 * 2 * 2 * (DIM+3);

	float *verts = new float[ sz ];
	size_t ct = 0;
	for( int idx = 0; idx < DIM+3; ++idx ) {
		verts[ ct ] = -1;
		verts[ ct+1 ] = 0;
		verts[ ct+2 ] = idx-1;
		verts[ ct+3 ] = DIM+1;
		verts[ ct+4 ] = 0;
		verts[ ct+5 ] = idx-1;
		ct += 6;

		verts[ ct ] = idx-1;
		verts[ ct+1 ] = 0;
		verts[ ct+2 ] = -1;
		verts[ ct+3 ] = idx-1;
		verts[ ct+4 ] = 0;
		verts[ ct+5 ] = DIM+1;
		ct += 6;
	}

	// Create the vertex array to record buffer assignments.
	glGenVertexArrays( 1, &m_grid_vao );
	glBindVertexArray( m_grid_vao );

	// Create the cube vertex buffer
	glGenBuffers( 1, &m_grid_vbo );
	glBindBuffer( GL_ARRAY_BUFFER, m_grid_vbo );
//	glBufferData( GL_ARRAY_BUFFER, sz*sizeof(float),
//		verts, GL_STATIC_DRAW );

        glBufferData( GL_ARRAY_BUFFER, (sz+DIM*DIM*36*3+6*3)*sizeof(float),
                verts, GL_STATIC_DRAW );

	// Specify the means of extracting the position values properly.
	GLint posAttrib = m_shader.getAttribLocation( "position" );
	glEnableVertexAttribArray( posAttrib );
	glVertexAttribPointer( posAttrib, 3, GL_FLOAT, GL_FALSE, 0, nullptr );

	// Reset state to prevent rogue code from messing with *my* 
	// stuff!
	glBindVertexArray( 0 );
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

	// OpenGL has the buffer now, there's no need for us to keep a copy.
	delete [] verts;

	CHECK_GL_ERRORS;
}



// create a six-faces bar
// directly replace it with the old data in buffer
float* A1::CreateBar(int x, int z, int h){
       int pnum = 36;
       float* verts = new  float[ 3 * pnum  ];
       verts[0] = x;  //up
       verts[1] = h;
       verts[2] = z;

       verts[3] = x+1;
       verts[4] = h;
       verts[5] = z;

       verts[6] = x;
       verts[7] = h;
       verts[8] = z+1;

       verts[9] = x+1;
       verts[10] = h;
       verts[11] = z;

       verts[12] = x;
       verts[13] = h;
       verts[14] = z+1;

       verts[15] = x+1;
       verts[16] = h;
       verts[17] = z+1;

       verts[18] = x; // Down
       verts[19] = 0;
       verts[20] = z;

       verts[21] = x+1;
       verts[22] = 0;
       verts[23] = z;

       verts[24] = x;
       verts[25] = 0;
       verts[26] = z+1;

       verts[27] = x+1; 
       verts[28] = 0;
       verts[29] = z;
       verts[30] = x;
       verts[31] = 0;
       verts[32] = z+1;

       verts[33] = x+1;
       verts[34] = 0;
       verts[35] = z+1;

       verts[36] = x; // Left
       verts[37] = h;
       verts[38] = z;

       verts[39] = x;
       verts[40] = 0;
       verts[41] = z;

       verts[42] = x;
       verts[43] = h;
       verts[44] = z+1;

       verts[45] = x;
       verts[46] = 0;
       verts[47] = z;

       verts[48] = x;
       verts[49] = h;
       verts[50] = z+1;

       verts[51] = x;
       verts[52] = 0;
       verts[53] = z+1;

       verts[54] = x+1; //Right
       verts[55] = h;
       verts[56] = z;

       verts[57] = x+1;
       verts[58] = 0;
       verts[59] = z;

       verts[60] = x+1;
       verts[61] = h;
       verts[62] = z+1;

       verts[63] = x+1; 
       verts[64] = 0;
       verts[65] = z;

       verts[66] = x+1;
       verts[67] = h;
       verts[68] = z+1;

       verts[69] = x+1;
       verts[70] = 0;
       verts[71] = z+1;

       verts[72] = x;//Front
       verts[73] = h;
       verts[74] = z+1;

       verts[75] = x;
       verts[76] = 0;
       verts[77] = z+1;

       verts[78] = x+1;
       verts[79] = h;
       verts[80] = z+1;

       verts[81] = x; 
       verts[82] = 0;
       verts[83] = z+1;

       verts[84] = x+1;
       verts[85] = h;
       verts[86] = z+1;

       verts[87] = x+1;
       verts[88] = 0;
       verts[89] = z+1;

       verts[90] = x; //Back
       verts[91] = h;
       verts[92] = z;

       verts[93] = x;
       verts[94] = 0;
       verts[95] = z;

       verts[96] = x+1;
       verts[97] = h;
       verts[98] = z;

       verts[99] = x; 
       verts[100] = 0;
       verts[101] = z;

       verts[102] = x+1;
       verts[103] = h;
       verts[104] = z;

       verts[105] = x+1;
       verts[106] = 0;
       verts[107] = z;

       return verts;
}



// create a four points lineloop
// directly replace it with the old data in buffer
float* A1::CreateLoop(int x, int z, int h){
       int pnum = 4;
       float* verts = new  float[ 3 * pnum  ];
       verts[0] = 0;  
       verts[1] = 0;
       verts[2] = 0;

       verts[3] = 1;
       verts[4] = 0;
       verts[5] = 1;

       verts[6] = 1;
       verts[7] = 0;
       verts[8] = 0;

       verts[9] = 0;
       verts[10] = 0;
       verts[11] = 1;

       return verts;
}


void A1::ResetColours(){
        for(float f=0.0f; f< 0.8f;f=f+0.1f){
           int i = f*10;
           colours[i][0] = f + 0.1f;
           colours[i][1] = (f + 0.2f) * 0.5f;
           colours[i][2] = 1.0f - f ;
        }
        colours[0][0] = 1.0f;
        colours[0][1] = 1.0f;
        colours[0][2] = 1.0f;


};



//----------------------------------------------------------------------------------------
/*
 * Called once per frame, before guiLogic().
 */
void A1::appLogic()
{
	// Place per frame, application logic here ...
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, after appLogic(), but before the draw() method.
 */
void A1::guiLogic()
{
	// We already know there's only going to be one window, so for 
	// simplicity we'll store button states in static local variables.
	// If there was ever a possibility of having multiple instances of
	// A1 running simultaneously, this would break; you'd want to make
	// this into instance fields of A1.
	static bool showTestWindow(false);
	static bool showDebugWindow(true);

	ImGuiWindowFlags windowFlags(ImGuiWindowFlags_AlwaysAutoResize);
	float opacity(0.5f);

	ImGui::Begin("Debug Window", &showDebugWindow, ImVec2(100,100), opacity, windowFlags);
		if( ImGui::Button( "Quit Application" ) ) {
			glfwSetWindowShouldClose(m_window, GL_TRUE);
		}


                if( ImGui::Button( "Reset" ) ) {

                        for(int i=0;i<DIM*DIM;++i){
                             board[i]=0;
                             c_board[i] = 0;
                        }//end for
                        ResetColours();
                        nrot = 0;
                        rot = 0;
                        sel_x = 0;
                        sel_z = 0;
                        m_shape_size = 1.0;


                }



		// Eventually you'll create multiple colour widgets with
		// radio buttons.  If you use PushID/PopID to give them all
		// unique IDs, then ImGui will be able to keep them separate.
		// This is unnecessary with a single colour selector and
		// radio button, but I'm leaving it in as an example.

		// Prefixing a widget name with "##" keeps it from being
		// displayed.

		ImGui::PushID( 0 );
		ImGui::ColorEdit3( "##Colour", colours[current_col] );
	//	ImGui::SameLine();




		if( ImGui::RadioButton( "##Col", &current_col, 0 ) ) {
                        c_board[DIM*sel_z + sel_x] = 0;
			// Select this colour.
		}
                ImGui::PopID();
                ImGui::SameLine();
                ImGui::PushID( 1 );
                if( ImGui::RadioButton( "##Col", &current_col, 1 ) ) {
                        c_board[DIM*sel_z + sel_x] = 1;
                        // Select this colour.
                }


		ImGui::PopID();
                ImGui::SameLine();

                ImGui::PushID( 2 );
                if( ImGui::RadioButton( "##Col", &current_col, 2 ) ) {
                        c_board[DIM*sel_z + sel_x] = 2;
                        // Select this colour.
                }


                ImGui::PopID();
                ImGui::SameLine();
                ImGui::PushID( 3 );
                if( ImGui::RadioButton( "##Col", &current_col, 3 ) ) {
                        c_board[DIM*sel_z + sel_x] = 3;
                        // Select this colour.
                }


                ImGui::PopID();
                ImGui::SameLine();
                ImGui::PushID( 4 );
                if( ImGui::RadioButton( "##Col", &current_col, 4 ) ) {
                        c_board[DIM*sel_z + sel_x] = 4;
                        // Select this colour.
                }


                ImGui::PopID();
                ImGui::SameLine();
                ImGui::PushID( 5 );
                if( ImGui::RadioButton( "##Col", &current_col, 5 ) ) {
                        c_board[DIM*sel_z + sel_x] = 5;
                        // Select this colour.
                }


                ImGui::PopID();
                ImGui::SameLine();
                ImGui::PushID( 6 );
                if( ImGui::RadioButton( "##Col", &current_col, 6 ) ) {
                        c_board[DIM*sel_z + sel_x] = 6;
                        // Select this colour.
                }


                ImGui::PopID();
                ImGui::SameLine();
                ImGui::PushID( 7 );
                if( ImGui::RadioButton( "##Col", &current_col, 7 ) ) {
                        c_board[DIM*sel_z + sel_x] = 7;
                        // Select this colour.
                }


                ImGui::PopID();

/*
		// For convenience, you can uncomment this to show ImGui's massive
		// demonstration window right in your application.  Very handy for
		// browsing around to get the widget you want.  Then look in 
		// shared/imgui/imgui_demo.cpp to see how it's done.
		if( ImGui::Button( "Test Window" ) ) {
			showTestWindow = !showTestWindow;
		}
*/



                ImGui::SliderFloat("Red Channel", &colours[current_col][0], 0.0f, 1.0f);
                ImGui::SliderFloat("Green Channel", &colours[current_col][1], 0.0f, 1.0f);
                ImGui::SliderFloat("Blue Channel", &colours[current_col][2], 0.0f, 1.0f);


		ImGui::Text( "Framerate: %.1f FPS", ImGui::GetIO().Framerate );

	ImGui::End();

	if( showTestWindow ) {
		ImGui::ShowTestWindow( &showTestWindow );
	}
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, after guiLogic().
 */
void A1::draw()
{
	// Create a global transformation for the model (centre it).
	mat4 W;
//	W = glm::translate( W, vec3( -float(DIM)/2.0f, 0, -float(DIM)/2.0f ) );
     
vec3 y_axis(0.0f,1.0f,0.0f);
 W *= glm::rotate(mat4(), ((float) (nrot / 100)), y_axis);
 W *= glm::scale(mat4(), vec3(m_shape_size));
W = glm::translate( W, vec3( -float(DIM)/2.0f, 0, -float(DIM)/2.0f ) );
 


	m_shader.enable();
		glEnable( GL_DEPTH_TEST );



                vec3 z_axis(0.0f,0.0f,1.0f);


		glUniformMatrix4fv( P_uni, 1, GL_FALSE, value_ptr( proj ) );
		glUniformMatrix4fv( V_uni, 1, GL_FALSE, value_ptr( view ) );
		glUniformMatrix4fv( M_uni, 1, GL_FALSE, value_ptr( W ) );





		// Just draw the grid for now.
		glBindVertexArray( m_grid_vao );




		glUniform3f( col_uni, 1, 1, 1 );

		glDrawArrays( GL_LINES, 0, (3+DIM)*4 );                 
//                glUniform3f(col_uni, colour[0], colour[1],
//                                colour[2]);


                //draw cubes
                for(int i=0;i<DIM*DIM;++i){
                    if(board[i] <= 0){ continue;}
                    glUniformMatrix4fv( M_uni, 1, GL_FALSE, value_ptr( W * glm::scale(mat4(), vec3(1,board[i],1)) ) );
                    int ci = c_board[i];
                    glUniform3f(col_uni, colours[ci][0],colours[ci][1],
                                colours[ci][2]);
                    glDrawArrays( GL_TRIANGLES, (3+DIM)*4+i*36, 36);
                }




glDisable( GL_DEPTH_TEST );


                //draw selection loop
                glUniform3f( col_uni, 0.5, 1, 1 );
                glUniformMatrix4fv( M_uni, 1, GL_FALSE, 
                    value_ptr( W * glm::translate(mat4(), vec3(sel_x,board[DIM*sel_z + sel_x],sel_z)) ) );
                glDrawArrays(  GL_LINE_LOOP, (3+DIM)*4+(DIM*DIM)*36, 4);




		// Draw the cubes
		// Highlight the active square.
	m_shader.disable();

	// Restore defaults
	glBindVertexArray( 0 );

	CHECK_GL_ERRORS;
}

//----------------------------------------------------------------------------------------
/*
 * Called once, after program is signaled to terminate.
 */
void A1::cleanup()
{}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles cursor entering the window area events.
 */
bool A1::cursorEnterWindowEvent (
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
bool A1::mouseMoveEvent(double xPos, double yPos) 
{
	bool eventHandled(false);

	if (!ImGui::IsMouseHoveringAnyWindow()) {
		// Put some code here to handle rotations.  Probably need to
		// check whether we're *dragging*, not just moving the mouse.
		// Probably need some instance variables to track the current
		// rotation amount, and maybe the previous X position (so 
		// that you can rotate relative to the *change* in X.

	}
        if (m_mouseButtonActive) {
             nrot = rot +  xPos - mouse_x;
        }else{
             mouse_x = xPos;
        }


	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse button events.
 */
bool A1::mouseButtonInputEvent(int button, int actions, int mods) {
	bool eventHandled(false);

	if (!ImGui::IsMouseHoveringAnyWindow()) {
		// The user clicked in the window.  If it's the left
		// mouse button, initiate a rotation.
	}

        if (actions == GLFW_PRESS) {
                if (!ImGui::IsMouseHoveringAnyWindow()) {
                        
                        m_mouseButtonActive = true;
                }
        }

        if (actions == GLFW_RELEASE) {
                m_mouseButtonActive = false;
                rot = nrot;
        }



	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse scroll wheel events.
 */
bool A1::mouseScrollEvent(double xOffSet, double yOffSet) {
	bool eventHandled(false);

	// Zoom in or out.
        if(yOffSet>0){
          if(m_shape_size < 2){
             m_shape_size += 0.2;
          }

        }else{
          if(m_shape_size > 0.5){
             m_shape_size -= 0.2;
          }

        }
	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles window resize events.
 */
bool A1::windowResizeEvent(int width, int height) {
	bool eventHandled(false);

	// Fill in with event handling code...

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles key input events.
 */
bool A1::keyInputEvent(int key, int action, int mods) {
	bool eventHandled(false);

	// Fill in with event handling code...
	if( action == GLFW_PRESS ) {
		// Respond to some key events.

                if (key == GLFW_KEY_Q) {
//                        cout << "Q key pressed" << endl;
                        glfwSetWindowShouldClose(m_window, GL_TRUE);
                        eventHandled = true;
                }
                if (key == GLFW_KEY_R) {
//                        cout << "R key pressed" << endl;
                       
                        for(int i=0;i<DIM*DIM;++i){
                             board[i]=0;
                             c_board[i] = 0;
                        }//end for
                        nrot = 0;
                        rot = 0;
                        ResetColours();
                        sel_x = 0;
                        sel_z = 0; 
                        m_shape_size = 1.0;




                        eventHandled = true;
                }

                if (key == GLFW_KEY_LEFT) {
//                        cout << "LEFT key pressed" << endl;
                        if(sel_x > 0){
                             if(shiftActive){
                                int offset = sel_z * DIM + (sel_x-1);
                                board[offset] = board[DIM*sel_z+sel_x];
                                c_board[offset] = c_board[DIM*sel_z+sel_x];
                             }
                             --sel_x;
                        }
                        eventHandled = true;
                }

                if (key == GLFW_KEY_RIGHT) {
//                        cout << "RIGHT key pressed" << endl;
                        if(sel_x <15){
                             if(shiftActive){
                                int offset = sel_z * DIM + (sel_x+1);
                                board[offset] = board[DIM*sel_z+sel_x];
                                c_board[offset] = c_board[DIM*sel_z+sel_x];
                             }
                             ++sel_x;
                        }

                       
                        eventHandled = true;
                }
                if (key == GLFW_KEY_UP) {
//                        cout << "UP key pressed" << endl;
                        if(sel_z > 0){
                             if(shiftActive){
                                int offset = (sel_z-1) * DIM + sel_x;
                                board[offset] = board[DIM*sel_z+sel_x];
                                c_board[offset] = c_board[DIM*sel_z+sel_x];
                             }
                             --sel_z;


                        }//end if sel
                        eventHandled = true;
                }
                if (key == GLFW_KEY_DOWN) {
//                        cout << "DOWN key pressed" << endl;
                        if(sel_z < 15){

                             if(shiftActive){
                                int offset = (sel_z+1) * DIM + sel_x;
                                board[offset] = board[DIM*sel_z+sel_x];
                                c_board[offset] = c_board[DIM*sel_z+sel_x];
                             }
                             ++sel_z;
                        }
                        eventHandled = true;
                }
                if (key == GLFW_KEY_SPACE) {
//                        cout << "space key pressed" << endl;
                       int offset = sel_z * DIM + sel_x;
                       ++board[offset];
                       if(1==board[offset]){  //if generate a new bar
/*                         float *ver = CreateBar(sel_x, sel_z, board[offset]);
                         glBindBuffer(GL_ARRAY_BUFFER, m_grid_vbo);
                         glBufferSubData(GL_ARRAY_BUFFER, 912+ offset*36*3*sizeof(float) , 108*sizeof(float), ver);
                         delete [] ver;
*/
                           c_board[sel_z*DIM + sel_x] = current_col;
                       }//end if 1=
                        eventHandled = true;
                }

                if (key == GLFW_KEY_BACKSPACE) {
//                        cout << "backspace key pressed" << endl;
                       int offset = sel_z * DIM + sel_x;
                       int cury = board[offset];
                       if(cury > 0){
                           --board[offset];
//                           --cury;
//                           float *ver = CreateBar(sel_x, sel_z, cury);
//                           glBindBuffer(GL_ARRAY_BUFFER, m_grid_vbo);
//                           glBufferSubData(GL_ARRAY_BUFFER, 912+ offset*36*3*sizeof(float) , 108*sizeof(float), ver);
//                           delete [] ver;
                       }
                        eventHandled = true;
                }

                if(key == GLFW_KEY_LEFT_SHIFT ||
                   key == GLFW_KEY_RIGHT_SHIFT){
                       shiftActive = true;
//                       cout<<"Pressing shift"<<endl;
                       eventHandled = true;
                }



	}else if ( action == GLFW_RELEASE ){
                if(key == GLFW_KEY_LEFT_SHIFT ||
                   key == GLFW_KEY_RIGHT_SHIFT){
                     shiftActive = false;
//                     cout<<"Released shift"<<endl;
                     eventHandled = true;
                }//end if k=g
        }

	return eventHandled;
}
