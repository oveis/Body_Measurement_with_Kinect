/****************************************************************************
* 
*  Computer Vision, Fall 2011
*  New York University
*
*  Created by Otavio Braga (obraga@cs.nyu.edu)
*
****************************************************************************/

#ifndef THRESHOLD_SEGMENTATION_H
#define THRESHOLD_SEGMENTATION_H


// Set foreground[i] to true for the pixels with depth smaller than 'threshold'
// !!!!!!!!!! Implement this !!!!!!!!!
void threshold_depth_map(unsigned short *depthimg,
                         int npts,
                         double threshold,
                         bool *foreground);

#endif // THRESHOLD_SEGMENTATION_H
