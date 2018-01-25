rootnode = gr.node('root')
rootnode:scale(0.1,0.1,0.1)


doc = gr.mesh('tor','tor')
doc:translate(0.0, 0.0, 1)
doc:set_material(gr.material({0.5, 0.5, 0.5}, {0.3, 0.3, 0.3}, 50.0))
rootnode:add_child(doc)

cou_jointure = gr.joint("neck-joint", {-15, 0, 15}, {-30,0, 30})
doc:add_child(cou_jointure)
--cou_jointure:set_material(gr.material({0.8, 0.2, 0.2}, {0.8, 0.8, 0.8}, 10.0))
cou_jointure:translate(0.0, 10.5, -5.0)

cou = gr.mesh('neck','cou')
cou:rotate('y', 0.0)
cou:translate(0.0, -0.3, 0.3)
cou:set_material(gr.material({0.5, 0.5, 0.5}, {0.3, 0.3, 0.3}, 50.0))
cou_jointure:add_child(cou)

tete_jointure = gr.joint("neck-joint", {-20, 0, 30}, {-30,0, 30})
tete_jointure:translate(0.0, 1.3, 0.0)
cou:add_child(tete_jointure)

tete = gr.mesh('head','tete')
tete:set_material(gr.material({0.5, 0.5, 0.5}, {0.3, 0.3, 0.3}, 50.0))
tete_jointure:add_child(tete)

-----------

epaule_g_jointure = gr.joint( 'ep-gauche-jointure', {-30, 0, 50}, {-30,0, 50})
epaule_g_jointure:translate(-6.0, 8.3, -5.6)
doc:add_child(epaule_g_jointure)

epaule_g = gr.mesh('leftshoder', 'ep-gauche')
epaule_g:translate(-2.7, 0.0, 0.0)
epaule_g:set_material(gr.material({0.5, 0.5, 0.5}, {0.3, 0.3, 0.3}, 50.0))
epaule_g_jointure:add_child(epaule_g)


bras_g_jointure = gr.joint("bras_g_jointure", {-50, 0, 50}, {-15,0, 50})
bras_g_jointure:translate(-5.0, 0.0, 0.0)
--bras_g_jointure:set_material(gr.material({0.8, 0.2, 0.2}, {0.8, 0.8, 0.8}, 10.0))
epaule_g:add_child(bras_g_jointure)

bras_g = gr.mesh('leftarm','br-gauche')
bras_g:translate(-2.5,0.0,0.0)
bras_g:set_material(gr.material({0.5, 0.5, 0.5}, {0.3, 0.3, 0.3}, 50.0))
bras_g_jointure:add_child(bras_g)


main_g_jointure = gr.joint("main_g_jointure", {-90, 0, 90}, {-5,0, 5})
main_g_jointure:translate(-5.0, 0.0, 0.0)
bras_g:add_child(main_g_jointure)

main_g = gr.mesh('lefthand','main-gauche')
main_g:translate(-2.5,0.0,0.0)
main_g:set_material(gr.material({0.5, 0.5, 0.5}, {0.3, 0.3, 0.3}, 50.0))
main_g_jointure:add_child(main_g)

--------------

epaule_d_jointure = gr.joint( 'ep-droit-jointure', {-50, 0, 50}, {-50,0, 30})
epaule_d_jointure:translate(6.0, 8.3, -5.6)
doc:add_child(epaule_d_jointure)

epaule_d = gr.mesh('rightshoulder', 'ep-droit')
epaule_d:translate(4.5, -0.5, -0.3)
epaule_d:set_material(gr.material({0.5, 0.5, 0.5}, {0.3, 0.3, 0.3}, 50.0))
epaule_d_jointure:add_child(epaule_d)

bras_d_jointure = gr.joint("bras_d_jointure", {-50, 0, 50}, {-50,0, 18})
bras_d_jointure:translate(5.0, 0.0, 0.0)
--bras_d_jointure:set_material(gr.material({0.8, 0.2, 0.2}, {0.8, 0.8, 0.8}, 10.0))
epaule_d:add_child(bras_d_jointure)

bras_d = gr.mesh('rightarm','br-droit')
bras_d:translate(3.7,0.0,0.0)
bras_d:set_material(gr.material({0.5, 0.5, 0.5}, {0.3, 0.3, 0.3}, 50.0))
bras_d_jointure:add_child(bras_d)

