/****************************************************************************
* 
*  Computer Vision, Fall 2011
*  New York University
*
*  Created by Otavio Braga (obraga@cs.nyu.edu)
*
****************************************************************************/

#include <iostream>
#include <math.h>
#include <stdio.h>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "kmeans_color.h"
#include "gmm_color.h"

#include "mincut_segmentation.h"
#include <graph.h>

#include "KinectInterface.h"

using namespace std;

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
// 만약 작동이 안되면, mean and cov hold the gaussians mean and covariance matrix를 의심해 볼것
/*
double cal_energy(unsigned char *rgbImage, int i, cv::Vec3d mean, double pi, cv::Matx33d inv_cov, double det_cov)
{
	cv::Matx13d dist_t(rgbImage[i*3]-mean[0], rgbImage[i*3+1]-mean[1], rgbImage[i*3+2]-mean[2]);
	cv::Matx31d dist(rgbImage[i*3]-mean[0], rgbImage[i*3+1]-mean[1], rgbImage[i*3+2]-mean[2]);

	return -log(pi) + (dist_t*inv_cov*dist)(0,0)/2.0 + log(det_cov)/2.0;
}
*/

double cal_energy(unsigned char *rgbImage, int i, cv::Vec3d mean, cv::Matx33d inv_cov, double log_pi_det)
{
	cv::Matx13d dist_t(rgbImage[i*3]-mean[0], rgbImage[i*3+1]-mean[1], rgbImage[i*3+2]-mean[2]);
	cv::Matx31d dist(rgbImage[i*3]-mean[0], rgbImage[i*3+1]-mean[1], rgbImage[i*3+2]-mean[2]);

	return (dist_t*inv_cov*dist)(0,0)/2.0 + log_pi_det;
}

void assign_gmm_component(unsigned char *rgbImage, int npts,
                          bool *alpha,						// foreground / background[npts]
                          std::vector<cv::Vec3d> mean[2],			// mean
                          std::vector<cv::Matx33d> cov[2],			// covariance matrix
                          std::vector<double> pi[2],				// mixing coefficient
                          std::vector<cv::Matx33d> inv_cov[2],			// inverse of the covariance matrix
                          std::vector<double> det_cov[2],			// determinant of the covariance matrix
                          unsigned char *component)				// save here
{        
    // !!!!!!!!!!!!!!!!!!!! Implement this !!!!!!!!!!!!!!!!!!!!
    //std::cout << "Warning: assign_gmm_component not implemented!\n";
	double energy_min, energy_temp;
	int i, j;

	// -log(pi) + (1/2)log(det) = log(pi^(-1))(det^(1/2))
	double log_pi_det[2][4];
	for(int i=0; i<2; i++)
		for(int j=0; j<4; j++)
			log_pi_det[i][j] = log(sqrt(det_cov[i][j]) / pi[i][j]);	

	// Energy compare
	for(i=0; i<npts; i++){
		energy_min = cal_energy(rgbImage, i, mean[alpha[i]][0], 
								inv_cov[alpha[i]][0], log_pi_det[alpha[i]][0]);
		component[i] = 0;
		
		for(j=1; j<mean[alpha[i]].size(); j++){
			energy_temp = cal_energy(rgbImage, i, mean[alpha[i]][j],
									inv_cov[alpha[i]][j], log_pi_det[alpha[i]][j]);
			if(energy_temp < energy_min){
				component[i] = j;
				energy_min = energy_temp;
			}
			
		}
		
	}
}

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

double distance(unsigned char *rgbImage, int index, int index_temp)
{
	double dist = pow(rgbImage[index*3]-rgbImage[index_temp*3],2)
					+ pow(rgbImage[index*3+1]-rgbImage[index_temp*3+1],2)
					+ pow(rgbImage[index*3+2]-rgbImage[index_temp*3+2],2);
	return dist;
}

double cal_weight(unsigned char *rgbImage, int i, int j, int gamma, int beta){
	return gamma*exp(-beta * (pow(rgbImage[i*3]-rgbImage[j*3],2)+pow(rgbImage[i*3+1]-rgbImage[j*3+1],2)+pow(rgbImage[i*3+2]-rgbImage[j*3+2],2)));
}

