/****************************************************************************
* 
*  Computer Vision, Fall 2011
*  New York University
*
*  Created by Otavio Braga (obraga@cs.nyu.edu)
*
****************************************************************************/

#include <fstream>
#include <iostream>

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "KinectInterface.h"
#include "Skeleton.h"

using namespace std;

#define CALIBRATION_POSE "Psi"

xn::ImageGenerator KinectInterface::g_ImageGenerator;
xn::DepthGenerator KinectInterface::g_DepthGenerator;
xn::UserGenerator KinectInterface::g_UserGenerator;
xn::Context KinectInterface::context;
        
std::vector<cv::Vec3d> KinectInterface::joints;
std::vector<cv::Vec2d> KinectInterface::joints_projected;

KinectInterface::KinectInterface()
{
    cout << "Initializing KinectInterface...\n";
    if (!Init())
        exit(-1);
    cout << "done\n";
}

#ifdef __APPLE__
//#define SAMPLE_XML_PATH "/Users/otavio/libs/OpenNI-Bin-MacOSX-v1.3.2.3/Samples/Config/SamplesConfig.xml"
#define SAMPLE_XML_PATH "SamplesConfig.xml"
#else
//#define SAMPLE_XML_PATH "F:\\Program Files (x86)\\OpenNI\\Samples\\Config\\SamplesConfig.xml"
#define SAMPLE_XML_PATH "SamplesConfig.xml"
#endif

void XN_CALLBACK_TYPE KinectInterface::User_NewUser(xn::UserGenerator& generator,
                                                 XnUserID nId, void *PCookie)
{
    printf("New User: %d\n", nId);
    g_UserGenerator.GetPoseDetectionCap().StartPoseDetection(CALIBRATION_POSE, nId);
}

void XN_CALLBACK_TYPE KinectInterface::User_LostUser(xn::UserGenerator& generator,
                                                  XnUserID nId, void *pCookie)
{
}

void XN_CALLBACK_TYPE KinectInterface::Pose_Detected(xn::PoseDetectionCapability& pose, 
                                                  const XnChar *strPose,
                                                  XnUserID nId, void *pCookie)
{
    printf("Pose %s for user %d\n", strPose, nId);
    g_UserGenerator.GetPoseDetectionCap().StopPoseDetection(nId);
    g_UserGenerator.GetSkeletonCap().RequestCalibration(nId, TRUE);
}

void XN_CALLBACK_TYPE KinectInterface::Calibration_Start(xn::SkeletonCapability& capability,
                                                      XnUserID nId, void *pCookie)
{
    printf("Starting calibration for user %d\n", nId);
}

void XN_CALLBACK_TYPE KinectInterface::Calibration_End(xn::SkeletonCapability& capability,
                                                    XnUserID nId, XnBool bSuccess,
                                                    void *pCookie)
{
    if (bSuccess)
    {
        printf("User calibrated\n");
        g_UserGenerator.GetSkeletonCap().StartTracking(nId);
    }
    else
    {
        printf("Failed to calibrate user %d\n", nId);
        g_UserGenerator.GetPoseDetectionCap().StartPoseDetection(CALIBRATION_POSE,
                                                                 nId);
    }
}

