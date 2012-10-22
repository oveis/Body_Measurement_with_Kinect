/****************************************************************************
* 
*  Computer Vision, Fall 2011
*  New York University
*
*  Created by Otavio Braga (obraga@cs.nyu.edu)
*
****************************************************************************/

#ifndef GAUSSIAN_MIXTURE_SEGMENTATION_H
#define GAUSSIAN_MIXTURE_SEGMENTATION_H

// Evaluates a gaussian distribution at point x with mean mu and standard
// deviation sigma.
double gaussian(double x, double mu, double sigma);

/*
 * Model the depth distribution as a mixture of gaussian and use it to
 * cluster the data in foreground/background.
 *
 *     P(d;mu1,sigma1,mu2,sigma2,p) = p*N(d;mu1,sigma1) + (1-p)*N(d;mu2,sigma2)
 *
 * Returns the depth that separates the two clusters.
 *
 * Parameters:
 *
 *     - gamma:
 *           Pre-allocated array with the same size as depthimage (npts).
 *           Used to store the responsibilites during the EM 'E' step.
 *     - foreground: Set to true for pixels on the foreground.
 *     - mu1, sigma1, mu2, sigma2:
 *           If not NULL, is set to the gaussian components mean and standard
 *           deviation.
 *     - p: If not NULL, is set to the mixing coefficient.
 */ 
float gaussian_mixture_segmentation(unsigned short *depthimage,
                                    int npts,
                                    float *gamma,
                                    bool *foreground,
                                    double *mu1 = 0, double *sigma1 = 0,
                                    double *mu2 = 0, double *sigma2 = 0,
                                    double *p = 0);

#endif // GAUSSIAN_MIXTURE_SEGMENTATION_H
