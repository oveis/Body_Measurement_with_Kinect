/****************************************************************************
* 
*  Computer Vision, Fall 2011
*  New York University
*
*  Created by Otavio Braga (obraga@cs.nyu.edu)
*
****************************************************************************/

#ifndef MINCUT_SEGMENTATION_H
#define MINCUT_SEGMENTATION_H

#include <opencv2/core/core.hpp>
/*
 * Assign each pixel to the GMM components with highest probability.
 *
 * For pixel i, use the GMM indexed by alpha[i] to tell if it belongs to the
 * foreground or background, i.e., if pixel i belongs to the foreground,
 * alpha[i] will be 1, and the gaussian mixture parameters will be in mean[1],
 * cov[1], pi[1], etc...
 *
 * - mean and cov hold the gaussians mean and covariance matrix, respectively.
 * - pi holds the mixing coefficients.
 * - inv_cov: is the inverse of the covariance matrix (provided here to
 *   save computations, so you don't have to invert a matrix everytime
 *   you need to evaluate the energy function.
 * - det_cov: the determinant of the covariance matrix. Likewise, provided
 *   to save computations.
 *
 * The result of the assignment should be stored in 'component', which is
 * a buffer assumed big enough to hold the result. If pixel i is
 * assigned to component k, i.e., component k has the highest probability
 * (or lowest energy) given the pixel color and foreground/background
 * assignment, then component[i] must be set to k.
 *
 * !!!!!!!!!!!!!!!!!!!! Implement this !!!!!!!!!!!!!!!!!!!!
 */
void assign_gmm_component(unsigned char *rgbImage, int npts,
                          bool *alpha,
                          std::vector<cv::Vec3d> mean[2],
                          std::vector<cv::Matx33d> cov[2],
                          std::vector<double> pi[2],
                          std::vector<cv::Matx33d> inv_cov[2],
                          std::vector<double> det_cov[2],
                          unsigned char *component);

enum
{
    TRIMAP_BG,
    TRIMAP_FG,
    TRIMAP_U,
};

/**
 * Segments a depth map with mincut. The result is stored in alpha, which is
 * assumed to be pre-allocated and big enough to hold the result.
 *
 * trimap: Indicates which pixels where already assigned to the foreground
 *         (trimap[i] == TRIMAP_FG) or background (trimap[i] == TRIMAP_BG).
 *         Your solution must preserve this assignment. Solve only for the
 *         pixels that have not been assigned yet (trimap[i] == TRIMAP_U).
 *
 * alpha: This is were the result of the algorithm is stored, set to false
 *        for background, and true for the foreground.
 *
 * component: buffer used to hold the GMM component each pixel is assigned to.
 *
 * K: The number of components in the GMM for the foreground and background.
 *
 * mean, cov, pi, inv_cov, det_cov:
 *      The parameters of the GMMs. mean[0], cov[0], * etc.. are the parameters 
 *      of the background GMM, and mean[1], cov[1], etc..., the foreground.
 *
 *      If mean[0].size() == K, it means the components have already been
 *      initialized in previous frame, so you can use it as initialization
 *      for the current frame. Otherwise, you should initialize the GMMs
 *      parameters.      
 *
 * !!!!!!!!!!!!!!!!!!!! Implement this !!!!!!!!!!!!!!!!!!!!
 */

void mincut_segmentation(unsigned char *rgbImage,
                         int width, int height,
                         unsigned char *trimap,
                         bool *alpha,
                         unsigned char *component,
                         int K,                         
                         std::vector<cv::Vec3d> mean[2],
                         std::vector<cv::Matx33d> cov[2],
                         std::vector<double> pi[2],
                         std::vector<cv::Matx33d> inv_cov[2],
                         std::vector<double> det_cov[2],
			int gamma, int user_filter);

#endif // MINCUT_SEGMENTATION_H