void mincut_segmentation(unsigned char *rgbImage,
                         int width, int height,
                         unsigned char *trimap,
                         bool *alpha,                   
                         unsigned char *component,
                         int K,									// nclusters
                         std::vector<cv::Vec3d> mean[2],
                         std::vector<cv::Matx33d> cov[2],
                         std::vector<double> pi[2],
                         std::vector<cv::Matx33d> inv_cov[2],
                         std::vector<double> det_cov[2],
						int gamma, int user_filter)
{
	// Calculation about beta
	double beta = 0;
	int index, index_temp;
	double dist_sum;
	double count = 0;

	for(int i=0; i<width; i++){
		for(int j=0; j<height; j++){
			index = i + j*width;
			for(int m_i=-1; m_i<2; m_i++){
				for(int m_j=-1; m_j<2; m_j++){
					if(m_i !=0 && m_j != 0)
						if(m_i == 0 || m_j == 0)
							if(m_i+i>=0 && m_i+i<width && m_j+j>=0 && m_j+j<height){
								index_temp = (m_i+i) + (m_j+j)*width;
								beta += distance(rgbImage, index, index_temp);
								count++;
							}
				}
			}
		}
	}
	beta /= count;

	// Initialization
	Graph::node_id nodes[width*height];
	Graph *graph = new Graph();	
	double energy_min[2];
	double energy, energy_temp;
	double weight;
	for(int i=0; i<width; i++)
		for(int j=0; j<height; j++)
			nodes[i+j*width] = graph->add_node();

	// -log(pi) + (1/2)log(det) = log(pi^(-1))(det^(1/2))
	double log_pi_det[2][4];
	for(int i=0; i<2; i++)
		for(int j=0; j<4; j++)
			log_pi_det[i][j] = log(sqrt(det_cov[i][j]) / pi[i][j]);
	

	for(int i=0; i<width; i++){
		for(int j=0; j<height; j++){
			index = i + j*width;
			if(trimap[index] == TRIMAP_BG)
				graph->set_tweights(nodes[index], 10000000, 0);
			else if(trimap[index] == TRIMAP_FG)
				graph->set_tweights(nodes[index], 0, 10000000);
			else{
				for(int k=0; k<2; k++){
					energy_min[k] = cal_energy(rgbImage, index, mean[k][0], inv_cov[k][0], log_pi_det[k][0]);
					for(int t=1; t<mean[k].size(); t++){
						energy_temp = cal_energy(rgbImage, index, mean[k][t], inv_cov[k][t], log_pi_det[k][t]);
						if(energy_temp < energy_min[k])
							energy_min[k] = energy_temp;
					}
				}
				graph->set_tweights(nodes[index], energy_min[0], energy_min[1]);
				for(int m_i=-1; m_i<2; m_i++){
					for(int m_j=-1; m_j<2; m_j++){
						if(m_i !=0 && m_j != 0)
							// if(m_i == 0 || m_j == 0)
								if(m_i+i>=0 && m_i+i<width && m_j+j>=0 && m_j+j<height){
									index_temp = (m_i+i) + (m_j+j)*width;
									weight = cal_weight(rgbImage, index, index_temp, gamma, beta);
									graph->add_edge(nodes[index], nodes[index_temp], weight, weight);
								}
					}
				}
			}
		}
	}
	
	Graph::flowtype flow = graph->maxflow();
	int bf;
	for(int i=0; i<width; i++){
		for(int j=0; j<height; j++){
			if(trimap[i+j*width] == TRIMAP_U){
				index = i+j*width;

				if(graph->what_segment(nodes[index]) == Graph::SOURCE){
					alpha[index] = true;
					bf = 1;
				} else{
					alpha[index] = false;
					bf = 0;
				}
				energy = cal_energy(rgbImage, index, mean[bf][0], 
									inv_cov[bf][0], log_pi_det[bf][0]);
				component[index] = 0;
				for(int k=1; k<mean[bf].size(); k++){
					energy_temp = cal_energy(rgbImage, index, mean[bf][k], 
											inv_cov[bf][k], log_pi_det[bf][k]);
					if(energy_temp < energy){
						energy = energy_temp;
						component[index] = k;
					}
				}
				
			}
		}
	}

	// filtering
	// using 5 X 5 and if background is more than foreground, this middle point will be backpoint.
	int count_filt_b=0, count_filt_f=0;
	int index_filt;
	for(int i=user_filter; i<width-user_filter; i++){
		for(int j=user_filter; j<height-user_filter; j++){
			count_filt_b = count_filt_f = 0;
			index_filt = i + j*width;
			for(int m=-user_filter; m<=user_filter; m++){
				for(int n=-user_filter; n<=user_filter; n++){
					//if(index_filt+m>=0 && index_filt+m<width && index_filt+n*width>=0 && index_filt+n*width<height){
						if(alpha[index_filt + m + n*width])
							count_filt_f++;
						else
							count_filt_b++;
					//}
				}
			}
			if(count_filt_f > count_filt_b)
				alpha[index_filt] = true;
			else if(count_filt_f < count_filt_b)
				alpha[index_filt] = false;
			else
				cout << "같음" << endl;
		}
	}
					

	delete graph;
}
