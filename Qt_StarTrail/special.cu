#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "special.cuh"
#include <cmath>

using namespace cv;
using namespace std;

__global__ void addBackground(uchar* star_trail, uchar* background, uchar* output, int rows, int cols)
{
    int x = blockIdx.x * blockDim.x + threadIdx.x;
    int y = blockIdx.y * blockDim.y + threadIdx.y;

    if (x < cols && y < rows)
    {
        for (int c = 0; c < 3; ++c)
        {
            if (star_trail[(y * cols + x) * 3 + c] == 0)
            {
                output[(y * cols + x) * 3 + c] = background[(y * cols + x) * 3 + c];
            }
            else
            {
                output[(y * cols + x) * 3 + c] = star_trail[(y * cols + x) * 3 + c];
            }
        }
    }
}
__global__ void addForeground(uchar* star_trail, uchar* foreground, uchar* foreground_mack, uchar* output, int rows, int cols)
{
    int x = blockIdx.x * blockDim.x + threadIdx.x;
    int y = blockIdx.y * blockDim.y + threadIdx.y;

    if (x < cols && y < rows)
    {
        for (int c = 0; c < 3; ++c)
        {
            if (foreground_mack[(y * cols + x) * 3 + c] != 0)
            {
                output[(y * cols + x) * 3 + c] = star_trail[(y * cols + x) * 3 + c];
            }
            else
            {
                output[(y * cols + x) * 3 + c] = foreground[(y * cols + x) * 3 + c];
            }
        }
    }
}
__global__ void scaleImage(uchar* input, uchar* record, int rows, int cols, float scale, int x_center, int y_center)
{
    int x = blockIdx.x * blockDim.x + threadIdx.x;
    int y = blockIdx.y * blockDim.y + threadIdx.y;

    // Calculate center coordinates
    int centerX = x_center;
    int centerY = y_center;

    // Calculate relative coordinates to the center
    int relX = x - centerX;
    int relY = y - centerY;

    // Apply scaling around the center
    float scale_d = 1.0 / (0.5 * scale);
    int new_x = static_cast<int>(relX * scale_d) + centerX;
    int new_y = static_cast<int>(relY * scale_d) + centerY;

    if (x < cols && y < rows && new_x >= 0 && new_x < cols && new_y >= 0 && new_y < rows)
    {
        for (int c = 0; c < 3; ++c)
        {
            // Copy pixel value to the output image
            if ((input[(new_y * cols + new_x) * 3 + c] + record[(y * cols + x) * 3 + c]) > 255)
            {
                record[(y * cols + x) * 3 + c] = 255;
            }
            else
            {
                record[(y * cols + x) * 3 + c] = input[(new_y * cols + new_x) * 3 + c] + record[(y * cols + x) * 3 + c];
            }
        }
    }
}
__global__ void rotateandscale(uchar* input, uchar* rotat, int rows, int cols, float angle, int x_center, int y_center)
{
    int x = blockIdx.x * blockDim.x + threadIdx.x;
    int y = blockIdx.y * blockDim.y + threadIdx.y;

    if (x < cols && y < rows)
    {
        // Calculate center coordinates
        int centerX = x_center;
        int centerY = y_center;

        // Calculate relative coordinates to the center
        int relX = x - centerX;
        int relY = y - centerY;

        // Apply scaling around the center
        float scale_d = (angle * 0.5);
        int scaledX = static_cast<int>(relX * scale_d);
        int scaledY = static_cast<int>(relY * scale_d);

        // Apply rotation around the center
        float radians = angle * 3.141592653589793;
        int rotatedX = static_cast<int>(cos(radians) * scaledX - sin(radians) * scaledY) + centerX;
        int rotatedY = static_cast<int>(sin(radians) * scaledX + cos(radians) * scaledY) + centerY;

        if (rotatedX >= 0 && rotatedX < cols && rotatedY >= 0 && rotatedY < rows)
        {
            for (int c = 0; c < 3; ++c)
            {
                if ((input[(rotatedY * cols + rotatedX) * 3 + c] + rotat[(y * cols + x) * 3 + c]) > 255)
                {
                    rotat[(y * cols + x) * 3 + c] = 255;
                }
                else
                {
                    rotat[(y * cols + x) * 3 + c] = input[(rotatedY * cols + rotatedX) * 3 + c] + rotat[(y * cols + x) * 3 + c];
                }
            }
        }
        else
        {
            for (int c = 0; c < 3; ++c)
            {
                rotat[(y * cols + x) * 3 + c] = rotat[(y * cols + x) * 3 + c];
            }
        }
    }
}

