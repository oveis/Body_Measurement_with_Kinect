/****************************************************************************
* 
*  Computer Vision, Fall 2011
*  New York University
*
*  Created by Otavio Braga (obraga@cs.nyu.edu)
*
****************************************************************************/

#ifndef GMM_COLOR_H
#define GMM_COLOR_H

#include <opencv2/core/core.hpp>

/*
 * Model the color distribution of the foreground/background region with a
 * GMM.
 *
 * Instead of using the full EM algorithm for estimation, use the cluster
 * assignment of each pixel in the 'cluster' array, which gives, for each
 * pixel, the GMM it was assigned to. The mixing coefficients can be estimated
 * simply as the proportion of pixels that belong to each component.
 *
 * mean must be filled with the sample mean of each gaussian, and cov with the
 * sample covariance matrices. Moreover, for efficiency purposes, also fill in
 * inv_cov and det_cov with the inverses and determinants of the covariance
 * matrices, respectively.
 * 
 * trimap, label: Take only into account the pixels i for which trimap[i] == label.
 *
 */
void gmm_color(unsigned char *rgbImage, int npts,
               std::vector<cv::Vec3d> &mean,
               std::vector<cv::Matx33d> &cov,
               std::vector<double> &pi,
               std::vector<cv::Matx33d> &inv_cov,
               std::vector<double> &det_cov,
               unsigned char *cluster,
               unsigned char *trimap, unsigned char label);

#endif // GMM_COLOR_H
