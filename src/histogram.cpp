/****************************************************************************
* 
*  Computer Vision, Fall 2011
*  New York University
*
*  Created by Otavio Braga (obraga@cs.nyu.edu)
*
****************************************************************************/

#include "histogram.h"

void compute_histogram(unsigned short *depthimage,
                       int npts,
                       histogram &hist,
                       bool normalize)
{
    hist.clear();

    for (int i = 0; i < npts; i++)
        hist.insert_point(depthimage[i]);

    if (normalize)
        hist.normalize();
}
