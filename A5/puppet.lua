rootnode = gr.node('root')
rootnode:scale(0.1,0.1,0.1)
rootnode:rotate('y',180)
rootnode:translate(0.0,0.0,10.0);

epee_jointure = gr.joint("sword-joint", {-180, 0, 180}, {-180,0, 180})
rootnode:add_child(epee_jointure)
--cou_jointure:set_material(gr.material({0.8, 0.2, 0.2}, {0.8, 0.8, 0.8}, 10.0))
epee_jointure:translate(0.0, -7.0, 0)
--epee_jointure:rotate('z', 90.0)
--epee_jointure:scale(0.012,0.015,0.012)



epee = gr.mesh('sword','sword')
epee:rotate('z', 90.0)
epee:scale(0.012,0.015,0.012)
epee:translate(13.0, 0.0, 0.0)
epee:set_material(gr.material({0.5, 0.5, 0.5}, {0.3, 0.3, 0.3}, 50.0))
--epee_jointure:add_child(epee)

sw1 = gr.mesh('sw1','sw1')
sw1:rotate('z', 90.0)
sw1:scale(0.012,0.012,0.012)
sw1:translate(13.0, 0.0, 0.0)
sw1:set_material(gr.material({1.0, 0.847, 0.396}, {0.33, 0.33, 0.33}, 10.0))
epee_jointure:add_child(sw1)

sw2 = gr.mesh('sw2','sw2')
sw2:set_material(gr.material({0.321569, 0.368627, 0.6}, {0.33, 0.33, 0.33}, 10.0))
sw1:add_child(sw2)

sw3 = gr.mesh('sw3','sw3')
sw3:set_material(gr.material({1.0, 0.847, 0.396}, {0.33, 0.33, 0.33}, 10.0))
sw2:add_child(sw3)

sw4 = gr.mesh('sw4','sw4')
sw4:set_material(gr.material({1.0, 0.847, 0.396}, {0.33, 0.33, 0.33}, 10.0))
sw3:add_child(sw4)

sw5 = gr.mesh('sw5','sw5')
sw5:set_material(gr.material({0.79, 0.819, 0.855}, {0.33, 0.33, 0.33}, 5.0))
sw4:add_child(sw5)



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

epaule_g_jointure = gr.joint( 'ep-gauche-jointure', {-50, 0, 50}, {0,0, 0})
epaule_g_jointure:translate(-6.0, 8.3, -5.6)
doc:add_child(epaule_g_jointure)

epaule_g = gr.mesh('leftshoder', 'ep-gauche')
epaule_g:translate(-2.7, 0.0, 0.0)
epaule_g:set_material(gr.material({0.5, 0.5, 0.5}, {0.3, 0.3, 0.3}, 50.0))
--epaule_g_jointure:add_child(epaule_g)


bras_g_jointure = gr.joint("bras_g_jointure", {-50, 0, 50}, {-15,0, 50})
bras_g_jointure:translate(-5.0, 0.0, 0.0)
--bras_g_jointure:set_material(gr.material({0.8, 0.2, 0.2}, {0.8, 0.8, 0.8}, 10.0))
epaule_g:add_child(bras_g_jointure)


---------------------new left arm
bras_g = gr.mesh('leftarm','br-gauche')
bras_g:rotate('y', 90.0)
bras_g:scale(0.025,0.025,0.025)
bras_g:translate(-1.4,-23.9,-0.7)
--bras_g:translate(-2.5,0.0,0.0)
bras_g:set_material(gr.material({0.5, 0.5, 0.5}, {0.3, 0.3, 0.3}, 50.0))
--bras_g_jointure:add_child(bras_g)
epaule_g_jointure:add_child(bras_g)

main_g_jointure = gr.joint("main_g_jointure", {-90, 0, 90}, {-5,0, 5})
main_g_jointure:translate(-5.0, 0.0, 0.0)
--bras_g:add_child(main_g_jointure)

main_g = gr.mesh('lefthand','main-gauche')
main_g:translate(-2.5,0.0,0.0)
main_g:set_material(gr.material({0.5, 0.5, 0.5}, {0.3, 0.3, 0.3}, 50.0))
--main_g_jointure:add_child(main_g)

--------------

