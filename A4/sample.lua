-- A simple scene with five spheres

mat1 = gr.material({0.7, 0.5, 0.7}, {0.5, 0.7, 0.5}, 25)
mat2 = gr.material({0.5, 0.5, 0.5}, {0.5, 0.7, 0.5}, 25)
mat3 = gr.material({1.0, 0.6, 0.1}, {0.5, 0.7, 0.5}, 25)
mat4 = gr.material({0.0, 0.2, 0.5}, {0.2, 0.2, 0.2}, 20)


scene_root = gr.node('root')
scene_root:translate(0,0,100)
--scene_root:scale(2,2,1)


back = gr.nh_sphere('b1', {-100, 0, -5000}, 100)
scene_root:add_child(back)
back:set_material(mat4)
back:scale(1,1,0.1)

s1 = gr.nh_sphere('s1', {-160, 0, -400}, 5)
scene_root:add_child(s1)
s1:set_material(mat1)

f2 = gr.nh_sphere('f2', {-140, 0, -400}, 5)
scene_root:add_child(f2)
f2:set_material(mat1)

f3 = gr.nh_sphere('f3', {-120, 0, -400}, 5)
scene_root:add_child(f3)
f3:set_material(mat1)

f4 = gr.nh_sphere('f4', {-100, 0, -400}, 5)
scene_root:add_child(f4)
f4:set_material(mat1)

f5 = gr.nh_sphere('f5', {-140, 30, -400}, 5)
scene_root:add_child(f5)
f5:set_material(mat1)

f6 = gr.nh_sphere('f6', {-130, 50, -400}, 5)
scene_root:add_child(f6)
f6:set_material(mat1)

f7 = gr.nh_sphere('f7', {-120, 70, -400}, 5)
scene_root:add_child(f7)
f7:set_material(mat1)

f8 = gr.nh_sphere('f8', {-120, 20, -400}, 5)
scene_root:add_child(f8)
f8:set_material(mat1)

f9 = gr.nh_sphere('f9', {-120, -20, -400}, 5)
scene_root:add_child(f9)
f9:set_material(mat1)
-----------------------------------------------------
scene_eight = gr.node('eight_root')
scene_root:add_child(scene_eight)

scene_eight:translate(-20,0,0)

e1 = gr.nh_sphere('e1', {0, 30, -400}, 5)
scene_eight:add_child(e1)
e1:set_material(mat2)

e2 = gr.nh_sphere('e2', {-30, 50, -400}, 5)
scene_eight:add_child(e2)
e2:set_material(mat2)

e3 = gr.nh_sphere('e3', {0, 70, -400}, 5)
scene_eight:add_child(e3)
e3:set_material(mat2)

e4 = gr.nh_sphere('e4', {30, 50, -400}, 5)
scene_eight:add_child(e4)
e4:set_material(mat2)

e5 = gr.nh_sphere('e5', {-30, 0, -400}, 5)
scene_eight:add_child(e5)
e5:set_material(mat2)

e6 = gr.nh_sphere('e6', {0, -20, -400}, 5)
scene_eight:add_child(e6)
e6:set_material(mat2)

e7 = gr.nh_sphere('e7', {30, 0, -400}, 5)
scene_eight:add_child(e7)
e7:set_material(mat2)
-------------------------------------------------------

scene_eight2 = gr.node('eight_root2')
scene_root:add_child(scene_eight2)

scene_eight2:translate(85,0,0)

ee1 = gr.nh_sphere('ee1', {0, 30, -400}, 5)
scene_eight2:add_child(ee1)
ee1:set_material(mat3)

ee2 = gr.nh_sphere('ee2', {-30, 50, -400}, 5)
scene_eight2:add_child(ee2)
ee2:set_material(mat3)

ee3 = gr.nh_sphere('ee3', {0, 70, -400}, 5)
scene_eight2:add_child(ee3)
ee3:set_material(mat3)

ee4 = gr.nh_sphere('ee4', {30, 50, -400}, 5)
scene_eight2:add_child(ee4)
ee4:set_material(mat3)

ee5 = gr.nh_sphere('ee5', {-30, 0, -400}, 5)
scene_eight2:add_child(ee5)
ee5:set_material(mat3)

ee6 = gr.nh_sphere('ee6', {0, -20, -400}, 5)
scene_eight2:add_child(ee6)
ee6:set_material(mat3)

ee7 = gr.nh_sphere('ee7', {30, 0, -400}, 5)
scene_eight2:add_child(ee7)
ee7:set_material(mat3)



white_light = gr.light({-300.0, 150.0, 400.0}, {0.9, 0.9, 0.9}, {1, 0, 0})
orange_light = gr.light({-400.0, 100.0, 150.0}, {0.7, 0.0, 0.7}, {1, 0, 0})

gr.render(scene_root, 'simple.png', 256, 256,
	  {0, 0, 800}, {0, 0, -800}, {0, 1, 0}, 50,
	  {0.3, 0.3, 0.3}, {white_light, orange_light})
