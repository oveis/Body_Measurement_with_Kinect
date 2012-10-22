/****************************************************************************
* 
*  Computer Vision, Fall 2011
*  New York University
*
*  Created by Otavio Braga (obraga@cs.nyu.edu)
*
****************************************************************************/

#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/glu.h>
#else
#include <GL/freeglut.h>
#include <GL/glu.h>
#endif

#include "Skeleton.h"

const int Skeleton::NUM_JOINTS = 24;

const int NBONES = 12;
int BONES[NBONES][2] = {{Skeleton::SKEL_HEAD, Skeleton::SKEL_NECK},
                        {Skeleton::SKEL_LEFT_SHOULDER, Skeleton::SKEL_RIGHT_SHOULDER},
                        {Skeleton::SKEL_NECK, Skeleton::SKEL_TORSO},
                        {Skeleton::SKEL_LEFT_SHOULDER, Skeleton::SKEL_LEFT_ELBOW},
                        {Skeleton::SKEL_RIGHT_SHOULDER, Skeleton::SKEL_RIGHT_ELBOW},
                        {Skeleton::SKEL_LEFT_HAND, Skeleton::SKEL_LEFT_ELBOW},
                        {Skeleton::SKEL_RIGHT_HAND, Skeleton::SKEL_RIGHT_ELBOW},
                        {Skeleton::SKEL_LEFT_KNEE, Skeleton::SKEL_LEFT_HIP},
                        {Skeleton::SKEL_RIGHT_KNEE, Skeleton::SKEL_RIGHT_HIP},
                        {Skeleton::SKEL_RIGHT_FOOT, Skeleton::SKEL_RIGHT_ANKLE},
                        {Skeleton::SKEL_LEFT_ANKLE, Skeleton::SKEL_LEFT_KNEE},
                        {Skeleton::SKEL_LEFT_HIP, Skeleton::SKEL_RIGHT_HIP},
                        };

Skeleton::Skeleton() : joints(SKEL_NJOINTS), confidence(SKEL_NJOINTS)
{ 
}

Skeleton::Skeleton(const Skeleton &src) : joints(src.joints)
{
}

