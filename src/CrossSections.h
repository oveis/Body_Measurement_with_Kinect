/****************************************************************************
* 
*  Computer Vision, Fall 2011
*  New York University
*
*  Created by Otavio Braga (obraga@cs.nyu.edu)
*
****************************************************************************/

#ifndef CROSS_SECTIONS_H
#define CROSS_SECTIONS_H

#include <opencv2/core/core.hpp>

// The cross sections of the user's body
enum
{
   CS_CHEST,
   CS_HIPS,
   CS_WAIST,
   CS_LEFT_BICEPS,
   CS_RIGHT_BICEPS,
   CS_LEFT_THIGH,
   CS_RIGHT_THIGH,
   NUM_CS
};

// Compute the cross sections where we will measure the body.
//
// joints: the 19 3D joint positions.
// axis: the torso principal components.
// O, N, x, y: the cross section planes.
//
// !!!!!!!!!!!!!!!!!!!! Implement this !!!!!!!!!!!!!!!!!!!!
void ComputeCrossSections(std::vector<cv::Vec3d> &joints, cv::Vec3d axis[3],
                          cv::Vec3d O[NUM_CS],
                          cv::Vec3d N[NUM_CS],
                          cv::Vec3d x[NUM_CS],
                          cv::Vec3d y[NUM_CS]);

#endif // CROSS_SECTIONS_H
