/****************************************************************************
* 
*  Computer Vision, Fall 2011
*  New York University
*
*  Created by Otavio Braga (obraga@cs.nyu.edu)
*
****************************************************************************/

#ifndef SKELETON_NODE_H
#define SKELETON_NODE_H

#include <assert.h>
#include <vector>

#include <XnOpenNI.h>

class Skeleton
{
    public:
        /// Default constructor.
        Skeleton();
        
        /// Copy constructor.
        Skeleton(const Skeleton &src);

        enum JointID
        {
            SKEL_HEAD,              
            SKEL_NECK,              
            SKEL_TORSO,             
            SKEL_LEFT_SHOULDER,     
            SKEL_LEFT_ELBOW,        
            SKEL_LEFT_HAND,         
            SKEL_RIGHT_SHOULDER,    
            SKEL_RIGHT_ELBOW,       
            SKEL_RIGHT_HAND,        
            SKEL_LEFT_HIP,          
            SKEL_LEFT_KNEE,         
            SKEL_LEFT_FOOT,         
            SKEL_RIGHT_HIP,         
            SKEL_RIGHT_KNEE,        
            SKEL_RIGHT_FOOT,        
            SKEL_NJOINTS
        };

        // Set a joint position
        bool setJoint(JointID joint_id, XnVector3D &joint);

        // Get a joint position
        XnVector3D getJoint(JointID joint_id) const;

        // Get all joint positions
        const std::vector<XnVector3D> &getJoints() const { return joints; }
        
        // Set all joint positions
        void setJoints(const std::vector<XnVector3D> &joints) { this->joints = joints; };
        
    private:
        // The joint positions in the world (root joint) coordinate system
        std::vector<XnVector3D> joints;
};

inline bool Skeleton::setJoint(JointID joint_id, XnVector3D &joint)
{
    if (joint_id < 0 || joint_id >= SKEL_NJOINTS)
        return false;
    joints[joint_id] = joint;
    return true;
}

inline XnVector3D Skeleton::getJoint(JointID joint_id) const
{
    assert(joint_id >= 0 && joint_id < SKEL_NJOINTS);
    return joints[joint_id];
}

#endif // SKELETON_NODE_H
