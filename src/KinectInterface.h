/****************************************************************************
* 
*  Computer Vision, Fall 2011
*  New York University
*
*  Created by Otavio Braga (obraga@cs.nyu.edu)
*
****************************************************************************/

#ifndef KINECT_INTERFACE_H
#define KINECT_INTERFACE_H

#include <deque>

#include <opencv/cv.h>

#include <XnCppWrapper.h>
#include <XnVNite.h>

class KinectInterface
{
    public:
        KinectInterface();
    
        ~KinectInterface()
        {
            // Clean up
            context.Shutdown();
        }

        static void updateKinectData();

        static IplImage *getRGB() { return 0; }
        static IplImage *getDepthMap() { return 0; };

        static std::vector<cv::Vec3d> &getJoints() { return joints; }
        static std::vector<cv::Vec2d> &getProjectedJoints() { return joints_projected; }

        static xn::Context& getContext() { return context; }

        static xn::ImageGenerator& getImageGenerator() { return g_ImageGenerator; }
        static xn::DepthGenerator& getDepthGenerator() { return g_DepthGenerator; }

    protected:
        bool Init();

    private:
        IplImage *ConvertKinectImageToOpenCV(xn::ImageGenerator *imageGenerator,
                                             xn::UserGenerator *userGenerator);

        static void XN_CALLBACK_TYPE User_NewUser(xn::UserGenerator& generator,
                                                  XnUserID nId, void *PCookie);

        static void XN_CALLBACK_TYPE User_LostUser(xn::UserGenerator& generator,
                                                   XnUserID nId, void *pCookie);

        static void XN_CALLBACK_TYPE Pose_Detected(xn::PoseDetectionCapability& pose, 
                                                   const XnChar *strPose,
                                                   XnUserID nId, void *pCookie);

        static void XN_CALLBACK_TYPE Calibration_Start(xn::SkeletonCapability& capability,
                                                       XnUserID nId, void *pCookie);

        static void XN_CALLBACK_TYPE Calibration_End(xn::SkeletonCapability& capability,
                                                     XnUserID nId, XnBool bSuccess,
                                                     void *pCookie);
    
        static xn::DepthGenerator g_DepthGenerator;
        static xn::ImageGenerator g_ImageGenerator;
        static xn::UserGenerator g_UserGenerator;
    
        static xn::Context context;

        static std::vector<cv::Vec3d> joints;
        static std::vector<cv::Vec2d> joints_projected;
};

#endif // KINECT_INTERFACE_H
