#pragma once

#include <opencv2/opencv.hpp>

class Camera
{
public:
    Camera(int index = 0); // Default camera is index 0
    ~Camera();
    bool GetFrame(cv::Mat& frame);
private:
    cv::VideoCapture cap_;
};