main_d_jointure = gr.joint("main_d_jointure", {-90, 0, 90}, {-30,0, 30})
main_d_jointure:translate(5.0, 0.0, 0.0)
--main_d_jointure:set_material(gr.material({0.8, 0.2, 0.2}, {0.8, 0.8, 0.8}, 10.0))
bras_d:add_child(main_d_jointure)

main_d = gr.mesh('righthand','main-droit')
main_d:translate(2.5,0.0,0.0)
main_d:set_material(gr.material({0.5, 0.5, 0.5}, {0.3, 0.3, 0.3}, 50.0))
main_d_jointure:add_child(main_d)

-----------------

cuisse_g_jointure = gr.joint("cuisse_g_jointure", {-30, 0, 30}, {-5,0, 5})
cuisse_g_jointure:translate(-3.8,-12.0,-5.0);
doc:add_child(cuisse_g_jointure)

cuisse_g = gr.mesh('leftbigleg', 'cuisse-gauche')
cuisse_g:translate(0.3,-5.0,4.2);
cuisse_g:set_material(gr.material({0.5, 0.5, 0.5}, {0.3, 0.3, 0.3}, 50.0))
cuisse_g_jointure:add_child(cuisse_g)


jambe_g_jointure =  gr.joint("jambe_g_jointure", {-30, 0, 30}, {-10,0, 10})
jambe_g_jointure:translate(0.5,-9.5,-4.2);
--jambe_g_jointure:set_material(gr.material({0.8, 0.2, 0.2}, {0.8, 0.8, 0.8}, 10.0))
cuisse_g:add_child(jambe_g_jointure)

jambe_g = gr.mesh('leftsmalllge', 'jambe-gauche')
jambe_g:translate(-0.4,-9.8,2.4);
jambe_g:set_material(gr.material({0.5, 0.5, 0.5}, {0.3, 0.3, 0.3}, 50.0))
jambe_g_jointure:add_child(jambe_g)



pied_g_jointure = gr.joint("pied_g_jointure", {-30, 0, 30}, {-5,0, 5})
pied_g_jointure:translate(0.2,-9.0,-3.3);
jambe_g:add_child(pied_g_jointure)


pied_g = gr.mesh('leftshoe', 'pied-gauche')
pied_g:translate(0.3,-2.0,8.4);
pied_g:set_material(gr.material({0.5, 0.5, 0.5}, {0.3, 0.3, 0.3}, 50.0))
pied_g_jointure:add_child(pied_g)


------------------

cuisse_d_jointure = gr.joint("cuisse_d_jointure", {-30, 0, 30}, {-5,0, 5})
cuisse_d_jointure:translate(3.8,-12.0,-5.0);
doc:add_child(cuisse_d_jointure)

cuisse_d = gr.mesh('rightbigleg', 'cuisse-droit')
cuisse_d:translate(-0.3,-8.0,0.2);
cuisse_d:set_material(gr.material({0.5, 0.5, 0.5}, {0.3, 0.3, 0.3}, 50.0))
cuisse_d_jointure:add_child(cuisse_d)

jambe_d_jointure =  gr.joint("jambe_d_jointure", {-30, 0, 30}, {-10,0, 10})
jambe_d_jointure:translate(-0.5,-7.0,0.0);
cuisse_d:add_child(jambe_d_jointure)

jambe_d = gr.mesh('rightsmalllge', 'jambe-droite')
jambe_d:translate(0.2,-9.8,-0.4);
jambe_d:set_material(gr.material({0.5, 0.5, 0.5}, {0.3, 0.3, 0.3}, 50.0))
jambe_d_jointure:add_child(jambe_d)



pied_d_jointure =  gr.joint("pied_d_jointure", {-30, 0, 30}, {-10,0, 10})
pied_d_jointure:translate(0.0,-7.5,-1.0);
--pied_d_jointure:set_material(gr.material({0.8, 0.2, 0.2}, {0.8, 0.8, 0.8}, 10.0))
jambe_d:add_child(pied_d_jointure)


pied_d = gr.mesh('leftshoe', 'pied-droit')
pied_d:translate(0.15,-3.0,8.7);
pied_d:set_material(gr.material({0.5, 0.5, 0.5}, {0.3, 0.3, 0.3}, 50.0))
pied_d_jointure:add_child(pied_d)




rootnode:translate(0.0, 0.0, -10.0)

return rootnode
