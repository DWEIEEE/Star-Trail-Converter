#include <QString>
#include <QtWidgets/QWidget>
#include <opencv2/opencv.hpp>
#include <vector>
#include "func.h"

using namespace cv;
using namespace std;

void Func::canny(Mat img1, Mat& out)
{
	Mat gray, edge;
	cvtColor(img1, gray, COLOR_RGB2GRAY);
	blur(gray, edge, Size(3, 3));
	Canny(edge, out, 100, 50);
}
void Func::enhance(cv::Mat img1, cv::Mat& img_out)
{
	cv::Mat enhancedImage;
	cv::pow(img1 / 255, 2.0, enhancedImage);
	enhancedImage *= 255.0;
	img_out = enhancedImage.clone();
}
void Func::Rotation(cv::Mat img1, cv::Mat& img_out, double x_scale, double y_scale)
{
	Mat imgadd = img1.clone();
	for (float angle = 0.0; angle < 120.0; angle += 0.5)
	{
		cv::Point2f center(img1.cols * x_scale, img1.rows * y_scale);
		cv::Mat rotationMatrix = cv::getRotationMatrix2D(center, angle, 1.0);
		cv::Mat rotatedImage;
		cv::warpAffine(img1, rotatedImage, rotationMatrix, img1.size());
		imgadd = (imgadd + rotatedImage);
	}
	img_out = imgadd.clone();
}
void Func::Rotation2(cv::Mat img1, cv::Mat& img_out)
{
	Mat imgadd = img1.clone();
	for (float angle = 0.0; angle < 120.0; angle += 0.5)
	{
		cv::Point2f center(img1.cols / 2.0, img1.rows / 2);
		cv::Mat rotationMatrix = cv::getRotationMatrix2D(center, angle, 1.0 + angle / 70);
		cv::Mat rotatedImage;
		cv::warpAffine(img1, rotatedImage, rotationMatrix, img1.size());
		imgadd = (imgadd + rotatedImage);
	}
	img_out = imgadd.clone();
}
void Func::addforeground(cv::Mat img1, cv::Mat img_back, cv::Mat img_src, cv::Mat& img_out)
{
	Mat img = img1.clone();
	for (int i = 0; i < img_src.rows; i++)
	{
		for (int j = 0; j < img_src.cols; j++)
		{
			if (img_back.at<Vec3b>(i, j)[0] == 0 && img_back.at<Vec3b>(i, j)[1] == 0 && img_back.at<Vec3b>(i, j)[2] == 0)
			{
				for (int k = 0; k < 3; k++)
				{
					img.at<Vec3b>(i, j)[k] = img_src.at<Vec3b>(i, j)[k];
				}
			}
		}
	}
	img_out = img.clone();
}
vector<StarInfo> Func::calculateStar(Mat image, Point& max_star) 
{
	//start
	Mat grayImage;
	cvtColor(image, grayImage, COLOR_BGR2GRAY);
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(grayImage, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE);
	cout << contours.size() << "顆" << endl;

	double max = -1;
	vector<StarInfo> stars;
	for (size_t i = 0; i < contours.size(); ++i) {

		StarInfo star;
		size_t size;

		Mat mask = Mat::zeros(image.size(), CV_8UC1);
		drawContours(mask, contours, static_cast<int>(i), Scalar(255), FILLED);

		Scalar meanColor = mean(image, mask);

		double brightness;
		brightness = (meanColor[0] + meanColor[1] + meanColor[2]) / 3;

		size = 0;
		Point center(0, 0);

		//計算星星的中心座標和大小
		for (int y = 0; y < mask.rows; ++y) {
			for (int x = 0; x < mask.cols; ++x) {
				if (mask.at<uchar>(y, x) != 0) {
					center.x += x;
					center.y += y;
					size++;
				}
			}
		}

		if (size > 0) {
			center.x /= size;
			center.y /= size;
		}

		star.brightness = brightness;
		star.size = size;
		star.center = center;

		if (star.size * star.brightness > max) {
			max = star.size * star.brightness;
			max_star = star.center;
		}

		stars.push_back(star);
	}
	return stars;
}
void Func::powerLawTransform(Mat image, double gamma, Mat& output) 
{
	Mat imgNormalized, imgPowerLaw;
	normalize(image, imgNormalized, 0, 1, NORM_MINMAX, CV_32F);
	pow(imgNormalized, gamma, imgPowerLaw);
	normalize(imgPowerLaw, imgPowerLaw, 0, 255, NORM_MINMAX, CV_8U);
	output = imgPowerLaw.clone();
}
void Func::filterAurora_1(Mat image, Mat& output) 
{
	Mat blurredImage;
	GaussianBlur(image, blurredImage, Size(71, 71), 0);
	Mat highPassImage = image - blurredImage;

	Mat hsvImage;
	cvtColor(highPassImage, hsvImage, COLOR_BGR2HSV);
	// (sat)
	int saturationThreshold = 60;
	for (int i = 0; i < hsvImage.rows; ++i) {
		for (int j = 0; j < hsvImage.cols; ++j) {
			if (hsvImage.at<Vec3b>(i, j)[1] > saturationThreshold) {
				hsvImage.at<Vec3b>(i, j)[2] = 0;
			}
		}
	}
	Mat filteredImage;
	cvtColor(hsvImage, filteredImage, COLOR_HSV2BGR);

	Mat grayImage;
	cvtColor(filteredImage, grayImage, COLOR_BGR2GRAY);

	// Power-law ，增強暗部細節
	double gamma = 0.25;
	Mat enhancedImage;
	powerLawTransform(grayImage, gamma, enhancedImage);

	Mat binaryImage;
	threshold(enhancedImage, binaryImage, 150, 255, THRESH_BINARY);

	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(binaryImage, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE);
	Mat mask = Mat::zeros(image.size(), CV_8UC1);
	for (size_t i = 0; i < contours.size(); ++i) {
		drawContours(mask, contours, static_cast<int>(i), Scalar(255), FILLED);
	}

	// 保留星星，其他是0
	Mat result;
	image.copyTo(result, binaryImage);
	output = result.clone();
}