bool KinectInterface::Init()
{
    XnStatus nRetVal = XN_STATUS_OK;

    /*nRetVal = context.Init();
    if (nRetVal != XN_STATUS_OK)
    {
        const XnChar *str = xnGetStatusString(nRetVal);
        cerr << str;
        exit(-1);
    }*/

    xn::EnumerationErrors errors;
    nRetVal = context.InitFromXmlFile(SAMPLE_XML_PATH, &errors);
    if (nRetVal == XN_STATUS_NO_NODE_PRESENT)
    {
        XnChar strError[1024];
        errors.ToString(strError, 1024);
        printf("%s\n", strError);
        return false;
    }
    else if (nRetVal != XN_STATUS_OK)
    {
        const XnChar *str = xnGetStatusString(nRetVal);
        cerr << str;
        return false;
    }

    nRetVal = context.FindExistingNode(XN_NODE_TYPE_DEPTH, g_DepthGenerator);
    if (nRetVal != XN_STATUS_OK)
    {
        const XnChar *str = xnGetStatusString(nRetVal);
        cerr << str;

        // Create the depth generator
        nRetVal = g_DepthGenerator.Create(context);
        if (nRetVal != XN_STATUS_OK)
        {
            const XnChar *str = xnGetStatusString(nRetVal);
            cerr << str;
            return false;
        }
    }

    nRetVal = context.FindExistingNode(XN_NODE_TYPE_IMAGE, g_ImageGenerator);
    if (nRetVal != XN_STATUS_OK)
    {
        const XnChar *str = xnGetStatusString(nRetVal);
        cerr << str;

        // Create the image generator
        nRetVal = g_ImageGenerator.Create(context);
        if (nRetVal != XN_STATUS_OK)
        {
            const XnChar *str = xnGetStatusString(nRetVal);
            cerr << str;
            return false;
        }
    }
    
    nRetVal = context.FindExistingNode(XN_NODE_TYPE_USER, g_UserGenerator);
    if (nRetVal != XN_STATUS_OK)
    {
        const XnChar *str = xnGetStatusString(nRetVal);
        cerr << str;

        // Create the user generator
        nRetVal = g_UserGenerator.Create(context);
        if (nRetVal != XN_STATUS_OK)
        {
            const XnChar *str = xnGetStatusString(nRetVal);
            cerr << str;
            return false;
        }
    }

    if (!g_UserGenerator.IsCapabilitySupported(XN_CAPABILITY_SKELETON))
    {
        printf("Supplied user generator doesn't support skeleton\n");
        return false;
    }

    XnCallbackHandle h1, h2, h3;
    
    g_UserGenerator.RegisterUserCallbacks(
        &KinectInterface::User_NewUser,
        &KinectInterface::User_LostUser,
        NULL, h1);

    g_UserGenerator.GetPoseDetectionCap().RegisterToPoseCallbacks(
        &KinectInterface::Pose_Detected,
        NULL, NULL, h2);
    
    g_UserGenerator.GetSkeletonCap().RegisterCalibrationCallbacks(
        &KinectInterface::Calibration_Start,
        &KinectInterface::Calibration_End,
        NULL, h3);

    // Set the profile
    nRetVal = g_UserGenerator.GetSkeletonCap().SetSkeletonProfile(XN_SKEL_PROFILE_ALL);

    // Register the depth and rgb images
    g_DepthGenerator.GetAlternativeViewPointCap().SetViewPoint(g_ImageGenerator);
    
    //XnMapOutputMode rgb_mode;
    //rgb_mode.nFPS = 30;
    //rgb_mode.nXRes = 640;
    //rgb_mode.nYRes = 480;
    //g_ImageGenerator.SetMapOutputMode(rgb_mode);
    
    // Start generating
    nRetVal = context.StartGeneratingAll();
    // TODO: check error code

    return true;
}

void KinectInterface::updateKinectData()
{ 
    // Update to next frame
    XnStatus nRetVal = context.WaitAndUpdateAll();
    // TODO: check error code
    
    xn::ImageMetaData imageMD;
    xn::DepthMetaData depthMD;
    
    // Retrieve the RGB image 
    g_ImageGenerator.GetMetaData(imageMD);

    // Retrieve the depth map
    g_DepthGenerator.GetMetaData(depthMD);

    XnUserID aUsers[8];
    XnUInt16 nUsers = 8;
    g_UserGenerator.GetUsers(aUsers, nUsers);

    joints.resize(30);
    joints_projected.resize(30);
    
    if (nUsers > 0 && g_UserGenerator.GetSkeletonCap().IsTracking(aUsers[0]))
    {
        int joint_id_OpenNI = (int)XN_SKEL_HEAD;
        int joint_id = 0;

        for (; joint_id_OpenNI <= XN_SKEL_RIGHT_FOOT; joint_id_OpenNI++)
        {
            if (!g_UserGenerator.GetSkeletonCap().IsJointActive((XnSkeletonJoint)joint_id_OpenNI))
                continue;

            XnSkeletonJointPosition Joint;
            g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(aUsers[0], (XnSkeletonJoint)(joint_id_OpenNI), Joint);
            
            joints[joint_id] = cv::Vec3d(Joint.position.X, Joint.position.Y, Joint.position.Z);

            g_DepthGenerator.ConvertRealWorldToProjective(1, &Joint.position, &Joint.position);
            joints_projected[joint_id] = cv::Vec2d(Joint.position.X, Joint.position.Y);

            joint_id++;
        }
    }
    else
    {
        joints.resize(0);
        joints_projected.resize(0);
    }
}

IplImage *KinectInterface::ConvertKinectImageToOpenCV(xn::ImageGenerator *imageGenerator,
                                                      xn::UserGenerator *userGenerator)
{
    xn::ImageMetaData imageMD;
    imageGenerator->GetMetaData(imageMD);

    const XnUInt8 *pImage = imageMD.Data();

    IplImage *image = cvCreateImage(cvSize(imageMD.XRes(), imageMD.YRes()),
                                    IPL_DEPTH_8U, 3);
    memcpy(image->imageData, imageMD.Data(), imageMD.DataSize());

    if (userGenerator)
    {
        xn::SceneMetaData sceneMD;
        userGenerator->GetUserPixels(0, sceneMD);
        const XnLabel *pImage = sceneMD.Data();
        const XnLabel* pImageRow = sceneMD.Data();
        for (XnUInt y = 0; y < sceneMD.YRes(); ++y)
        {
            const XnLabel* pImage = pImageRow;
            for (XnUInt x = 0; x < sceneMD.XRes(); ++x, pImage++)
            {
                if (!*pImage)
                    cvSet2D(image, y, x, cvScalar(0,0,0,0));
            }
            pImageRow += sceneMD.XRes();
        }
    }
    
    return image;
}
