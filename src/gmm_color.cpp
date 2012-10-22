/****************************************************************************
* 
*  Computer Vision, Fall 2011
*  New York University
*
*  Created by Otavio Braga (obraga@cs.nyu.edu)
*
****************************************************************************/

#include <iostream>
#include <stdlib.h>
#include <numeric>
#include <stdio.h>
#include <vector>
#include <math.h>
#include "kmeans_color.h"
#include "gmm_color.h"
#include <opencv2/core/core.hpp>

using namespace std;

#ifndef M_PI
#define M_PI 3.141592653589
#endif


void gmm_color(unsigned char *rgbImage, int npts,
               std::vector<cv::Vec3d> &mean,			// sample mean of each gaussian
               std::vector<cv::Matx33d> &cov,			// sample covariance matrices
               std::vector<double> &pi,					// mixing coefficient
               std::vector<cv::Matx33d> &inv_cov,		// inverses of covariance
               std::vector<double> &det_cov,			// determinants of covariance matrice
               unsigned char *cluster,
               unsigned char *trimap, unsigned char label)
{
    // !!!!!!!!!!!!!!!!!!!! Implement this !!!!!!!!!!!!!!!!!!!!
    //std::cout << "Warning: gmm_color not implemented!\n";
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

	// Initialization
	int nclusters = mean.size();
	double *count = new double[nclusters];
	double sum[nclusters][3][3];
	for(int i=0; i<nclusters; i++){
		count[i] = 0;
		pi[i] = 0;

		for(int j=0; j<3; j++)
			for(int k=0; k<3; k++)
				sum[i][j][k] = 0;
	}

	//The mixing coefficients can be estimated
 	//simply as the proportion of pixels that belong to each component.

/*
	// pi
	double total_count = 0;
	for(int i=0; i<npts; i++){
		if(trimap[i] == label){
			total_count++;
			pi[cluster[i]]++;
		}
	}
	//if(total_count == 0) return;
	for(int i=0; i<nclusters; i++){
		if(total_count != 0)
			pi[i] /= total_count;
	}
*/
		
	// pi & covariance
	double total_count = 0;
	double dist[3];
	for(int i=0; i<npts; i++){
		if(trimap[i] == label){
			
			// pi
			total_count++;
			pi[cluster[i]]++;

			// covariance
			count[cluster[i]]++;
			for(int j=0; j<3; j++)
				dist[j]= (double)rgbImage[i*3+j] - mean[cluster[i]][j];
			for(int m=0; m<3; m++)
				for(int n=0; n<3; n++)
					sum[cluster[i]][m][n] += dist[m]*dist[n];
		}
	}
	
	if(total_count == 0)
		return;
	
	for(int i=0; i<nclusters; i++){

		// pi
		pi[i] /= total_count;

		// covariance
		if(count[i] != 0){
			for(int m=0; m<3; m++){
				for(int n=0; n<3; n++){
					cov[i](m, n) = sum[i][m][n] / count[i];
				}
			}
		}
	}

	//  determinants of covariance matrice & invert of covariance matrice
	for(int i=0; i<nclusters; i++){
		det_cov[i] = cv::determinant(cov[i]);
		cv::invert(cov[i], inv_cov[i]);
	}
}

