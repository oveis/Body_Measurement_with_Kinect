/****************************************************************************
* 
*  Computer Vision, Fall 2011
*  New York University
*
*  Created by Otavio Braga (obraga@cs.nyu.edu)
*
****************************************************************************/

#include <iostream>
#include "threshold.h"

void threshold_depth_map(unsigned short *depthimg,
                         int npts,
                         double threshold,
                         bool *foreground)
{    
    // !!!!!!!!!!!!!!!!!!!! Implement this !!!!!!!!!!!!!!!!!!!!
    //std::cout << "Warning: threshold_depth_map not implemented!\n";
	for(int i=0; i< npts; i++){
		if(depthimg[i] < threshold)
			foreground[i] = true;
		else
			foreground[i] = false;
	}
}

