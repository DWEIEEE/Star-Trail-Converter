#pragma once
#include <QString>
#include <QThread>
#include <QtWidgets/QWidget>
#include <opencv2/opencv.hpp>
#include "ui_UI.h"

class Func : public QWidget
{
    Q_OBJECT

public:
    Func(QWidget* parent = nullptr);
    void showImg(cv::Mat img);
    void Resize(cv::Mat img, cv::Mat& out_img, double threshold);
    void background(cv::Mat img1, cv::Mat& img_out);
    void foreground(cv::Mat img1, cv::Mat img_back, cv::Mat& img_out);

private:
    Ui::MyUIClass ui;
};
