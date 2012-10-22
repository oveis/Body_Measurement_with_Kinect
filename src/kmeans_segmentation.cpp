/****************************************************************************
* 
*  Computer Vision, Fall 2011
*  New York University
*
*  Created by Otavio Braga (obraga@cs.nyu.edu)
*
****************************************************************************/

#include <math.h>
#include <iostream>

#include "kmeans_segmentation.h"

using namespace std;

float k_means_segmentation(unsigned short *depthimage,
                           int npts,
                           bool *foreground,
                           double *centroid_fg, double *centroid_bg)
{

        // Segments a depth map with k-means (k = 2) clustering. The result is
        // stored in foreground, which is a pre-allocated array assumed to be 
        // big enough to hold the result.
        //
        // Returns the threshold value that separates foreground and background
        // depths, i.e., the mean of the two clusters centroids. Moreover, if
        // centroid1 and centroid2 are not NULL, return the cluster centroids in
        // them.
        //
        //
    // !!!!!!!!!!!!!!!!!!!! Implement this !!!!!!!!!!!!!!!!!!!!
	unsigned short depth_min, depth_max;
	unsigned short cur_centroid_fg, cur_centroid_bg, pre_centroid_fg, pre_centroid_bg;
	for(int i=0; i<npts; i++)
		if(depthimage[i] != 0)
			depth_min = depth_max = depthimage[i];

	for(int i=0; i<npts; i++){
		if(depthimage[i] == 0)
			continue;
		else{
			if(depthimage[i] < depth_min)
				depth_min = depthimage[i];
			if(depthimage[i] > depth_max)
				depth_max = depthimage[i];
		}
	}

	int count_0 = 0;

	cur_centroid_fg = depth_min;
	cur_centroid_bg = depth_max;
	double comp1, comp2;
	double sum_fg, sum_bg;
	bool is_change = true;
	int count_fg, count_bg;
	count_fg = count_bg = 0;

	while(is_change){
		sum_fg = sum_bg = 0.0;
		count_fg = count_bg = 0;
		pre_centroid_fg = cur_centroid_fg;
		pre_centroid_bg = cur_centroid_bg;


		for(int i=0; i<npts; i++){
			if(depthimage[i] != 0){	
				comp1 = pow(cur_centroid_fg-depthimage[i],2);
				comp2 = pow(cur_centroid_bg-depthimage[i],2);
			
				if(comp1 <= comp2)
					foreground[i] = true;
				else
					foreground[i] = false;		
			}	
		}

		for(int i=0; i<npts; i++){
			if(depthimage[i] != 0){
				if(foreground[i]){
					sum_fg += depthimage[i];
					count_fg++;
				}
				else{
					sum_bg += depthimage[i];
					count_bg++;
				}
			}
		}
	
		if(count_fg == 0)
			cur_centroid_fg = pre_centroid_fg;
		else
			cur_centroid_fg = (unsigned short)(sum_fg / count_fg);
		if(count_bg == 0)
			cur_centroid_bg = pre_centroid_bg;
		else
			cur_centroid_bg = (unsigned short)(sum_bg / count_bg);		
			
		if(( pre_centroid_fg == cur_centroid_fg) && (pre_centroid_bg == cur_centroid_bg))
			is_change = false;
	}

	if (centroid_fg)
	        *centroid_fg = cur_centroid_fg;

	if (centroid_bg)
        	*centroid_bg = cur_centroid_bg;

	if(centroid_fg && centroid_bg){
		return (cur_centroid_fg + cur_centroid_bg)/2;	
	}
	else
		return 0.0;
}
