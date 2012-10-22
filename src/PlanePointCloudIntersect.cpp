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

#include "PlanePointCloudIntersect.h"

void PlanePointCloudIntersect(std::vector<cv::Vec3d> &point_cloud,
                              cv::Vec3d &O, cv::Vec3d &N, cv::Vec3d &u, cv::Vec3d &v,
                              std::vector<cv::Vec2d> &intersection, int epsilon)
{
    // !!!!!!!!!!!!!!!!!!!! Implement this !!!!!!!!!!!!!!!!!!!!
    //std::cout << "Warning: PlanePointCloudIntersect not implemented!\n";
	cv::Vec3d project, temp;
	cv::Vec2d point;
	double dot, abs_dot;
	double x, y;

	for(int i=0; i<point_cloud.size(); i++){
		temp = point_cloud[i] - O;
		double size = sqrt(temp[0]*temp[0] + temp[1]*temp[1] + temp[2]*temp[2]);
		if(size>200)
			continue;
		dot = temp.dot(N);
		abs_dot = dot;
		if(abs_dot < 0)
			abs_dot = -abs_dot;
		if(abs_dot >= epsilon)
			continue;
		temp = temp - dot*N;
		
		point[0] = temp.dot(u);
		point[1] = temp.dot(v);
		intersection.push_back(point);
	}
		
}
