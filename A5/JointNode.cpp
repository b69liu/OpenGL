#include "JointNode.hpp"

//---------------------------------------------------------------------------------------
JointNode::JointNode(const std::string& name)
	: SceneNode(name)
{
	m_nodeType = NodeType::JointNode;
}

//---------------------------------------------------------------------------------------
JointNode::~JointNode() {

}
 //---------------------------------------------------------------------------------------
void JointNode::set_joint_x(double min, double init, double max) {
	m_joint_x.min = min;
	m_joint_x.init = init;
	m_joint_x.max = max;
        cur_x = init;
}

//---------------------------------------------------------------------------------------
void JointNode::set_joint_y(double min, double init, double max) {
	m_joint_y.min = min;
	m_joint_y.init = init;
	m_joint_y.max = max;
        cur_y = init;
}

//---------------------------------------------------------------------------------------
void JointNode::move_x(double value){
        if(value+cur_x > m_joint_x.max){
             cur_x = m_joint_x.max;
        }else if(value+cur_x < m_joint_x.min){
             cur_x = m_joint_x.min;
        }else{
             cur_x += value;
        }//end if val
            
}

//---------------------------------------------------------------------------------------
void JointNode::move_y(double value){
        if(value+cur_y > m_joint_y.max){
             cur_y = m_joint_y.max;
        }else if(value+cur_y < m_joint_y.min){
             cur_y = m_joint_y.min;
        }else{
             cur_y += value;
        }//end if val
 
}



