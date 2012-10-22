/****************************************************************************
* 
*  Computer Vision, Fall 2011
*  New York University
*
*  Created by Otavio Braga (obraga@cs.nyu.edu)
*
****************************************************************************/

#ifndef FIT_ELLIPSE_H
#define FIT_ELLIPSE_H

#include <opencv2/core/core.hpp>

// Fit an ellipse to a set of points.
// sx and sy are filled with the ellipse's semi-major and minor axis, respectively,
// theta with the ellipse's rotation, and center, with its center.
//
// !!!!!!!!!!!!!!!!!!!! Implement this !!!!!!!!!!!!!!!!!!!!
void FitEllipse(std::vector<cv::Vec2d> &pts,
                double *sx, double *sy, double *theta, cv::Vec2d *center);

#endif // FIT_ELLIPSE_H
