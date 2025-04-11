#ifndef CUDA_FUNCTIONS_CUH
#define CUDA_FUNCTIONS_CUH
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

__global__ void addForeground(uchar* star_trail, uchar* foreground, uchar* foreground_mask, uchar* output, int rows, int cols);
__global__ void addBackground(uchar* star_trail, uchar* background, uchar* output, int rows, int cols);
__global__ void rotateImage(uchar* input, uchar* rotat, int rows, int cols, float angle);
__global__ void rotateandscale(uchar* input, uchar* rotat, int rows, int cols, float angle);
__global__ void scaleImage(uchar* input, uchar* record, int rows, int cols, float scale);
void Circle_rotate(cv::Mat img1, cv::Mat seg_img, cv::Mat mask_img, cv::Mat img, cv::Mat& img_out, int x_center, int y_center);
void Radiation_rotate(cv::Mat img1, cv::Mat seg_img, cv::Mat mask_img, cv::Mat img, cv::Mat& img_out, int x_center, int y_center);
void Spiral_rotate(cv::Mat img1, cv::Mat seg_img, cv::Mat mask_img, cv::Mat img, cv::Mat& img_out, int x_center, int y_center);
void Circular_GIF(cv::Mat img1, cv::Mat seg_img, cv::Mat mask_img, cv::Mat img, int x_center, int y_center, std::string outpath, std::string basename);
void Radiation_GIF(cv::Mat img1, cv::Mat seg_img, cv::Mat mask_img, cv::Mat img, int x_center, int y_center, std::string outpath, std::string basename);
void Spiral_GIF(cv::Mat img1, cv::Mat seg_img, cv::Mat mask_img, cv::Mat img, int x_center, int y_center, std::string outpath, std::string basename);
#endif