#include "camera.h"

Camera::Camera(int index)
    : cap_(index)
{
}

Camera::~Camera()
{
    cap_.release(); // Releasing the buffer memory
}

bool Camera::GetFrame(cv::Mat& frame)
{
    bool ret = true;
    if (!cap_.isOpened()) ret = false;
    else
    {
        cap_ >> frame;
        if(frame.empty()) ret = false;
    }
    return ret;
}