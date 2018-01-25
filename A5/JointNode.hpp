#pragma once

#include "SceneNode.hpp"

class JointNode : public SceneNode {
public:

	JointNode(const std::string & name);
	virtual ~JointNode();

	void set_joint_x(double min, double init, double max);
	void set_joint_y(double min, double init, double max);
        void move_x(double value); //rotate selected part along x axis
        void move_y(double value); //rotate selected part along y axis
        
	struct JointRange {
		double min, init, max;
	};


	JointRange m_joint_x, m_joint_y;
        float cur_x,cur_y;
};
