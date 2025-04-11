#pragma once
#include <QtWidgets/QWidget>
#include <QMouseEvent>
#include "ui_UI.h"
#include "func.h"
#include <time.h>

class MyUI : public QWidget
{
    Q_OBJECT

public:
    explicit MyUI(QWidget* parent = nullptr);
    bool eventFilter(QObject* obj, QEvent* event);
    void work_Circular();
    void work_Radiation();
    void work_Spiral();
    void work_Star();
    void work_Crop();
    void starSky_work_Crop();
    void get_TimeStr(std::string& timestr);
    void saveMBox(bool mode, bool choose);
    void waitingMBox(bool mode);

signals:
    void getSegment(const QString& string, QLabel* label, QLabel* label_2 = nullptr, QLabel* label_3 = nullptr);
    void getImg(const QString& string, QLabel* label, bool resize = false);
    void getInputPath(const QString& string);
    void getPos(int x, int y, int tx, int ty, QPen pen);
    void getFiles();
    void sendUpdate();

private slots:
    void input_browse();
    void output_browse();
    void pushLeft();
    void pushRight();
    void changeFolder();
    void changeFile();
    void generateMode();
    void restoreMode();
    void searchFolder(const QString& string);
    void DoSeg(const QString& string, QLabel* label, QLabel* label_2 = nullptr, QLabel* label_3 = nullptr);
    void postImg(const QString& string, QLabel* label, bool resize = false);
    void loadImg();
    void drawPos(int x, int y, int tx, int ty, QPen pen);
    void backPos();
    void newPos();
    void refresh();
    void Update();
    void save_circularPNG();
    void save_radiationPNG();
    void save_spiralPNG();
    void save_circularGIF();
    void save_radiationGIF();
    void save_spiralGIF();

private:
    Ui::MyUIClass ui;
    Func Func;
    QStringList files;
    int now = 0;
    int itemCount = 0;
    QString input_folderPath;
    QString output_folderPath;
    bool selected = true;
    bool mode = true;
    bool touch_lock = true;
    QPixmap touchscreen;
    int origin_height;
    int origin_width;
    int real_height;
    int real_width;
    int seg_height;
    int seg_width;
    int touch_x;
    int touch_y;
    int star_x;
    int star_y;
    double start_time;
    double end_time_1;
    double end_time_2;
    double end_time_3;
    QPen penO;
    QPen penR;
    cv::Mat star_img;// 3 channel 
    cv::Mat resize_img;
    cv::Mat seg_img;
    cv::Mat mask_img;// 1 channel
    cv::Mat tmp_mask;
    cv::Mat img;
    cv::Mat Circular_outcome;
    cv::Mat Radiation_outcome;
    cv::Mat Spiral_outcome;
};