void Func::background(cv::Mat img1, cv::Mat& img_out)
{
	Mat result, bg, fg;
	Rect rect(0, 0, img1.cols, img1.rows* 0.8);
	grabCut(img1, result, rect, bg, fg, 1, GC_INIT_WITH_RECT);
	compare(result, GC_PR_FGD, result, CMP_EQ);
	Mat fore(img1.size(), CV_8UC3, Scalar(0, 0, 0));
	img1.copyTo(fore, result);
	img_out = fore.clone();
}
void Func::foreground(cv::Mat img1, cv::Mat img_back, cv::Mat& img_out)
{
	Mat result;
	subtract(img1, img_back, result);
	img_out = result.clone();
}
void Func::Resize(cv::Mat img, cv::Mat& out_img, double threshold)
{
	if (img.rows > threshold)
	{
		int ocol = img.cols / (double)(img.rows / (double)threshold);
		cv::resize(img, out_img, cv::Size(ocol, threshold));// img.cols, img.rows
	}
	else {
		out_img = img.clone();
	}
}
void Func::getMask(Mat img, Mat& img_out)
{
	Mat result, bg, fg;
	Rect rect(0, 0, img.cols, img.rows - 100);
	grabCut(img, result, rect, bg, fg, 2, GC_INIT_WITH_RECT);
	compare(result, GC_PR_FGD, result, CMP_EQ);
	Mat fore(img.size(), CV_8UC3, Scalar(0, 0, 0));
	img_out = result.clone();
}
void Func::showImg(cv::Mat img)
{
	imshow("display", img);
	waitKey(0);
	destroyAllWindows();
}
Func::Func(QWidget* parent) : QWidget(parent)
{

}