/****************************************************************************
* 
*  Computer Vision, Fall 2011
*  New York University
*
*  Created by Otavio Braga (obraga@cs.nyu.edu)
*
****************************************************************************/

#include <iostream>
#include <opencv2/core/core.hpp>
#include <math.h>

#include "AngularSkeleton.h"
#include "Skeleton.h"

using namespace std;

#ifndef M_PI
#define M_PI 3.141592653589
#endif

cv::Vec3d unit_vector(cv::Vec3d vector){
	cv::Vec3d temp;
	double length = sqrt(pow(vector[0],2)+pow(vector[1],2)+pow(vector[2],2));
	temp[0] = vector[0]/length;
	temp[1] = vector[1]/length;
	temp[2] = vector[2]/length;
	return temp;
}

void computeSkeletonAngularRepresentation(std::vector<cv::Vec3d> &joints,
                                          cv::vector<double> &angles,
                                          cv::Vec3d axis[3])
{
    // !!!!!!!!!!!!!!!!!!!! Implement this !!!!!!!!!!!!!!!!!!!!
//    std::cout << "Warning: computeAngularRepresentation not implemented!\n";

	angles.resize(SAR_NANGLES);
    for (unsigned int i = 0; i < angles.size(); i++)
        angles[i] = 0.0;

	for(int i=0; i<3; i++){	
		axis[0][i] = joints[SKEL_NECK][i] - joints[SKEL_TORSO][i];
		axis[1][i] = joints[SKEL_RIGHT_SHOULDER][i] - joints[SKEL_LEFT_SHOULDER][i];
	}
	// W = U X V
	axis[2] = axis[0].cross(axis[1]);
/*
	axis[2][0] = axis[0][1]*axis[1][2] - axis[0][2]*axis[1][1];
	axis[2][1] = -(axis[0][0]*axis[1][2] - axis[0][2]*axis[1][0]);
	axis[2][2] = axis[0][0]*axis[1][1] - axis[0][1]*axis[1][0];
*/
	// make Unit Vector
	for(int i=0; i<3; i++)
		axis[i] = unit_vector(axis[i]);	

	double length_u = sqrt(pow(axis[0][0],2)+pow(axis[0][1],2)+pow(axis[0][2],2));			
	double length_v = sqrt(pow(axis[1][0],2)+pow(axis[1][1],2)+pow(axis[1][2],2));
	
	double pitch, yaw, roll;
	if(axis[2][0]!=1 && axis[2][0]!=-1){
		pitch = -asin(axis[2][0]);
		double x = cos(pitch);
		yaw = atan2(axis[2][1]/x, axis[2][2]/x);
		roll = atan2(axis[1][0]/x, axis[0][0]/x);
	}
	else{
		roll = 0;
		if(axis[2][0] == -1){
			pitch = M_PI/2.0;
			yaw = atan2(axis[0][1], axis[0][2]);
		}
		else{
			pitch = -M_PI/2.0;
			yaw = atan2(-axis[0][1], -axis[0][2]);
		}
	}

	angles[SAR_TORSO_YAW] = yaw *(180/M_PI);
	angles[SAR_TORSO_PITCH] = pitch*(180/M_PI);
	angles[SAR_TORSO_ROLL] = roll*(180/M_PI);

	cv::Vec3d temp;
	double length_temp = 0;

	// Left shoulder
	temp[0] = joints[SKEL_LEFT_ELBOW][0] - joints[SKEL_LEFT_SHOULDER][0];
	temp[1] = joints[SKEL_LEFT_ELBOW][1] - joints[SKEL_LEFT_SHOULDER][1];
	temp[2] = joints[SKEL_LEFT_ELBOW][2] - joints[SKEL_LEFT_SHOULDER][2];
	length_temp = sqrt(pow(temp[0],2)+pow(temp[1],2)+pow(temp[2],2));
	angles[SAR_LS_AZIMUTH] = acos((axis[1][0]*temp[0]+axis[1][1]*temp[1]+axis[1][2]*temp[2])
							/ (length_v*length_temp)) * (180/M_PI);
	angles[SAR_LS_INC] = acos((axis[0][0]*temp[0]+axis[0][1]*temp[1]+axis[0][2]*temp[2])
							/ (length_u*length_temp)) * (180/M_PI);
    

	// Right shoulder
	temp[0] = joints[SKEL_RIGHT_ELBOW][0] - joints[SKEL_RIGHT_SHOULDER][0];
	temp[1] = joints[SKEL_RIGHT_ELBOW][1] - joints[SKEL_RIGHT_SHOULDER][1];
	temp[2] = joints[SKEL_RIGHT_ELBOW][2] - joints[SKEL_RIGHT_SHOULDER][2];
	length_temp = sqrt(pow(temp[0],2)+pow(temp[1],2)+pow(temp[2],2));
	angles[SAR_RS_AZIMUTH] = acos((axis[1][0]*temp[0]+axis[1][1]*temp[1]+axis[1][2]*temp[2])
							/ (length_v*length_temp)) * (180/M_PI);
	angles[SAR_RS_INC] = acos((axis[0][0]*temp[0]+axis[0][1]*temp[1]+axis[0][2]*temp[2])
							/ (length_u*length_temp)) * (180/M_PI);


	// Left hip
	temp[0] = joints[SKEL_LEFT_KNEE][0] - joints[SKEL_LEFT_HIP][0];
	temp[1] = joints[SKEL_LEFT_KNEE][1] - joints[SKEL_LEFT_HIP][1];
	temp[2] = joints[SKEL_LEFT_KNEE][2] - joints[SKEL_LEFT_HIP][2];
	length_temp = sqrt(pow(temp[0],2)+pow(temp[1],2)+pow(temp[2],2));
	angles[SAR_LH_AZIMUTH] = acos((axis[1][0]*temp[0]+axis[1][1]*temp[1]+axis[1][2]*temp[2])
							/ (length_v*length_temp)) * (180/M_PI);
	angles[SAR_LH_INC] = acos((axis[0][0]*temp[0]+axis[0][1]*temp[1]+axis[0][2]*temp[2])
							/ (length_u*length_temp)) * (180/M_PI);    

	// Right hip
	temp[0] = joints[SKEL_RIGHT_KNEE][0] - joints[SKEL_RIGHT_HIP][0];
	temp[1] = joints[SKEL_RIGHT_KNEE][1] - joints[SKEL_RIGHT_HIP][1];
	temp[2] = joints[SKEL_RIGHT_KNEE][2] - joints[SKEL_RIGHT_HIP][2];
	length_temp = sqrt(pow(temp[0],2)+pow(temp[1],2)+pow(temp[2],2));
	angles[SAR_RH_AZIMUTH] = acos((axis[1][0]*temp[0]+axis[1][1]*temp[1]+axis[1][2]*temp[2])
							/ (length_v*length_temp)) * (180/M_PI);
	angles[SAR_RH_INC] = acos((axis[0][0]*temp[0]+axis[0][1]*temp[1]+axis[0][2]*temp[2])
							/ (length_u*length_temp)) * (180/M_PI);
    
    // Left elbow
	cv::Vec3d b, proj_u, proj_temp;
	double length_b, length_proj_u, length_proj_temp;
	b[0] = joints[SKEL_LEFT_ELBOW][0] - joints[SKEL_LEFT_SHOULDER][0];
	b[1] = joints[SKEL_LEFT_ELBOW][1] - joints[SKEL_LEFT_SHOULDER][1];
	b[2] = joints[SKEL_LEFT_ELBOW][2] - joints[SKEL_LEFT_SHOULDER][2];
	temp[0] = joints[SKEL_LEFT_HAND][0] - joints[SKEL_LEFT_ELBOW][0];
	temp[1] = joints[SKEL_LEFT_HAND][1] - joints[SKEL_LEFT_ELBOW][1];
	temp[2] = joints[SKEL_LEFT_HAND][2] - joints[SKEL_LEFT_ELBOW][2];
	length_temp = sqrt(pow(temp[0],2)+pow(temp[1],2)+pow(temp[2],2));
	length_b = sqrt(pow(b[0],2)+pow(b[1],2)+pow(b[2],2));
	angles[SAR_LE_INC] = acos((b[0]*temp[0]+b[1]*temp[1]+b[2]*temp[2])
							/ (length_b*length_temp)) * (180/M_PI);
	double u_dot_b = axis[0][0]*b[0] + axis[0][1]*b[1] + axis[0][2]*b[2];
	proj_u[0] = axis[0][0] - u_dot_b*b[0];
	proj_u[1] = axis[0][1] - u_dot_b*b[1];
	proj_u[2] = axis[0][2] - u_dot_b*b[2];
	length_proj_u = sqrt(pow(proj_u[0],2)+pow(proj_u[1],2)+pow(proj_u[2],2));
	double temp_dot_b = temp[0]*b[0] + temp[1]*b[1] + temp[2]*b[2];
	proj_temp[0] = temp[0] - temp_dot_b*b[0];
	proj_temp[1] = temp[1] - temp_dot_b*b[1];
	proj_temp[2] = temp[2] - temp_dot_b*b[2];
	length_proj_temp = sqrt(pow(proj_temp[0],2)+pow(proj_temp[1],2)+pow(proj_temp[2],2));
	angles[SAR_LE_AZIMUTH] = acos((proj_u[0]*proj_temp[0]+proj_u[1]*proj_temp[1]+proj_u[2]*proj_temp[2])
							/ (length_proj_u*length_proj_temp)) * (180/M_PI);

//	cout << "left elbow : " << angles[SAR_LE_INC];

	// Right elbow

	b[0] = joints[SKEL_RIGHT_ELBOW][0] - joints[SKEL_RIGHT_SHOULDER][0];
	b[1] = joints[SKEL_RIGHT_ELBOW][1] - joints[SKEL_RIGHT_SHOULDER][1];
	b[2] = joints[SKEL_RIGHT_ELBOW][2] - joints[SKEL_RIGHT_SHOULDER][2];
	temp[0] = joints[SKEL_RIGHT_HAND][0] - joints[SKEL_RIGHT_ELBOW][0];
	temp[1] = joints[SKEL_RIGHT_HAND][1] - joints[SKEL_RIGHT_ELBOW][1];
	temp[2] = joints[SKEL_RIGHT_HAND][2] - joints[SKEL_RIGHT_ELBOW][2];
	length_temp = sqrt(pow(temp[0],2)+pow(temp[1],2)+pow(temp[2],2));
	length_b = sqrt(pow(b[0],2)+pow(b[1],2)+pow(b[2],2));
	angles[SAR_RE_INC] = acos((b[0]*temp[0] + b[1]*temp[1] + b[2]*temp[2])
							/ (length_b * length_temp)) * (180/M_PI);

	u_dot_b = axis[0][0]*b[0] + axis[0][1]*b[1] + axis[0][2]*b[2];
	proj_u[0] = axis[0][0] - u_dot_b*b[0];
	proj_u[1] = axis[0][1] - u_dot_b*b[1];
	proj_u[2] = axis[0][2] - u_dot_b*b[2];
	length_proj_u = sqrt(pow(proj_u[0],2)+pow(proj_u[1],2)+pow(proj_u[2],2));
	temp_dot_b = temp[0]*b[0] + temp[1]*b[1] + temp[2]*b[2];
	proj_temp[0] = temp[0] - temp_dot_b*b[0];
	proj_temp[1] = temp[1] - temp_dot_b*b[1];
	proj_temp[2] = temp[2] - temp_dot_b*b[2];
	length_proj_temp = sqrt(pow(proj_temp[0],2)+pow(proj_temp[1],2)+pow(proj_temp[2],2));
	angles[SAR_RE_AZIMUTH] = acos((proj_u[0]*proj_temp[0]+proj_u[1]*proj_temp[1]+proj_u[2]*proj_temp[2])
							/ (length_proj_u*length_proj_temp)) * (180/M_PI);

	// Left knee
	b[0] = joints[SKEL_LEFT_KNEE][0] - joints[SKEL_LEFT_HIP][0];
	b[1] = joints[SKEL_LEFT_KNEE][1] - joints[SKEL_LEFT_HIP][1];
	b[2] = joints[SKEL_LEFT_KNEE][2] - joints[SKEL_LEFT_HIP][2];
	temp[0] = joints[SKEL_LEFT_FOOT][0] - joints[SKEL_LEFT_KNEE][0];
	temp[1] = joints[SKEL_LEFT_FOOT][1] - joints[SKEL_LEFT_KNEE][1];
	temp[2] = joints[SKEL_LEFT_FOOT][2] - joints[SKEL_LEFT_KNEE][2];
	length_temp = sqrt(pow(temp[0],2)+pow(temp[1],2)+pow(temp[2],2));
	length_b = sqrt(pow(b[0],2)+pow(b[1],2)+pow(b[2],2));
	angles[SAR_LK_INC] = acos((b[0]*temp[0]+b[1]*temp[1]+b[2]*temp[2])
							/ (length_b*length_temp)) * (180/M_PI);
	u_dot_b = axis[0][0]*b[0] + axis[0][1]*b[1] + axis[0][2]*b[2];
	proj_u[0] = axis[0][0] - u_dot_b*b[0];
	proj_u[1] = axis[0][1] - u_dot_b*b[1];
	proj_u[2] = axis[0][2] - u_dot_b*b[2];
	length_proj_u = sqrt(pow(proj_u[0],2)+pow(proj_u[1],2)+pow(proj_u[2],2));
	temp_dot_b = temp[0]*b[0] + temp[1]*b[1] + temp[2]*b[2];
	proj_temp[0] = temp[0] - temp_dot_b*b[0];
	proj_temp[1] = temp[1] - temp_dot_b*b[1];
	proj_temp[2] = temp[2] - temp_dot_b*b[2];
	length_proj_temp = sqrt(pow(proj_temp[0],2)+pow(proj_temp[1],2)+pow(proj_temp[2],2));
	angles[SAR_LK_AZIMUTH] = acos((proj_u[0]*proj_temp[0]+proj_u[1]*proj_temp[1]+proj_u[2]*proj_temp[2])
							/ (length_proj_u*length_proj_temp)) * (180/M_PI);

	// Right knee
	b[0] = joints[SKEL_RIGHT_KNEE][0] - joints[SKEL_RIGHT_HIP][0];
	b[1] = joints[SKEL_RIGHT_KNEE][1] - joints[SKEL_RIGHT_HIP][1];
	b[2] = joints[SKEL_RIGHT_KNEE][2] - joints[SKEL_RIGHT_HIP][2];
	temp[0] = joints[SKEL_RIGHT_FOOT][0] - joints[SKEL_RIGHT_KNEE][0];
	temp[1] = joints[SKEL_RIGHT_FOOT][1] - joints[SKEL_RIGHT_KNEE][1];
	temp[2] = joints[SKEL_RIGHT_FOOT][2] - joints[SKEL_RIGHT_KNEE][2];
	length_temp = sqrt(pow(temp[0],2)+pow(temp[1],2)+pow(temp[2],2));
	length_b = sqrt(pow(b[0],2)+pow(b[1],2)+pow(b[2],2));
	angles[SAR_RK_INC] = acos((b[0]*temp[0]+b[1]*temp[1]+b[2]*temp[2])
							/ (length_b*length_temp)) * (180/M_PI);
	u_dot_b = axis[0][0]*b[0] + axis[0][1]*b[1] + axis[0][2]*b[2];
	proj_u[0] = axis[0][0] - u_dot_b*b[0];
	proj_u[1] = axis[0][1] - u_dot_b*b[1];
	proj_u[2] = axis[0][2] - u_dot_b*b[2];
	length_proj_u = sqrt(pow(proj_u[0],2)+pow(proj_u[1],2)+pow(proj_u[2],2));
	temp_dot_b = temp[0]*b[0] + temp[1]*b[1] + temp[2]*b[2];
	proj_temp[0] = temp[0] - temp_dot_b*b[0];
	proj_temp[1] = temp[1] - temp_dot_b*b[1];
	proj_temp[2] = temp[2] - temp_dot_b*b[2];
	length_proj_temp = sqrt(pow(proj_temp[0],2)+pow(proj_temp[1],2)+pow(proj_temp[2],2));
	angles[SAR_RK_AZIMUTH] = acos((proj_u[0]*proj_temp[0]+proj_u[1]*proj_temp[1]+proj_u[2]*proj_temp[2])
							/ (length_proj_u*length_proj_temp)) * (180/M_PI);
}

