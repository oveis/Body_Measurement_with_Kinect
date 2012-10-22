/****************************************************************************
* 
*  Computer Vision, Fall 2011
*  New York University
*
*  Created by Otavio Braga (obraga@cs.nyu.edu)
*
****************************************************************************/

#ifndef KMEANS_COLOR_H
#define KMEANS_COLOR_H

#include <opencv2/core/core.hpp>

/*
 * Compute k-means in color space for the pixels in trimap with the
 * given label.
 *
 * centroids: Should be filled with the centroids of each cluster. Note that
 *            if centroids is not empty, you can use the values in them
 *            for initialization. Since we are calling this method for
 *            consecutive frames, there should be temporal coherence
 *            in the color clusters.
 *
 * cluster: is a pre-allocated buffer where you indicate the
 *          cluster assignment of each pixel inside the region.
 *          Don't modify the pixels outside the region!
 *
 * trimap and label: Only consider the pixels i where trimap[i] == label.
 *
 * !!!!!!!!!!!!!!!!!!!! Implement this !!!!!!!!!!!!!!!!!!!!
 */
void k_means_color(unsigned char *rgbImage, int npts, int nclusters,
                   std::vector<cv::Vec3d> &centroids,
                   unsigned char *cluster,
                   unsigned char *trimap = 0, unsigned char label = 1);

#endif // KMEANS_COLOR_H
