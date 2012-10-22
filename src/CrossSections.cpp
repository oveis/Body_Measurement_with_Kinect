/****************************************************************************
* 
*  Computer Vision, Fall 2011
*  New York University
*
*  Created by Otavio Braga (obraga@cs.nyu.edu)
*
****************************************************************************/

#include <iostream>
#include "CrossSections.h"
#include "Skeleton.h"
#include <math.h>

enum
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

void unit_vector(cv::Vec3d &x){
	double length = sqrt(x[0]*x[0] + x[1]*x[1] + x[2]*x[2]);
	x[0] /= length;
	x[1] /= length;
	x[2] /= length;
}	
	
void ComputeCrossSections(std::vector<cv::Vec3d> &joints, cv::Vec3d axis[3],
                          cv::Vec3d O[NUM_CS],
                          cv::Vec3d N[NUM_CS],
                          cv::Vec3d x[NUM_CS],
                          cv::Vec3d y[NUM_CS])
{
    // !!!!!!!!!!!!!!!!!!!! Implement this !!!!!!!!!!!!!!!!!!!!
    //std::cout << "Warning: ComputeCrossSections not implemented!\n";


	// Chest, Waist and Hips
	O[CS_CHEST] = (joints[SKEL_TORSO] + joints[SKEL_NECK])*0.5;
	O[CS_WAIST] = joints[SKEL_TORSO];
	O[CS_HIPS] = (joints[SKEL_LEFT_HIP] + joints[SKEL_RIGHT_HIP])*0.5;

	N[CS_CHEST] = axis[0];
	N[CS_WAIST] = axis[0];
	N[CS_HIPS] = axis[0];
	unit_vector(N[CS_CHEST]);
	unit_vector(N[CS_WAIST]);
	unit_vector(N[CS_HIPS]);

	x[CS_CHEST] = axis[1];
	x[CS_WAIST] = axis[1];
	x[CS_HIPS] = axis[1];

	y[CS_CHEST] = axis[2];
	y[CS_WAIST] = axis[2];
	y[CS_HIPS] = axis[2];

	// Biceps
	O[CS_LEFT_BICEPS] = (joints[SKEL_LEFT_SHOULDER] + joints[SKEL_LEFT_ELBOW])*0.5;
	O[CS_RIGHT_BICEPS] = (joints[SKEL_RIGHT_SHOULDER] + joints[SKEL_RIGHT_ELBOW])*0.5;
	N[CS_LEFT_BICEPS] = joints[SKEL_LEFT_ELBOW] - joints[SKEL_LEFT_SHOULDER];
	N[CS_RIGHT_BICEPS] = joints[SKEL_RIGHT_ELBOW] - joints[SKEL_RIGHT_SHOULDER];
	unit_vector(N[CS_LEFT_BICEPS]);
	unit_vector(N[CS_RIGHT_BICEPS]);
	x[CS_LEFT_BICEPS] = axis[0];
	x[CS_RIGHT_BICEPS] = axis[0];
	y[CS_LEFT_BICEPS] = axis[2];
	y[CS_RIGHT_BICEPS] = axis[2];

	// Tights
	O[CS_LEFT_THIGH] = (joints[SKEL_LEFT_HIP] + joints[SKEL_LEFT_KNEE])*0.5;
	O[CS_RIGHT_THIGH] = (joints[SKEL_RIGHT_HIP] + joints[SKEL_RIGHT_KNEE])*0.5;
	N[CS_LEFT_THIGH] = joints[SKEL_LEFT_KNEE] - joints[SKEL_LEFT_HIP];
	N[CS_RIGHT_THIGH] = joints[SKEL_RIGHT_KNEE] - joints[SKEL_RIGHT_HIP];
	unit_vector(N[CS_LEFT_THIGH]);
	unit_vector(N[CS_RIGHT_THIGH]);
	x[CS_LEFT_THIGH] = axis[1];
	x[CS_RIGHT_THIGH] = axis[1];
	y[CS_LEFT_THIGH] = axis[2];
	y[CS_RIGHT_THIGH] = axis[2];		

	
}
