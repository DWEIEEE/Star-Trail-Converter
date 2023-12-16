#pragma once
#include <QString>
#include <vector>
#include <QtWidgets/QWidget>
#include <opencv2/opencv.hpp>
#include "ui_UI.h"

struct StarInfo {
    cv::Point center;
    std::vector<cv::Point> points;
    std::vector<cv::Vec3b> colors;
    double size;
    double brightness;
};

class Func : public QWidget
{
    Q_OBJECT

public:
    Func(QWidget* parent = nullptr);
    void showImg(cv::Mat img);
    void Resize(cv::Mat img, cv::Mat& out_img, double threshold);
    void background(cv::Mat img1, cv::Mat& img_out);
    void foreground(cv::Mat img1, cv::Mat img_back, cv::Mat& img_out);
    std::vector<StarInfo> calculateStar(cv::Mat image, cv::Point& max_star);
    void filterAurora_1(cv::Mat image, cv::Mat& output);
    void powerLawTransform(cv::Mat image, double gamma, cv::Mat& output);
    void getMask(cv::Mat img1, cv::Mat& img_out);

private:
    Ui::MyUIClass ui;
};
