#include <QString>
#include <QtWidgets/QWidget>
#include <opencv2/opencv.hpp>
#include "func.h"

using namespace cv;

Func::Func(QWidget* parent) : QWidget(parent)
{

}

void Func::background(cv::Mat img1, cv::Mat& img_out)
{
	Mat result, bg, fg;
	Rect rect(0, 0, img1.cols, img1.rows - 100);
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