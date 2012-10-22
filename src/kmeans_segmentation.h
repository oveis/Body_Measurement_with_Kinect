/****************************************************************************
* 
*  Computer Vision, Fall 2011
*  New York University
*
*  Created by Otavio Braga (obraga@cs.nyu.edu)
*
****************************************************************************/

#ifndef K_MEANS_SEGMENTATION_H
#define K_MEANS_SEGMENTATION_H

/**
 * Segments a depth map with k-means (k = 2) clustering. The result is
 * stored in foreground, which is a pre-allocated array assumed to be 
 * big enough to hold the result.
 *
 * Returns the threshold value that separates foreground and background
 * depths, i.e., the mean of the two clusters centroids. Moreover, if
 * centroid1 and centroid2 are not NULL, return the cluster centroids in
 * them.
 *
 *
 * !!!!!!!!!!!!!!!!!!!! Implement this !!!!!!!!!!!!!!!!!!!!
 */
float k_means_segmentation(unsigned short *depthimage,
                           int npts,
                           bool *foreground,
                           double *centroid1 = 0, double *centroid2 = 0);

#endif // K_MEANS_SEGMENTATION_H