bool matchAngularPose(cv::vector<double> &pose, PoseID poseid)
{
    // !!!!!!!!!!!!!!!!!!!! Implement this !!!!!!!!!!!!!!!!!!!!
   // std::cout << "Warning: matchAngularPose not implemented!\n";
/*
	cout << "pitch : " << pose[SAR_TORSO_PITCH] << endl;
	cout << "right_elbow : " << pose[SAR_RE_INC] << ", left_elbow : " << pose[SAR_LE_INC] << endl;
	cout << "right_elbow_azi : " << pose[SAR_RE_AZIMUTH] << ", left_elbow_azi : " << pose[SAR_LE_AZIMUTH] << endl;
	cout << "right_shoulder : " << pose[SAR_RS_INC] << ", right_shoulder : " << pose[SAR_LS_INC] << endl;
*/
/*
	cout << endl;
	cout << "pitch : " << pose[SAR_TORSO_PITCH] << endl;
	cout << "yaw : " << pose[SAR_TORSO_YAW] << endl;
	cout << "roll : " << pose[SAR_TORSO_ROLL] << endl;
*/
	if((poseid==POSE_HOSTAGE_FORWARD && pose[SAR_TORSO_YAW]>0) || (poseid==POSE_HOSTAGE_BACKWARDS && pose[SAR_TORSO_YAW]<0))
		if((pose[SAR_RE_INC]>=50) && (pose[SAR_RE_AZIMUTH]<=5) && (pose[SAR_LE_INC]>=50 && pose[SAR_LE_AZIMUTH]<=5))
			return true;				

   return false;
}
