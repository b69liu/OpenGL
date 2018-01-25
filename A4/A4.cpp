#include <glm/ext.hpp>
#include <vector>
#include "GeometryNode.hpp"
#include "polyroots.hpp"
#include "PhongMaterial.hpp"

#include "A4.hpp"

using namespace std;
using namespace glm;

void A4_Render(
		// What to render
		SceneNode * root,

		// Image to write to, set to a given width and height
		Image & image,

		// Viewing parameters
		const glm::vec3 & eye,
		const glm::vec3 & view,
		const glm::vec3 & up,
		double fovy,

		// Lighting parameters
		const glm::vec3 & ambient,
		const list<Light *> & lights
) {

  // Fill in raytracing code here...

      cout << "Calling A4_Render(\n" <<
		  "\t" << *root <<
          "\t" << "Image(width:" << image.width() << ", height:" << image.height() << ")\n"
          "\t" << "eye:  " << glm::to_string(eye) << endl <<
		  "\t" << "view: " << glm::to_string(view) << endl <<
		  "\t" << "up:   " << glm::to_string(up) << endl <<
		  "\t" << "fovy: " << fovy << endl <<
          "\t" << "ambient: " << glm::to_string(ambient) << endl <<
		  "\t" << "lights{" << endl;

	for(const Light * light : lights) {
		cout << "\t\t" <<  *light << endl;
	}
	cout << "\t}" << endl;
        cout <<")" << endl;

	size_t h = image.height();
	size_t w = image.width();
        
        mat4 father_tans = mat4(1,0,0,0,
                                0,1,0,0,
                                0,0,1,0,
                                0,0,0,1);


int nodnum = 0;
	for (uint y = 0; y < h; ++y) {
		for (uint x = 0; x < w; ++x) {
			// Red: increasing from top to bottom
                        if(x%30 == y%30 || y%30 == -x%30){
		     	   image(x, y, 0) = 0.1; //(double)y / h;
                           image(x, y, 1) = 0.2;
                           image(x, y, 2) = 0.3;
                        }else{
                           image(x, y, 0) = 0.6; //(double)y / h;
                           image(x, y, 1) = 0.7;
                           image(x, y, 2) = 0.8;
                        }
			// Green: increasing from left to right
			//image(x, y, 1) = (double)x / w;
			// Blue: in lower-left and upper-right corners
			//image(x, y, 2) = ((y < h/2 && x < w/2)
			//			  || (y >= h/2 && x >= w/2)) ? 1.0 : 0.0;
                        glm::vec4 intersect = glm::vec4(0,0,-99999,1);
                        double midx = w/2;
                        double midy = h/2;
                        double xp,yp,zp;
                        xp = (double)x - midx;
                        yp = (double) (h-y)-midy;
                        zp = 0.0;

                        double xE,yE,zE;
                        xE = eye.x;
                        yE = eye.y;
                        zE = eye.z;

                        
                        SceneNode * st_ptr;
                        vector<SceneNode *> stack;
                        stack.push_back(root);
                        while(!stack.empty()){
                           st_ptr = stack.back();
                           stack.pop_back();

                           father_tans = st_ptr->father_trans * st_ptr->get_transform();
                  
                           //add children
                           for(SceneNode * ch : st_ptr->children){
                               ch->father_trans = father_tans;
                               stack.push_back(ch);
                           }//end for ch
                           if (st_ptr->m_nodeType != NodeType::GeometryNode)
                              continue;
                           const GeometryNode * geometryNode = static_cast<const GeometryNode *>(st_ptr);
                           //assume everything is nosphere
                           NonhierSphere * nsp = static_cast<NonhierSphere *>(geometryNode->m_primitive);
                           glm::vec3 centre = vec3(nsp->getpos());
                           double r = nsp->getrad();
                           double t[2];
                           double a,b,c;
                           a = r*father_tans[0][0];
                           b = r*father_tans[1][1];
                           c = r*father_tans[2][2];
                           vec4 ct = father_tans*vec4(centre,1);
                           centre = vec3(ct);

			   int rnum =  quadraticRoots(
                               (xp*xp + xE*xE -2*xp*xE)/(a*a) + 
                               (yp*yp + yE*yE -2*yp*yE)/(b*b) + 
                               (zp*zp + zE*zE -2*zp*zE)/(c*c), 

                               
                               2*(xp-xE)*(xE-centre.x)/(a*a) +
                               2*(yp-yE)*(yE-centre.y)/(b*b) +
                               2*(zp-zE)*(zE-centre.z)/(c*c),

                               (xE*xE + (centre.x) * (centre.x) -2*(centre.x) * xE)/(a*a) +
                               (yE*yE + (centre.y) * (centre.y) -2*((centre.y)) * yE)/(b*b) +
                               (zE*zE + (centre.z) * (centre.z) -2*(centre.z) * zE)/(c*c) - 1 , t);



                           double td;
                           if(1 == rnum){
                               td = t[0];
                           }else if(2 == rnum){
                               td = t[0];
                               if(td > t[1]){td = t[1];}
                           }else{
                               continue;                 //not hit anything
                           }

                           //test if the object is behind
                           if(intersect.z > zE + td*(zp - zE)) continue;
                           intersect.x = xE + td*(xp - xE);
                           intersect.y = yE + td*(yp - yE);
                           intersect.z = zE + td*(zp - zE);
                           double colour_r,colour_g,colour_b;
                           //set it to ambient colour with the amt ref coe
                           double ka = 0.3;
                           colour_r = ambient[0] * ka;
                           colour_g = ambient[1] * ka;
                           colour_b = ambient[2] * ka;
//cout <<"background r="<<colour_r<<" g="<<colour_g<<" b="<<colour_b<<" (" <<x<<","<<y<<")" <<endl;


                           vec4 norm = intersect - vec4(centre,1);
                           //double norm_len = sqrt(norm[0]*norm[0]+norm[1]*norm[1]+norm[2]*norm[2]);
                           //norm[0] = norm[0]/norm_len;
                           //norm[1] = norm[1]/norm_len;
                           //norm[2] = norm[2]/norm_len;
                           norm = normalize(norm);
                           
                           vec4 v = vec4(eye,0)-intersect;
                           v = normalize(v);
                           double p = ((PhongMaterial*) geometryNode->m_material)->getp();

	                   for(Light * each_light: lights){
                              
                               vec4 l = vec4(each_light->position,0) - intersect;
                               //shadow
//------------------------------------------------------------------------------------                               
        mat4 sfather_tans = mat4(1,0,0,0,
                                0,1,0,0,
                                0,0,1,0,
                                0,0,0,1);


                        bool blocked = false;  //whether the light is blicked
                        SceneNode * sst_ptr;
                        vector<SceneNode *> sstack;
                        sstack.push_back(root);
                        while(!sstack.empty()){
                           sst_ptr = sstack.back();
                           sstack.pop_back();
                           sfather_tans = sst_ptr->sfather_trans * sst_ptr->get_transform();

                           //add children
                           for(SceneNode * ch : sst_ptr->children){
                               ch->sfather_trans = sfather_tans;
                               sstack.push_back(ch);
                           }//end for ch
                           if (sst_ptr->m_nodeType != NodeType::GeometryNode)
                              continue;
                           const GeometryNode * sgeometryNode = static_cast<const GeometryNode *>(sst_ptr);

                           //assume everything is nosphere
                           NonhierSphere * snsp = static_cast<NonhierSphere *>(sgeometryNode->m_primitive);
                           glm::vec3 scentre = vec3(snsp->getpos());
                           double sr = snsp->getrad();  //r for tested object

                           double st[2];
                           double sa,sb,sc;
                           sa = sr;
                           sb = sr;
                           sc = sr;
                           sa = sr*sfather_tans[0][0];
                           sb = sr*sfather_tans[1][1];
                           sc = sr*sfather_tans[2][2];

                           vec4 sct = sfather_tans*vec4(scentre,1);
                           scentre = vec3(sct);
                           double sxp,syp,szp;
                           sxp = each_light->position.x;
                           syp = each_light->position.y;
                           szp = each_light->position.z;
                           double sxE,syE,szE;
                           sxE = intersect.x;
                           syE = intersect.y;
                           szE = intersect.z;




                           int srnum =  quadraticRoots(
                               (sxp*sxp + sxE*sxE -2*sxp*sxE)/(sa*sa) +
                               (syp*syp + syE*syE -2*syp*syE)/(sb*sb) +
                               (szp*szp + szE*szE -2*szp*szE)/(sc*sc),


                               2*(sxp-sxE)*(sxE-scentre.x)/(sa*sa) +
                               2*(syp-syE)*(syE-scentre.y)/(sb*sb) +
                               2*(szp-szE)*(szE-scentre.z)/(sc*sc),

                               (sxE*sxE + (scentre.x) * (scentre.x) -2*(scentre.x) * sxE)/(sa*sa) +
                               (syE*syE + (scentre.y) * (scentre.y) -2*(scentre.y) * syE)/(sb*sb) +
                               (szE*szE + (scentre.z) * (scentre.z) -2*(scentre.z) * szE)/(sc*sc) - 1 , st);



                           if(1==srnum){
                               if(st[0] < 1 && st[0]>0.001) blocked =true;
                           }else if(2==srnum){
                               if((st[0] <1 && st[0]>0.001 )|| (st[1]<1 && st[1]>0.001)) blocked =true;
                           }//end if srnum
                        }//end shadow while




                            if(blocked){
                                   continue;   //end loop to jump to next light
                            }//end if blocked








//------------------------------------------------------------------------------------------------


                               l = normalize(l);

                               vec4 r = -l + 2*dot(l,norm)*norm;
                               r = normalize(r);

                               vec3 mkd = ((PhongMaterial*) geometryNode->m_material)->getkd();
                               vec3 mks = ((PhongMaterial*) geometryNode->m_material)->getks();
                               
//cout << " kd = "<<mkd[0] <<" dotln="<<dot(l,norm) <<" r="<<each_light->colour.r <<endl;
//cout << "norm=" <<norm[0] <<""<<norm[1]<<""<<norm[2]<<""<<norm[3] <<endl;
                               vec3 rtho = mkd + mks*pow(dot(r,v),p)/dot(norm,l);

                               double specular_r = mks[0]*pow(dot(r,v),p)*each_light->colour.r;
                               if(specular_r<0.0){specular_r = 0.0;}
                               double diffusion_r = mkd[0]*dot(l,norm)*each_light->colour.r;
                               if(diffusion_r<0.0){diffusion_r=0.0;}
                               colour_r += specular_r + diffusion_r;
                               
                               double specular_g = mks[1]*pow(dot(r,v),p)*each_light->colour.g;
                               if(specular_g<0.0){specular_g = 0.0;}
                               double diffusion_g = mkd[1]*dot(l,norm)*each_light->colour.g;
                               if(diffusion_g<0.0){diffusion_g=0.0;}
                               colour_g += specular_g + diffusion_g;

                               double specular_b = mks[2]*pow(dot(r,v),p)*each_light->colour.b;
                               if(specular_b<0.0){specular_b = 0.0;}
                               double diffusion_b = mkd[2]*dot(l,norm)*each_light->colour.b;
                               if(diffusion_b<0.0){diffusion_b=0.0;}
                               colour_b += specular_b + diffusion_b;




                           }//end for each light
                           



if(rnum > 0){

image(x, y, 0) = colour_r;
image(x, y, 1) = colour_g;
image(x, y, 2) = colour_b;
}
                        }//end while st
		}//end for x
	}//end for yi
}