epaule_d_jointure = gr.joint( 'ep-droit-jointure', {-50, 0, 50}, {0,0, 0})
epaule_d_jointure:translate(6.0, 8.3, -5.6)
doc:add_child(epaule_d_jointure)

epaule_d = gr.mesh('rightshoulder', 'ep-droit')
epaule_d:translate(4.5, -0.5, -0.3)
epaule_d:set_material(gr.material({0.5, 0.5, 0.5}, {0.3, 0.3, 0.3}, 50.0))
--epaule_d_jointure:add_child(epaule_d)

bras_d_jointure = gr.joint("bras_d_jointure", {-50, 0, 50}, {-50,0, 18})
bras_d_jointure:translate(5.0, 0.0, 0.0)
--bras_d_jointure:set_material(gr.material({0.8, 0.2, 0.2}, {0.8, 0.8, 0.8}, 10.0))
epaule_d:add_child(bras_d_jointure)


------------new right arm
bras_d = gr.mesh('rightarm','br-droit')
bras_d:rotate('y', 90.0)
bras_d:scale(0.025,0.025,0.025)
bras_d:translate(1.7,-23.7,-0.5)
--bras_d:translate(3.7,0.0,0.0)
bras_d:set_material(gr.material({0.5, 0.5, 0.5}, {0.3, 0.3, 0.3}, 50.0))
--bras_d_jointure:add_child(bras_d)
epaule_d_jointure:add_child(bras_d)

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
cuisse_g_jointure:translate(-3.8,-11.0,-5.0);
doc:add_child(cuisse_g_jointure)

cuisse_g = gr.mesh('leftbigleg', 'cuisse-gauche')
cuisse_g:translate(0.3,-6.0,4.2);
cuisse_g:set_material(gr.material({0.5, 0.5, 0.5}, {0.3, 0.3, 0.3}, 50.0))
cuisse_g_jointure:add_child(cuisse_g)

--jambe_g_jointure = gr.mesh('sphere','s0')
jambe_g_jointure =  gr.joint("jambe_g_jointure", {-30, 0, 30}, {-0,0, 0})
jambe_g_jointure:translate(0.5,-8.5,-2.8);
--jambe_g_jointure:set_material(gr.material({0.8, 0.2, 0.2}, {0.8, 0.8, 0.8}, 10.0))
cuisse_g:add_child(jambe_g_jointure)

jambe_g = gr.mesh('leftsmalllge', 'jambe-gauche')
jambe_g:translate(-0.4,-10.8,0.9);
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
cuisse_d_jointure:translate(3.8,-10.0,-5.0);
doc:add_child(cuisse_d_jointure)

cuisse_d = gr.mesh('rightbigleg', 'cuisse-droit')
cuisse_d:translate(-0.3,-10.0,0.2);
cuisse_d:set_material(gr.material({0.5, 0.5, 0.5}, {0.3, 0.3, 0.3}, 50.0))
cuisse_d_jointure:add_child(cuisse_d)

--jambe_d_jointure = gr.mesh('sphere','s0')
jambe_d_jointure =  gr.joint("jambe_d_jointure", {-30, 0, 30}, {-0,0, 0})
jambe_d_jointure:translate(-0.5,-6.0,1.0);
cuisse_d:add_child(jambe_d_jointure)

jambe_d = gr.mesh('rightsmalllge', 'jambe-droite')
jambe_d:translate(0.2,-11.5,-1.4);
jambe_d:set_material(gr.material({0.5, 0.5, 0.5}, {0.3, 0.3, 0.3}, 50.0))
jambe_d_jointure:add_child(jambe_d)



pied_d_jointure =  gr.joint("pied_d_jointure", {-30, 0, 30}, {-10,0, 10})
pied_d_jointure:translate(0.0,-7.5,-1.0);
--pied_d_jointure:set_material(gr.material({0.8, 0.2, 0.2}, {0.8, 0.8, 0.8}, 10.0))
jambe_d:add_child(pied_d_jointure)


pied_d = gr.mesh('leftshoe', 'pied-droit')
pied_d:translate(0.15,-2.2,8.7);
pied_d:set_material(gr.material({0.5, 0.5, 0.5}, {0.3, 0.3, 0.3}, 50.0))
pied_d_jointure:add_child(pied_d)




rootnode:translate(0.0, 0.0, -10.0)

return rootnode
