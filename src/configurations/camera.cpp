#include "camera.h"

Camera* Camera::getFrontCamera()
{
    return &Front;
}

Camera* Camera::getBottomCamera()
{
    return &Bottom;
}

Camera::Camera(cv::Mat intrinsicParametersIn) : intrinsicParameters(intrinsicParametersIn)
{
    //intrinsicParametersIn.copyTo(intrinsicParameters);
}

const cv::Mat& Camera::getIntrinsicParameters() const
{
    return intrinsicParameters;
}

Camera Camera::front()
{
    float mat[9] = {
        561.999146f,        0.0f, 307.433982f,
               0.0f, 561.782697f, 190.144373f,
               0.0f,        0.0f,        1.0f,
    };

    return Camera(cv::Mat(3, 3, CV_32F, mat));
}

Camera Camera::bottom()
{
    float mat[9] = {
        686.994766f,        0.0f, 329.323208f,
               0.0f, 688.195055f, 159.323007f,
               0.0f,        0.0f,        1.0f,
    };

    return Camera(cv::Mat(3, 3, CV_32F, mat));
}

Camera Camera::Front  = Camera::front();
Camera Camera::Bottom = Camera::bottom();
