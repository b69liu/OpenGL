rootNode = gr.node('root')
rootNode:translate(0.0,0.0,-5);

doc = gr.mesh('cube', 'name-of-cube1')
doc:scale(0.7, 1.0, 0.7)
doc:rotate('y', 50.0)
doc:translate(0.0, 0.0, 0.0)
doc:set_material(gr.material({0.8, 0.2, 0.2}, {0.8, 0.8, 0.8}, 10.0))
rootNode:add_child(doc)

cou_joint = gr.joint("neck-joint", {-50, 31, 50}, {-50,50, 50})
cou_joint:translate(0.0, 0.4, 0.0)
doc:add_child(cou_joint)




cou = gr.mesh('cube','cou')
cou:scale(0.2, 0.2, 0.2)
cou:rotate('y', 0.0)
cou:translate(0.0, 0.2, 0.0)
cou:set_material(gr.material({0.8, 0.2, 0.2}, {0.8, 0.8, 0.8}, 10.0))
cou_joint:add_child(cou)

tete = gr.mesh('head','tete')
tete:scale(0.5, 0.5, 0.5)
tete:rotate('y', 0.0)
tete:translate(0.0, 1.5, 0.0)
tete:set_material(gr.material({0.8, 0.2, 0.2}, {0.8, 0.8, 0.8}, 10.0))
cou:add_child(tete)



epaule_g = gr.mesh('sphere', 'ep-gauche')
epaule_g:scale(0.5,0.1,0.2)
epaule_g:rotate('x',0)
epaule_g:translate(-1.0,0.45,0.0);
epaule_g:set_material(gr.material({0.8, 0.2, 0.2}, {0.8, 0.8, 0.8}, 10.0))
doc:add_child(epaule_g)

bras_g = gr.mesh('sphere','br-gauche')
bras_g:scale(0.8,0.9,0.9)
bras_g:rotate('x',0)
bras_g:translate(-1.5,0.0,0.0)
bras_g:set_material(gr.material({0.8, 0.2, 0.2}, {0.8, 0.8, 0.8}, 10.0))
epaule_g:add_child(bras_g)


main_g = gr.mesh('sphere','main-gauche')
main_g:scale(0.5,0.3,0.8)
main_g:rotate('x',0)
main_g:translate(-1.2,0.0,0.0)
main_g:set_material(gr.material({0.8, 0.2, 0.2}, {0.8, 0.8, 0.8}, 10.0))
bras_g:add_child(main_g)




epaule_d = gr.mesh('sphere', 'ep-droit')
epaule_d:scale(0.5,0.1,0.2)
epaule_d:rotate('x',0)
epaule_d:translate(1.0,0.45,0.0);
epaule_d:set_material(gr.material({0.8, 0.2, 0.2}, {0.8, 0.8, 0.8}, 10.0))

doc:add_child(epaule_d)

bras_d = gr.mesh('sphere','br-droit')
bras_d:scale(0.8,0.9,0.9)
bras_d:rotate('x',0)
bras_d:translate(1.5,0.0,0.0)
bras_d:set_material(gr.material({0.8, 0.2, 0.2}, {0.8, 0.8, 0.8}, 10.0))
epaule_d:add_child(bras_d)

main_d = gr.mesh('sphere','main-droit')
main_d:scale(0.5,0.3,0.8)
main_d:rotate('x',0)
main_d:translate(1.2,0.0,0.0)
main_d:set_material(gr.material({0.8, 0.2, 0.2}, {0.8, 0.8, 0.8}, 10.0))
bras_d:add_child(main_d)

-------------
cuisse_g = gr.mesh('sphere', 'cuisse-gauche')
cuisse_g:scale(0.16,0.4,0.16)
cuisse_g:rotate('x',0)
cuisse_g:translate(-0.36,-0.9,0.0);
cuisse_g:set_material(gr.material({0.8, 0.2, 0.2}, {0.8, 0.8, 0.8}, 10.0))
doc:add_child(cuisse_g)

jambe_g = gr.mesh('sphere', 'jambe-gauche')
jambe_g:scale(0.9,1.1,0.9)
jambe_g:rotate('x',0)
jambe_g:translate(0.0,-1.25,0.0);
jambe_g:set_material(gr.material({0.8, 0.2, 0.2}, {0.8, 0.8, 0.8}, 10.0))
cuisse_g:add_child(jambe_g)

pied_g = gr.mesh('sphere', 'pied-gauche')
pied_g:scale(1.0,0.1,3.0)
pied_g:rotate('x',0)
pied_g:translate(0.0,-1.1,1.0);
pied_g:set_material(gr.material({0.8, 0.2, 0.2}, {0.8, 0.8, 0.8}, 10.0))
jambe_g:add_child(pied_g)



----

cuisse_d = gr.mesh('sphere', 'cuisse-droit')
cuisse_d:scale(0.16,0.4,0.16)
cuisse_d:rotate('x',0)
cuisse_d:translate(0.36,-0.9,0.0);
cuisse_d:set_material(gr.material({0.8, 0.2, 0.2}, {0.8, 0.8, 0.8}, 10.0))
doc:add_child(cuisse_d)

jambe_d = gr.mesh('sphere', 'jambe-droit')
jambe_d:scale(0.9,1.1,0.9)
jambe_d:rotate('x',0)
jambe_d:translate(0.0,-1.25,0.0);
jambe_d:set_material(gr.material({0.8, 0.2, 0.2}, {0.8, 0.8, 0.8}, 10.0))
cuisse_d:add_child(jambe_d)

pied_d = gr.mesh('sphere', 'pied-droit')
pied_d:scale(1.0,0.1,3.0)
pied_d:rotate('x',0)
pied_d:translate(0.0,-1.1,1.0);
pied_d:set_material(gr.material({0.8, 0.2, 0.2}, {0.8, 0.8, 0.8}, 10.0))
jambe_d:add_child(pied_d)


return rootNode

