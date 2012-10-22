/****************************************************************************
* 
*  Computer Vision, Fall 2011
*  New York University
*
*  Created by Otavio Braga (obraga@cs.nyu.edu)
*
****************************************************************************/

#include <iostream>
#include <stdio.h>
#include <vector>
#include "mincut_segmentation.h"
#include "kmeans_color.h"
#include <opencv2/core/core.hpp>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include "gmm_color.h"
#include "gmm_segmentation.h"
#include "histogram.h"
#include "KinectInterface.h"
#include "kmeans_segmentation.h"
#include "threshold.h"

using namespace std;

double distance(unsigned char* rgbImage, int i, cv::Vec3d x){
	return pow(rgbImage[i*3]-x[0],2)+pow(rgbImage[i*3+1]-x[1],2)+pow(rgbImage[i*3+2]-x[2],2);
}

void k_means_color(unsigned char *rgbImage, int npts, int nclusters,
                   std::vector<cv::Vec3d> &centroids,
                   unsigned char *cluster,
                   unsigned char *mask, unsigned char label)
{
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
 * cluster: a pre-allocated buffer where you indicate the
 *          cluster assignment of each pixel inside the region.
 *          Don't modify the pixels outside the region.
 *
 * trimap and label: Only consider the pixels i where trimap[i] == label.
 *
 * !!!!!!!!!!!!!!!!!!!! Implement this !!!!!!!!!!!!!!!!!!!!
 */
//	k_means_color((unsigned char *)rgbImage, npts,
//               n_color_clusters, mean[a], cluster, trimap, a);
	
    //std::cout << "Warning: k_means_color not implemented!\n";

// mean : centroids : 각각의 클러스터에서 각각의 색(rgb)에 대한 평균값을 입력하는 곳

	// Initialization
	double rgb_min[3], rgb_max[3];
	bool is_change = true;
	std::vector<cv::Vec3d> pre_centroids;
	pre_centroids.resize(nclusters);
	cv::Vec3d sum[nclusters];	
	double count[nclusters];

	for (int i=0; i<3; i++){
		rgb_min[i] = 255;
		rgb_max[i] = 0;
	}

	// centroids is empty
	if(centroids.size() != nclusters){
		centroids.resize(nclusters);
		for(int i=0; i<npts; i++){
			// Searching depth_min / depth_max value
			if(mask[i] == label){
				for(int j=0; j<3; j++){
					if(rgbImage[i*3+j] < rgb_min[j])
						rgb_min[j] = rgbImage[i*3+j];
					if(rgbImage[i*3+j] > rgb_max[j])
						rgb_max[j] = rgbImage[i*3+j];
				}
			}
		}
	
		// Initial centroids
		for(int i=0; i<nclusters; i++)
			for(int j=0; j<3; j++)
				centroids[i][j] = i*(rgb_max[j]-rgb_min[j])/nclusters + rgb_min[j];
		
	} 

	// Centroid Estimate
	int index;
	double distance_min, distance_comp;
	while(is_change){
		
		// Initialization
		for(int i=0; i<nclusters; i++){		
			sum[i] = 0;
			count[i] = 0;
			for(int j=0; j<3; j++)
				pre_centroids[i][j] = centroids[i][j];
		}
				
		for(int i=0; i<npts; i++){
			if(mask[i] == label){	
				distance_min = distance(rgbImage, i, centroids[0]);
				index = 0;

				for(int t=0; t<nclusters; t++){
					distance_comp = distance(rgbImage, i, centroids[t]);
					if(distance_min > distance_comp){
						index = t;
						distance_min = distance_comp;
					}
				}
				cluster[i] = index;
				count[index] ++;
				for(int k=0; k<3; k++)
					sum[index][k] += rgbImage[i*3+k];
			}
		}
		
		// New Centroid
		for(int i=0; i<nclusters; i++){
			if(count[i] != 0){
				for(int t=0; t<3; t++){
					centroids[i][t] = sum[i][t] / count[i];	//중요
				}
			}
		}
		
//
/*
		cout << endl;
		for(int i=0; i<nclusters; i++){
			cout << "pre_centroids[" << i << "] [0] : " << pre_centroids[i][0] << " [1] : " << pre_centroids[i][1] << " [2] : " << pre_centroids[i][2] << endl;
			cout << "centroids [" << i << "] [0] : " << centroids[i][0] << " [1] : " << centroids[i][1] << " [2] : " << centroids[i][2] << endl;
		}
*/
//
		// 소수점에서 않맞아서 계속 비교하므로 차이가 1보다 작으면 통과시키자.
		// Compare pre vs cur  => for loop interrup
		double diff;
		bool check = true;
		for(int i=0; i<nclusters; i++){

			for(int j=0; j<3; j++){
				diff = pre_centroids[i][j] - centroids[i][j];
				if(diff>1 || diff<-1){
					check = false;
					break;
				}
			}
			if(!check)
				break;
			if(i==nclusters-1 && check)
				is_change = false;
/*
			if(pre_centroids[i] != centroids[i]){
				is_change = true;
				break;
			} else{
				is_change = false;
				break;
			}
*/
		}
	}
}