__global__ void rotateImage(uchar* input, uchar* rotat, int rows, int cols, float angle, int x_center, int y_center)
{
    int x = blockIdx.x * blockDim.x + threadIdx.x;
    int y = blockIdx.y * blockDim.y + threadIdx.y;

    if (x < cols && y < rows)
    {
        float radians = angle * 3.141592653589793 / 180.0;
        int new_x = static_cast<int>(cos(radians) * (x - x_center) - sin(radians) * (y - y_center) + x_center);
        int new_y = static_cast<int>(sin(radians) * (x - x_center) + cos(radians) * (y - y_center) + y_center);

        if (new_x >= 0 && new_x < cols && new_y >= 0 && new_y < rows)
        {
            for (int c = 0; c < 3; ++c)
            {
                if ((input[(new_y * cols + new_x) * 3 + c] + rotat[(y * cols + x) * 3 + c]) > 255)
                {
                    rotat[(y * cols + x) * 3 + c] = 255;
                }
                else
                {
                    rotat[(y * cols + x) * 3 + c] = input[(new_y * cols + new_x) * 3 + c] + rotat[(y * cols + x) * 3 + c];
                }
            }
        }
        else
        {
            for (int c = 0; c < 3; ++c)
            {
                rotat[(y * cols + x) * 3 + c] = rotat[(y * cols + x) * 3 + c];
            }
        }
    }
}
void Circle_rotate(Mat img1, Mat seg_img, Mat mask_img, Mat img, Mat& img_out, int x_center, int y_center)
{
    Mat foreground = img - seg_img;
	uchar* d_img1, * d_rotat, * d_img, * d_seg_img, * d_tmp, * d_mask_img, * d_foreground;
	int size = img1.cols * img1.rows * 3;
	cudaMalloc((void**)&d_img1, size * sizeof(uchar));
	cudaMalloc((void**)&d_rotat, size * sizeof(uchar));
    cudaMalloc((void**)&d_img, size * sizeof(uchar));
    cudaMalloc((void**)&d_seg_img, size * sizeof(uchar));
    cudaMalloc((void**)&d_tmp, size * sizeof(uchar));
    cudaMalloc((void**)&d_mask_img, size * sizeof(uchar));
    cudaMalloc((void**)&d_foreground, size * sizeof(uchar));

	cudaMemcpy(d_img1, img1.data, size * sizeof(uchar), cudaMemcpyHostToDevice);
    cudaMemcpy(d_seg_img, seg_img.data, size * sizeof(uchar), cudaMemcpyHostToDevice);
    cudaMemcpy(d_mask_img, mask_img.data, size * sizeof(uchar), cudaMemcpyHostToDevice);
    cudaMemcpy(d_foreground, foreground.data, size * sizeof(uchar), cudaMemcpyHostToDevice);

	dim3 blockSize(32, 32);
	dim3 gridSize((img1.cols + blockSize.x - 1) / blockSize.x, (img1.rows + blockSize.y - 1) / blockSize.y);
	for (float angle = 0.0; angle < 240.0; angle += 0.5)
	{
        rotateImage<<<gridSize, blockSize>>>(d_img1, d_rotat, img1.rows, img1.cols, angle, x_center, y_center);
	}
    addBackground<<<gridSize, blockSize>>>(d_rotat, d_seg_img, d_tmp, img1.rows, img1.cols);
    addForeground<<<gridSize, blockSize>>>(d_tmp, d_foreground, d_mask_img, d_rotat, img1.rows, img1.cols);
	uchar* rotated_image = new uchar[size];
	cudaMemcpy(rotated_image, d_rotat, size * sizeof(uchar), cudaMemcpyDeviceToHost);
	Mat rotated_mat(img1.rows, img1.cols, CV_8UC3, rotated_image);
    //imshow("", rotated_mat);
    //waitKey(0);
	img_out = rotated_mat.clone();

    cudaFree(d_img);
    cudaFree(d_img1);
    cudaFree(d_rotat);
    cudaFree(d_seg_img);
    cudaFree(d_tmp);
    cudaFree(d_mask_img);
    cudaFree(d_foreground);
}
void Radiation_rotate(Mat img1, Mat seg_img, Mat mask_img, Mat img, Mat& img_out, int x_center, int y_center)
{
    Mat foreground = img - seg_img;
    uchar* d_img1, * d_rotat, * d_img, * d_seg_img, * d_tmp, * d_mask_img, * d_foreground;
    int size = img1.cols * img1.rows * 3;
    cudaMalloc((void**)&d_img1, size * sizeof(uchar));
    cudaMalloc((void**)&d_rotat, size * sizeof(uchar));
    cudaMalloc((void**)&d_img, size * sizeof(uchar));
    cudaMalloc((void**)&d_seg_img, size * sizeof(uchar));
    cudaMalloc((void**)&d_tmp, size * sizeof(uchar));
    cudaMalloc((void**)&d_mask_img, size * sizeof(uchar));
    cudaMalloc((void**)&d_foreground, size * sizeof(uchar));

    cudaMemcpy(d_img1, img1.data, size * sizeof(uchar), cudaMemcpyHostToDevice);
    cudaMemcpy(d_seg_img, seg_img.data, size * sizeof(uchar), cudaMemcpyHostToDevice);
    cudaMemcpy(d_mask_img, mask_img.data, size * sizeof(uchar), cudaMemcpyHostToDevice);
    cudaMemcpy(d_foreground, foreground.data, size * sizeof(uchar), cudaMemcpyHostToDevice);

    dim3 blockSize(32, 32);
    dim3 gridSize((img1.cols + blockSize.x - 1) / blockSize.x, (img1.rows + blockSize.y - 1) / blockSize.y);
    for (float angle = 2; angle <= 4; angle += 0.01)
    {
        scaleImage << <gridSize, blockSize >> > (d_img1, d_rotat, img1.rows, img1.cols, angle, x_center, y_center);
    }
    addBackground << <gridSize, blockSize >> > (d_rotat, d_seg_img, d_tmp, img1.rows, img1.cols);
    addForeground << <gridSize, blockSize >> > (d_tmp, d_foreground, d_mask_img, d_rotat, img1.rows, img1.cols);
    uchar* rotated_image = new uchar[size];
    cudaMemcpy(rotated_image, d_rotat, size * sizeof(uchar), cudaMemcpyDeviceToHost);
    Mat rotated_mat(img1.rows, img1.cols, CV_8UC3, rotated_image);
    //imshow("", rotated_mat);
    //waitKey(0);
    img_out = rotated_mat.clone();

    cudaFree(d_img);
    cudaFree(d_img1);
    cudaFree(d_rotat);
    cudaFree(d_seg_img);
    cudaFree(d_tmp);
    cudaFree(d_mask_img);
    cudaFree(d_foreground);
}
void Spiral_rotate(Mat img1, Mat seg_img, Mat mask_img, Mat img, Mat& img_out, int x_center, int y_center)
{
    Mat foreground = img - seg_img;
    uchar* d_img1, * d_rotat, * d_img, * d_seg_img, * d_tmp, * d_mask_img, * d_foreground;
    int size = img1.cols * img1.rows * 3;
    cudaMalloc((void**)&d_img1, size * sizeof(uchar));
    cudaMalloc((void**)&d_rotat, size * sizeof(uchar));
    cudaMalloc((void**)&d_img, size * sizeof(uchar));
    cudaMalloc((void**)&d_seg_img, size * sizeof(uchar));
    cudaMalloc((void**)&d_tmp, size * sizeof(uchar));
    cudaMalloc((void**)&d_mask_img, size * sizeof(uchar));
    cudaMalloc((void**)&d_foreground, size * sizeof(uchar));

    cudaMemcpy(d_img1, img1.data, size * sizeof(uchar), cudaMemcpyHostToDevice);
    cudaMemcpy(d_seg_img, seg_img.data, size * sizeof(uchar), cudaMemcpyHostToDevice);
    cudaMemcpy(d_mask_img, mask_img.data, size * sizeof(uchar), cudaMemcpyHostToDevice);
    cudaMemcpy(d_foreground, foreground.data, size * sizeof(uchar), cudaMemcpyHostToDevice);

    dim3 blockSize(32, 32);
    dim3 gridSize((img1.cols + blockSize.x - 1) / blockSize.x, (img1.rows + blockSize.y - 1) / blockSize.y);
    for (float angle = 2; angle <= 4; angle += 0.002)
    {
        rotateandscale << <gridSize, blockSize >> > (d_img1, d_rotat, img1.rows, img1.cols, angle, x_center, y_center);
    }
    addBackground << <gridSize, blockSize >> > (d_rotat, d_seg_img, d_tmp, img1.rows, img1.cols);
    addForeground << <gridSize, blockSize >> > (d_tmp, d_foreground, d_mask_img, d_rotat, img1.rows, img1.cols);
    uchar* rotated_image = new uchar[size];
    cudaMemcpy(rotated_image, d_rotat, size * sizeof(uchar), cudaMemcpyDeviceToHost);
    Mat rotated_mat(img1.rows, img1.cols, CV_8UC3, rotated_image);
    //imshow("", rotated_mat);
    //waitKey(0);
    img_out = rotated_mat.clone();

    cudaFree(d_img);
    cudaFree(d_img1);
    cudaFree(d_rotat);
    cudaFree(d_seg_img);
    cudaFree(d_tmp);
    cudaFree(d_mask_img);
    cudaFree(d_foreground);
}
void Circular_GIF(Mat img1, Mat seg_img, Mat mask_img, Mat img, int x_center, int y_center, string outpath, string basename)
{
    cv::VideoWriter writer;
    string path = "tmp.avi";
    writer.open(path.c_str(), VideoWriter::fourcc('M', 'P', '4', '2'), 30, Size(img1.cols, img1.rows));

    Mat foreground = img - seg_img;
    uchar* d_img1, * d_rotat, * d_img, * d_seg_img, * d_tmp, * d_mask_img, * d_foreground;
    int size = img1.cols * img1.rows * 3;
    cudaMalloc((void**)&d_img1, size * sizeof(uchar));
    cudaMalloc((void**)&d_rotat, size * sizeof(uchar));
    cudaMalloc((void**)&d_img, size * sizeof(uchar));
    cudaMalloc((void**)&d_seg_img, size * sizeof(uchar));
    cudaMalloc((void**)&d_tmp, size * sizeof(uchar));
    cudaMalloc((void**)&d_mask_img, size * sizeof(uchar));
    cudaMalloc((void**)&d_foreground, size * sizeof(uchar));

    cudaMemcpy(d_img1, img1.data, size * sizeof(uchar), cudaMemcpyHostToDevice);
    cudaMemcpy(d_seg_img, seg_img.data, size * sizeof(uchar), cudaMemcpyHostToDevice);
    cudaMemcpy(d_mask_img, mask_img.data, size * sizeof(uchar), cudaMemcpyHostToDevice);
    cudaMemcpy(d_foreground, foreground.data, size * sizeof(uchar), cudaMemcpyHostToDevice);

    dim3 blockSize(32, 32);
    dim3 gridSize((img1.cols + blockSize.x - 1) / blockSize.x, (img1.rows + blockSize.y - 1) / blockSize.y);
    for (float angle = 0.0; angle < 240.0; angle += 0.5)
    {
        rotateImage << <gridSize, blockSize >> > (d_img1, d_rotat, img1.rows, img1.cols, angle, x_center, y_center);
        addBackground << <gridSize, blockSize >> > (d_rotat, d_seg_img, d_tmp, img1.rows, img1.cols);
        addForeground << <gridSize, blockSize >> > (d_tmp, d_foreground, d_mask_img, d_rotat, img1.rows, img1.cols);

        uchar* rotated_image = new uchar[size];
        cudaMemcpy(rotated_image, d_rotat, size * sizeof(uchar), cudaMemcpyDeviceToHost);
        Mat rotated_mat(img1.rows, img1.cols, CV_8UC3, rotated_image);
        writer.write(rotated_mat);

        delete[] rotated_image;
    }
    writer.release();
    string ffmpegCommand = "E:\\ffmpeg\\bin\\ffmpeg.exe -i " + std::string("tmp.avi") + " " + outpath + "/Circular_" + basename + ".gif";
    system(ffmpegCommand.c_str());
    remove("tmp.avi");

    cudaFree(d_img);
    cudaFree(d_img1);
    cudaFree(d_rotat);
    cudaFree(d_seg_img);
    cudaFree(d_tmp);
    cudaFree(d_mask_img);
    cudaFree(d_foreground);
    /*
    delete[] d_img1;
    delete[] d_rotat;
    delete[] d_img;
    delete[] d_seg_img;
    delete[] d_tmp;
    delete[] d_mask_img;
    delete[] d_foreground;
    */
}
void Radiation_GIF(Mat img1, Mat seg_img, Mat mask_img, Mat img, int x_center, int y_center, string outpath, string basename)
{
    cv::VideoWriter writer;
    string path = "tmp.avi";
    writer.open(path.c_str(), VideoWriter::fourcc('M', 'P', '4', '2'), 30, Size(img1.cols, img1.rows));

    Mat foreground = img - seg_img;
    uchar* d_img1, * d_rotat, * d_img, * d_seg_img, * d_tmp, * d_mask_img, * d_foreground;
    int size = img1.cols * img1.rows * 3;
    cudaMalloc((void**)&d_img1, size * sizeof(uchar));
    cudaMalloc((void**)&d_rotat, size * sizeof(uchar));
    cudaMalloc((void**)&d_img, size * sizeof(uchar));
    cudaMalloc((void**)&d_seg_img, size * sizeof(uchar));
    cudaMalloc((void**)&d_tmp, size * sizeof(uchar));
    cudaMalloc((void**)&d_mask_img, size * sizeof(uchar));
    cudaMalloc((void**)&d_foreground, size * sizeof(uchar));

    cudaMemcpy(d_img1, img1.data, size * sizeof(uchar), cudaMemcpyHostToDevice);
    cudaMemcpy(d_seg_img, seg_img.data, size * sizeof(uchar), cudaMemcpyHostToDevice);
    cudaMemcpy(d_mask_img, mask_img.data, size * sizeof(uchar), cudaMemcpyHostToDevice);
    cudaMemcpy(d_foreground, foreground.data, size * sizeof(uchar), cudaMemcpyHostToDevice);

    dim3 blockSize(32, 32);
    dim3 gridSize((img1.cols + blockSize.x - 1) / blockSize.x, (img1.rows + blockSize.y - 1) / blockSize.y);
    for (float angle = 2; angle <= 4; angle += 0.01)
    {
        scaleImage << <gridSize, blockSize >> > (d_img1, d_rotat, img1.rows, img1.cols, angle, x_center, y_center);
        addBackground << <gridSize, blockSize >> > (d_rotat, d_seg_img, d_tmp, img1.rows, img1.cols);
        addForeground << <gridSize, blockSize >> > (d_tmp, d_foreground, d_mask_img, d_rotat, img1.rows, img1.cols);

        uchar* rotated_image = new uchar[size];
        cudaMemcpy(rotated_image, d_rotat, size * sizeof(uchar), cudaMemcpyDeviceToHost);
        Mat rotated_mat(img1.rows, img1.cols, CV_8UC3, rotated_image);
        writer.write(rotated_mat);
        delete[] rotated_image;
    }
    writer.release();
    string ffmpegCommand = "E:\\ffmpeg\\bin\\ffmpeg.exe -i " + std::string("tmp.avi") + " " + outpath + "/Radiation_" + basename + ".gif";
    system(ffmpegCommand.c_str());
    remove("tmp.avi");

    cudaFree(d_img);
    cudaFree(d_img1);
    cudaFree(d_rotat);
    cudaFree(d_seg_img);
    cudaFree(d_tmp);
    cudaFree(d_mask_img);
    cudaFree(d_foreground);
}
void Spiral_GIF(Mat img1, Mat seg_img, Mat mask_img, Mat img, int x_center, int y_center, string outpath, string basename)
{
    cv::VideoWriter writer;
    string path = "tmp.avi";
    writer.open(path.c_str(), VideoWriter::fourcc('M', 'P', '4', '2'), 30, Size(img1.cols, img1.rows));

    Mat foreground = img - seg_img;
    uchar* d_img1, * d_rotat, * d_img, * d_seg_img, * d_tmp, * d_mask_img, * d_foreground;
    int size = img1.cols * img1.rows * 3;
    cudaMalloc((void**)&d_img1, size * sizeof(uchar));
    cudaMalloc((void**)&d_rotat, size * sizeof(uchar));
    cudaMalloc((void**)&d_img, size * sizeof(uchar));
    cudaMalloc((void**)&d_seg_img, size * sizeof(uchar));
    cudaMalloc((void**)&d_tmp, size * sizeof(uchar));
    cudaMalloc((void**)&d_mask_img, size * sizeof(uchar));
    cudaMalloc((void**)&d_foreground, size * sizeof(uchar));

    cudaMemcpy(d_img1, img1.data, size * sizeof(uchar), cudaMemcpyHostToDevice);
    cudaMemcpy(d_seg_img, seg_img.data, size * sizeof(uchar), cudaMemcpyHostToDevice);
    cudaMemcpy(d_mask_img, mask_img.data, size * sizeof(uchar), cudaMemcpyHostToDevice);
    cudaMemcpy(d_foreground, foreground.data, size * sizeof(uchar), cudaMemcpyHostToDevice);

    dim3 blockSize(32, 32);
    dim3 gridSize((img1.cols + blockSize.x - 1) / blockSize.x, (img1.rows + blockSize.y - 1) / blockSize.y);
    for (float angle = 2; angle <= 4; angle += 0.002)
    {
        //scaleImage << <gridSize, blockSize >> > (d_img1, d_rotat, img1.rows, img1.cols, angle, x_center, y_center);
        //rotateImage << <gridSize, blockSize >> > (d_rotat, d_rotat, img1.rows, img1.cols, angle, x_center, y_center);
        rotateandscale << <gridSize, blockSize >> > (d_img1, d_rotat, img1.rows, img1.cols, angle, x_center, y_center);
        addBackground << <gridSize, blockSize >> > (d_rotat, d_seg_img, d_tmp, img1.rows, img1.cols);
        addForeground << <gridSize, blockSize >> > (d_tmp, d_foreground, d_mask_img, d_rotat, img1.rows, img1.cols);

        uchar* rotated_image = new uchar[size];
        cudaMemcpy(rotated_image, d_rotat, size * sizeof(uchar), cudaMemcpyDeviceToHost);
        Mat rotated_mat(img1.rows, img1.cols, CV_8UC3, rotated_image);
        writer.write(rotated_mat);
        delete[] rotated_image;
    }
    writer.release();
    string ffmpegCommand = "E:\\ffmpeg\\bin\\ffmpeg.exe -i " + std::string("tmp.avi") + " " + outpath + "/Spiral_" + basename + ".gif";
    system(ffmpegCommand.c_str());
    remove("tmp.avi");

    cudaFree(d_img);
    cudaFree(d_img1);
    cudaFree(d_rotat);
    cudaFree(d_seg_img);
    cudaFree(d_tmp);
    cudaFree(d_mask_img);
    cudaFree(d_foreground);
}