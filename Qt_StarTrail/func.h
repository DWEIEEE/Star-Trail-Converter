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
    void reResize(cv::Mat img, cv::Mat& out_img, int x, int y);
    void background(cv::Mat img1, cv::Mat& img_out);
    void newbackground(cv::Mat img1, cv::Mat& img_out, cv::Mat& mask_img, cv::Mat& tmp_mask);
    void foreground(cv::Mat img1, cv::Mat img_back, cv::Mat& img_out);

    void calculateStar(cv::Mat image, cv::Point& max_star);
    void filterAurora_1(cv::Mat image, cv::Mat& output);
    void powerLawTransform(cv::Mat image, double gamma, cv::Mat& output);
    void getMask(cv::Mat img1, cv::Mat& img_out);

    void canny(cv::Mat img1, cv::Mat& out);
    void enhance(cv::Mat img1, cv::Mat& img_out);
    void Rotation(cv::Mat img1, cv::Mat& img_out, int x_center, int y_center);
    void Rotation2(cv::Mat img1, cv::Mat& img_out, int x_center, int y_center);
    void Rotationline(cv::Mat img1, cv::Mat& img_out, int x_center, int y_center);
    void addforeground(cv::Mat img1, cv::Mat img_back, cv::Mat img_src, cv::Mat& img_out);
    void Circular_GIF(cv::Mat img, cv::Mat seg_img, cv::Mat origin_img, int x_center, int y_center, std::string path, std::string basename);
    void Radiation_GIF(cv::Mat img, cv::Mat seg_img, cv::Mat origin_img, int x_center, int y_center, std::string path, std::string basename);
    void Spiral_GIF(cv::Mat img, cv::Mat seg_img, cv::Mat origin_img, int x_center, int y_center, std::string path, std::string basename);
    void Homono_filter(cv::Mat img_in, cv::Mat& img_out);
    void My_DFT(cv::Mat input_image, cv::Mat& output_image, cv::Mat& transform_image);
    void mask_generate(cv::Mat img_in, cv::Mat& img_out, cv::Mat& tmp_mask, int row, int col);
    void restore_startrail(cv::Mat& image, cv::Mat& out);
    void starSky_mask_generate(cv::Mat img_in, cv::Mat& img_out, cv::Mat& tmp_mask, int row, int col);
    void starSky_newbackground(cv::Mat img1, cv::Mat& img_out, cv::Mat& mask_img, cv::Mat& tmp_mask);
private:
    Ui::MyUIClass ui;
};
