-- a3mark.lua
-- A very simple scene creating a trivial hierarchical puppet.
-- We'll use this scene as part of testing your assignment.
-- See a3mark.png for an example of what this file produces.

rootnode = gr.node('root')


s0 = gr.mesh('head','s0')
rootnode:add_child(s0)

rootnode:translate(-2.75, 0.0, -10.0)


return rootnode
