#include "myUI.h"
#include <QFileDialog>
#include <QStringList>
#include <QDebug>
#include <QDir>
#include <QStyle>
#include <QPixmap>
#include <QImage>
#include <QImageReader>
#include <QButtonGroup>
#include <QFileInfo>
#include <iostream>
#include <QLabel>
#include <QPainter>
#include <QPen>
#include <QMessageBox>
#include <thread>
#include <QTimer>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include "special.cuh"

using namespace std;
using namespace cv;

MyUI::MyUI(QWidget *parent) : QWidget(parent)
{
    ui.setupUi(this);
    setFixedSize(1290, 720);
    ui.pushButton_3->setEnabled(false);
    ui.pushButton_4->setEnabled(false);
    ui.pushButton_5->setEnabled(false);
    ui.pushButton_6->setEnabled(false);
    ui.pushButton_7->setEnabled(false);
    ui.pushButton_8->setEnabled(false);
    ui.pushButton_11->setEnabled(false);
    ui.pushButton_12->setEnabled(false);
    QImageReader::setAllocationLimit(0);
    QButtonGroup* buttonGroup_1 = new QButtonGroup(this);
    buttonGroup_1->addButton(ui.radioButton_3);
    buttonGroup_1->addButton(ui.radioButton_4);
    QButtonGroup* buttonGroup_2 = new QButtonGroup(this);
    buttonGroup_2->addButton(ui.radioButton);
    buttonGroup_2->addButton(ui.radioButton_2);
    ui.radioButton->setChecked(true);
    ui.radioButton_3->setChecked(true);
    ui.label_16->installEventFilter(this);
    penO.setColor(QColor(255, 120, 47));
    penO.setWidth(2);
    penR.setColor(QColor(255, 0, 0));
    penR.setWidth(2);
    connect(ui.pushButton, SIGNAL(clicked()), SLOT(input_browse()));
    connect(ui.pushButton_2, SIGNAL(clicked()), SLOT(output_browse()));
    connect(ui.pushButton_3, SIGNAL(clicked()), SLOT(save_circularPNG()));
    connect(ui.pushButton_4, SIGNAL(clicked()), SLOT(save_radiationPNG()));
    connect(ui.pushButton_5, SIGNAL(clicked()), SLOT(save_spiralPNG()));
    connect(ui.pushButton_6, SIGNAL(clicked()), SLOT(save_circularGIF()));
    connect(ui.pushButton_7, SIGNAL(clicked()), SLOT(save_radiationGIF()));
    connect(ui.pushButton_8, SIGNAL(clicked()), SLOT(save_spiralGIF()));
    connect(ui.pushButton_9, SIGNAL(clicked()), SLOT(backPos()));
    connect(ui.pushButton_10, SIGNAL(clicked()), SLOT(newPos()));
    connect(ui.pushButton_11, SIGNAL(clicked()), SLOT(pushLeft()));
    connect(ui.pushButton_12, SIGNAL(clicked()), SLOT(pushRight()));
    connect(ui.radioButton_4, SIGNAL(clicked()), SLOT(changeFolder()));
    connect(ui.radioButton_3, SIGNAL(clicked()), SLOT(changeFile()));
    connect(ui.radioButton_2, SIGNAL(clicked()), SLOT(restoreMode()));
    connect(ui.radioButton, SIGNAL(clicked()), SLOT(generateMode()));
    connect(this, &MyUI::getInputPath, this, &MyUI::searchFolder);
    connect(this, &MyUI::getFiles, this, &MyUI::loadImg);
    connect(this, &MyUI::getSegment, this, &MyUI::DoSeg);
    connect(this, &MyUI::getImg, this, &MyUI::postImg);
    connect(this, &MyUI::getPos, this, &MyUI::drawPos);
    connect(this, &MyUI::sendUpdate, this, &MyUI::Update);
}
void MyUI::get_TimeStr(string& timestr)
{
    auto currentTime = chrono::system_clock::now();
    time_t currentTimeT = chrono::system_clock::to_time_t(currentTime);
    struct tm* timeInfo = localtime(&currentTimeT);
    stringstream filenameStream;
    filenameStream << "_" << std::put_time(timeInfo, "%Y%m%d%H%M%S");
    timestr = filenameStream.str();
}
void MyUI::saveMBox(bool mode = true, bool choose = true)
{
    if (mode != true)
    {
        QMessageBox MBox(QMessageBox::Warning, "", "");
        MBox.setWindowTitle("Notice");
        MBox.setText("You need to choose the folder first.");
        MBox.setWindowFlags(Qt::Dialog);
        QTimer::singleShot(1500, &MBox, &QMessageBox::close);
        MBox.exec();
        
    }
    else
    {
        QMessageBox MBox(QMessageBox::Information, "", "");
        MBox.setWindowTitle("Hint");
        if (choose == true)
        {
            MBox.setText("Successfully saved the image.");
        }
        else
        {
            MBox.setText("Successfully saved the GIF.");
        }
        MBox.setWindowFlags(Qt::Dialog);
        QTimer::singleShot(1500, &MBox, &QMessageBox::close);
        MBox.exec();
    }
}
void MyUI::waitingMBox(bool mode = true)
{
    QMessageBox MBox(QMessageBox::Information, "", "");
    MBox.setWindowTitle("Hint");
    MBox.setText("Please wait ...");
    MBox.setWindowFlags(Qt::Dialog);
    QTimer::singleShot(1000, &MBox, &QMessageBox::close);
    MBox.exec();
}
void MyUI::save_circularGIF()
{
    if (ui.lineEdit_2->text() == "")
    {
        saveMBox(false);
        return;
    }
    string output_path = output_folderPath.toStdString();
    string filename = files[now].toStdString();
    size_t dotPosition = filename.find_last_of('.');
    string timestr;
    get_TimeStr(timestr);
    string basename = filename.substr(0, dotPosition) + timestr;

    waitingMBox();
    //Func.Circular_GIF(star_img, seg_img, img, star_x, star_y , output_path, basename);
    Circular_GIF(star_img, seg_img,mask_img, img, star_x, star_y, output_path, basename);
    saveMBox(true,false);
}
void MyUI::save_radiationGIF()
{
    if (ui.lineEdit_2->text() == "")
    {
        saveMBox(false);
        return;
    }
    string output_path = output_folderPath.toStdString();
    string filename = files[now].toStdString();
    size_t dotPosition = filename.find_last_of('.');
    string timestr;
    get_TimeStr(timestr);
    string basename = filename.substr(0, dotPosition) + timestr;

    waitingMBox();
    //Func.Radiation_GIF(star_img, seg_img, img, star_x, star_y, output_path, basename);
    Radiation_GIF(star_img, seg_img, mask_img, img, star_x, star_y, output_path, basename);
    saveMBox(true, false);
}
void MyUI::save_spiralGIF()
{
    if (ui.lineEdit_2->text() == "")
    {
        saveMBox(false);
        return;
    }
    string output_path = output_folderPath.toStdString();
    string filename = files[now].toStdString();
    size_t dotPosition = filename.find_last_of('.');
    string timestr;
    get_TimeStr(timestr);
    string basename = filename.substr(0, dotPosition) + timestr;

    waitingMBox();
    //Func.Spiral_GIF(star_img, seg_img, img, star_x, star_y, output_path, basename);
    Spiral_GIF(star_img, seg_img, mask_img, img, star_x, star_y, output_path, basename);
    saveMBox(true, false);
}
void MyUI::save_circularPNG()
{
    if (ui.lineEdit_2->text() == "")
    {
        saveMBox(false);
        return;
    }
    string output_path = output_folderPath.toStdString();
    string filename = files[now].toStdString();
    size_t dotPosition = filename.find_last_of('.');
    string basename = filename.substr(0, dotPosition);
    string timestr;
    get_TimeStr(timestr);
    if (mode == true) {
        imwrite(output_path + "/Circular_" + basename + timestr + ".png", Circular_outcome);
    }
    else {
        imwrite(output_path + "/StarSky_" + basename + timestr + ".png", Circular_outcome);
    }
    saveMBox();
}
void MyUI::save_radiationPNG()
{
    if (ui.lineEdit_2->text() == "")
    {
        saveMBox(false);
        return;
    }
    string output_path = output_folderPath.toStdString();
    string filename = files[now].toStdString();
    size_t dotPosition = filename.find_last_of('.');
    string basename = filename.substr(0, dotPosition);
    string timestr;
    get_TimeStr(timestr);
    imwrite(output_path + "/Radiation_" + basename + timestr + ".png", Radiation_outcome);
    saveMBox();
}
void MyUI::save_spiralPNG()
{
    if (ui.lineEdit_2->text() == "")
    {
        saveMBox(false);
        return;
    }
    string output_path = output_folderPath.toStdString();
    string filename = files[now].toStdString();
    size_t dotPosition = filename.find_last_of('.');
    string basename = filename.substr(0, dotPosition);
    string timestr;
    get_TimeStr(timestr);
    imwrite(output_path + "/Spiral_" + basename + timestr + ".png", Spiral_outcome);
    saveMBox();
}
void MyUI::work_Circular()
{
    /*
    addBack_img = seg_img + rotate_img;
    Func.addforeground(addBack_img, seg_img, img, img_out);
    Func.Rotation(star_img, rotate_img, star_x, star_y);
    addBack_img = seg_img + rotate_img;
    Func.addforeground(addBack_img, seg_img, img, img_out);
    Circular_outcome = img_out.clone();
    Mat rotate_img, addBack_img, img_out;
    Func.Rotation(star_img, rotate_img, star_x, star_y);
    addBack_img = seg_img + rotate_img;
    Func.addforeground(addBack_img, seg_img, img, img_out);
    Circular_outcome = img_out.clone();
    */
    Mat rotate_img, addBack_img, img_out;
    qDebug() << "1 : " << QString::number(img.rows) << QString::number(img.cols) << QString::number(img.channels());
    qDebug() << "2 : " << QString::number(seg_img.rows) << QString::number(seg_img.cols) << QString::number(seg_img.channels());
    qDebug() << "3 : " << QString::number(star_img.rows) << QString::number(star_img.cols) << QString::number(star_img.channels());
    qDebug() << "4 : " << QString::number(mask_img.rows) << QString::number(mask_img.cols) << QString::number(mask_img.channels());
    Circle_rotate(star_img, seg_img, mask_img, img, rotate_img, star_x, star_y);
    Circular_outcome = rotate_img.clone();
    end_time_1 = clock();
    QPixmap pixmap = QPixmap::fromImage(QImage(rotate_img.data, rotate_img.cols, rotate_img.rows, rotate_img.step, QImage::Format_BGR888));
    QSize scaledSize = pixmap.size().scaled(ui.label_11->size(), Qt::KeepAspectRatio);
    QPixmap scaled_pixmap = pixmap.scaled(scaledSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui.label_11->setPixmap(scaled_pixmap);
    QString printime = QString::number((end_time_1 - start_time) / CLOCKS_PER_SEC) + " sec";
    ui.label_13->setText(printime);
    qDebug() << "Circular Star trails is done!";
}
void MyUI::work_Radiation()
{
    Mat rotate_img, addBack_img, img_out;
    Radiation_rotate(star_img, seg_img, mask_img, img, rotate_img, star_x, star_y);
    Radiation_outcome = rotate_img.clone();
    /*
    Mat rotate_img, addBack_img, img_out;
    Func.Rotationline(star_img, rotate_img, star_x, star_y);
    addBack_img = seg_img + rotate_img;
    Func.addforeground(addBack_img, seg_img, img, img_out);
    Radiation_outcome = img_out.clone();
    */
    end_time_2 = clock();
    QPixmap pixmap = QPixmap::fromImage(QImage(rotate_img.data, rotate_img.cols, rotate_img.rows, rotate_img.step, QImage::Format_BGR888));
    QSize scaledSize = pixmap.size().scaled(ui.label_10->size(), Qt::KeepAspectRatio);
    QPixmap scaled_pixmap = pixmap.scaled(scaledSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui.label_10->setPixmap(scaled_pixmap);
    QString printime = QString::number((end_time_2 - start_time) / CLOCKS_PER_SEC) + " sec";
    ui.label_15->setText(printime);
    qDebug() << "Radiation Star trails is done!";
}
void MyUI::work_Spiral()
{
    Mat rotate_img, addBack_img, img_out;
    Spiral_rotate(star_img, seg_img, mask_img, img, rotate_img, star_x, star_y);
    Spiral_outcome = rotate_img.clone();
    /*
    Mat rotate_img, addBack_img, img_out;
    Func.Rotation2(star_img, rotate_img, star_x, star_y);
    addBack_img = seg_img + rotate_img;
    Func.addforeground(addBack_img, seg_img, img, img_out);
    Spiral_outcome = img_out.clone();
    */
    end_time_3 = clock();
    QPixmap pixmap = QPixmap::fromImage(QImage(rotate_img.data, rotate_img.cols, rotate_img.rows, rotate_img.step, QImage::Format_BGR888));
    QSize scaledSize = pixmap.size().scaled(ui.label_12->size(), Qt::KeepAspectRatio);
    QPixmap scaled_pixmap = pixmap.scaled(scaledSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui.label_12->setPixmap(scaled_pixmap);
    QString printime = QString::number((end_time_3 - start_time) / CLOCKS_PER_SEC) + " sec";
    ui.label_14->setText(printime);
    qDebug() << "Spiral Star trails is done!";
}
void MyUI::work_Star()
{
    Mat img_tmp, star_img_tmp;
    if (img.cols > 2000 or img.rows > 2000) {
        cv::resize(img, img_tmp, Size(img.cols / 4, img.rows / 4));
    }
    else{
        img_tmp = img.clone();
    }
    Func.filterAurora_1(img_tmp, star_img_tmp);//seg_img
    if (img.cols > 2000 or img.rows > 2000) {
        cv::resize(star_img_tmp, star_img, Size(img.cols, img.rows));
    }
    else {
        star_img = star_img_tmp.clone();
    }
    qDebug() << "Generate star image done !";

    //imshow("star_img", star_img);
    //waitKey(0);
    /*
    Point max_star;
    Func.calculateStar(star_img, max_star);
    qDebug() << "Max Star x:" << max_star.x << ", y:" << max_star.y;
    star_x = int(max_star.x);
    star_y = int(max_star.y);
    touch_x = star_x;
    touch_y = star_y;
    */
}
void MyUI::work_Crop()
{
    //Func.Resize(img, resize_img, 640);
    //Mat tmp;
    //Func.background(resize_img, tmp);
    //Func.reResize(tmp, seg_img, real_height, real_width);
    Func.newbackground(img, seg_img, mask_img, tmp_mask);
    qDebug() << "Segment done !";
}
void MyUI::starSky_work_Crop()
{
    Func.starSky_newbackground(img, seg_img, mask_img, tmp_mask);
    qDebug() << "Star sky Segment done !";
}
bool MyUI::eventFilter(QObject* obj, QEvent* event)
{
    if (obj == ui.label_16 and touch_lock == false)
    {
        if (event->type() == QEvent::MouseButtonPress)
        {
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);

            if (mouseEvent->button() == Qt::LeftButton)
            {
                qDebug() << "LeftButton";
                qDebug() << "label => X:" << mouseEvent->x() << " Y:" << mouseEvent->y();
                touch_x = round(mouseEvent->x() * (real_height - 1) / (seg_height - 1));
                touch_y = round(mouseEvent->y() * (real_width - 1) / (seg_width - 1));
                emit getPos(mouseEvent->x(), mouseEvent->y(), touch_x, touch_y, penO);
                return true;
            }
            else
            {
                qDebug() << "RightButton";
                qDebug() << "X:" << mouseEvent->x() << " Y:" << mouseEvent->y();
                return false;
            }
        }
        else
        {
            return false;
        }
    }
    else
    {
        return QWidget::eventFilter(obj, event);
    }
}
void MyUI::drawPos(int x, int y, int tx, int ty, QPen pen)
{
    QPixmap tmp = touchscreen;
    QPainter painter(&tmp);
    painter.setPen(pen);
    painter.drawEllipse(QPoint(x, y), 5, 5);
    ui.label_16->setPixmap(tmp);
    QString display_str = "(" + QString::number(x) + ", " + QString::number(y) + ") => (" + QString::number(tx) + ", " + QString::number(ty) + ")";
    ui.label_23->setText(display_str);
}
void MyUI::backPos()
{
    emit getPos(ceil(star_x * (seg_height - 1) / (real_height - 1)), ceil(star_y * (seg_width - 1) / (real_width - 1)), star_x, star_y, penR);
}
void MyUI::newPos()
{
    if (touch_x == star_x and touch_y == star_y)
    {
        QMessageBox MBox(QMessageBox::Warning, "", "");
        MBox.setWindowTitle("Notice");
        MBox.setText("You didn't change the center of the circle.");
        MBox.setWindowFlags(Qt::Dialog);
        QPushButton* CancelBut = MBox.addButton("Cancel", QMessageBox::AcceptRole);
        MBox.exec();
        qDebug() << "You didn/'t change the center of the circle";
    }
    else
    {
        QMessageBox MBox(QMessageBox::Question, "", "");
        MBox.setWindowTitle("Hint");
        MBox.setWindowFlags(Qt::Dialog);
        QString display_str = "from (" + QString::number(star_x) + ", " + QString::number(star_y) + ") to (" + QString::number(touch_x) + ", " + QString::number(touch_y) + ") ?";
        MBox.setText("Are you sure to change the center of the circle\n" + display_str);
        QPushButton* agreeBut = MBox.addButton("Agree", QMessageBox::AcceptRole);
        QPushButton* disagreeBut = MBox.addButton("Reject", QMessageBox::RejectRole);
        MBox.exec();
        if (MBox.clickedButton() == (QAbstractButton*)agreeBut) {
            qDebug() << "You successfully changed the center of the circle";
            star_x = touch_x;
            star_y = touch_y;
            emit getPos(ceil(touch_x * (seg_height - 1) / (real_height - 1)), ceil(touch_y * (seg_width - 1) / (real_width - 1)), touch_x, touch_y, penR);
            start_time = clock();
            emit sendUpdate();
        }
    }
}
void MyUI::pushLeft()
{
    if (now != 0) {
        now -= 1;
    }
    else {
        return;
    }
    QString displayNumber = QString::number(now + 1) + "/" + QString::number(itemCount);
    ui.label_20->setText(displayNumber);
    ui.label_21->setText(files[now]);
    ui.label_23->setText("");
    start_time = clock();
    ui.label_14->setText("");
    if (mode == true)
    {
        emit getImg((input_folderPath + "/" + files[now]), ui.label_9);
        emit getSegment((input_folderPath + "/" + files[now]), ui.label_10, ui.label_11, ui.label_12);
        emit getImg((input_folderPath + "/" + files[now]), ui.label_16, true);
    }
    else
    {
        emit getImg((input_folderPath + "/" + files[now]), ui.label_9);
        emit getSegment((input_folderPath + "/" + files[now]), ui.label_11);
    }
}
void MyUI::pushRight()
{
    if (now != itemCount-1) {
        now += 1;
    }
    else {
        return;
    }
    QString displayNumber = QString::number(now + 1) + "/" + QString::number(itemCount);
    ui.label_20->setText(displayNumber);
    ui.label_21->setText(files[now]);
    ui.label_23->setText("");
    start_time = clock();
    ui.label_14->setText("");
    if (mode == true)
    {
        emit getImg((input_folderPath + "/" + files[now]), ui.label_9);
        emit getSegment((input_folderPath + "/" + files[now]), ui.label_10, ui.label_11, ui.label_12);
        emit getImg((input_folderPath + "/" + files[now]), ui.label_16, true);
    }
    else
    {
        emit getImg((input_folderPath + "/" + files[now]), ui.label_9);
        emit getSegment((input_folderPath + "/" + files[now]), ui.label_11);
    }
}
void MyUI::postImg(const QString& filepath, QLabel* label, bool resize)
{
    QPixmap image(filepath);
    if (resize == true)
    {
        label->move(760, 400);
        label->setFixedSize(480, 270);
        QSize scaledSize0 = image.size().scaled(label->size(), Qt::KeepAspectRatio);
        QPixmap scaledPixmap = image.scaled(scaledSize0, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        touchscreen = scaledPixmap;
        label->setPixmap(scaledPixmap);
        label->setFixedSize(scaledPixmap.size());
        seg_width = scaledPixmap.width();
        seg_height =  scaledPixmap.height();
        label->move(760 + ((480 - scaledPixmap.width()) / 2), 400 + ((270 - scaledPixmap.height()) / 2));
        emit getPos(ceil(star_x * (seg_height - 1) / (real_height - 1)), ceil(star_y * (seg_width - 1) / (real_width - 1)), star_x, star_y, penR);
    }
    else
    {
        QSize scaledSize0 = image.size().scaled(label->size(), Qt::KeepAspectRatio);
        QPixmap scaledPixmap = image.scaled(scaledSize0, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        label->setPixmap(scaledPixmap);
    }
}
void MyUI::Update()
{
    std::thread work_1(&MyUI::work_Circular, this);
    std::thread work_2(&MyUI::work_Radiation, this);
    std::thread work_3(&MyUI::work_Spiral, this);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    work_1.join();
    work_2.join();
    work_3.join();
}
void MyUI::DoSeg(const QString& filepath, QLabel* label, QLabel* label_2, QLabel* label_3)
{
    string filename = filepath.toStdString();
    img = imread(filename, IMREAD_COLOR);
    real_height = img.rows;
    real_width = img.cols;
    
    if (mode == true)
    {
        std::thread work_1(&MyUI::work_Crop, this);
        std::thread work_2(&MyUI::work_Star, this);
        work_1.join();
        work_2.join();


        qDebug() << QString::number(img.rows) << QString::number(img.cols);
        qDebug() << QString::number(seg_img.rows) << QString::number(seg_img.cols);
        qDebug() << QString::number(star_img.rows) << QString::number(star_img.cols);
        qDebug() << QString::number(tmp_mask.rows) << QString::number(tmp_mask.cols);
        Mat star_img_2;
        cv::resize(star_img, star_img_2, Size(star_img.cols/4, star_img.rows/4), 0, 0, INTER_LINEAR);
        //imshow("star", star_img_2);
        //waitKey(0);
        Mat outcome;
        //cv::bitwise_and(star_img, star_img, outcome, tmp_mask);
        Mat tmp_mask_do;
        cvtColor(tmp_mask / 255, tmp_mask_do, COLOR_GRAY2BGR);
        multiply(star_img, tmp_mask_do, outcome);
        star_img = outcome.clone();
        //imshow("img", img);
        //waitKey(0);
        Mat star_img_3;
        cv::resize(star_img, star_img_3, Size(star_img.cols / 4, star_img.rows / 4), 0, 0, INTER_LINEAR);
        //imshow("star_2", star_img_3);
        //waitKey(0);
        //imshow("outcome", seg_img);
        //waitKey(0);
        //imshow("mask", mask_img);
        //waitKey(0);
        //imshow("tmp_mask", tmp_mask);
        //waitKey(0);
        //destroyAllWindows();

        Point max_star;
        Mat star_tmp;
        if (img.cols > 2000 or img.rows > 2000) {
            cv::resize(star_img, star_tmp, Size(img.cols/4, img.rows/4));
        }
        else {
            star_tmp = star_img.clone();
        }
        Func.calculateStar(star_tmp, max_star);
        if (img.cols > 2000 or img.rows > 2000) {
            star_x = int(max_star.x)*4;
            star_y = int(max_star.y)*4;
            qDebug() << "Max Star x:" << max_star.x << ", y:" << max_star.y;
        }
        else
        {
            star_x = int(max_star.x);
            star_y = int(max_star.y);
            qDebug() << "Max Star x:" << max_star.x << ", y:" << max_star.y;
        }
        touch_x = star_x;
        touch_y = star_y;

        //qDebug() << QString::number(img.rows) << QString::number(img.cols);
        //qDebug() << QString::number(seg_img.rows) << QString::number(seg_img.cols);
        //qDebug() << QString::number(star_img.rows) << QString::number(star_img.cols);
        //qDebug() << QString::number(tmp_mask.rows) << QString::number(tmp_mask.cols);
        std::thread work_3(&MyUI::work_Circular, this);
        std::thread work_4(&MyUI::work_Radiation, this);
        std::thread work_5(&MyUI::work_Spiral, this);
        std::this_thread::sleep_for(std::chrono::seconds(1));

        work_5.join();
        work_4.join();
        work_3.join();
    }
    else
    {
        std::thread work_1(&MyUI::starSky_work_Crop, this);
        work_1.join();

        Mat starsky, outcome;
        Func.restore_startrail(img, starsky);
        Func.addforeground(starsky, seg_img, img, outcome);
        Circular_outcome = outcome.clone();
        /*
        imshow("img", img);
        waitKey(0);
        imshow("seg_img", seg_img);
        waitKey(0);
        imshow("mask_img", mask_img);
        waitKey(0);
        imshow("outcome", outcome);
        waitKey(0);
        destroyAllWindows();
        */
        end_time_1 = clock();
        QPixmap pixmap = QPixmap::fromImage(QImage(outcome.data, outcome.cols, outcome.rows, outcome.step, QImage::Format_BGR888));
        QSize scaledSize = pixmap.size().scaled(ui.label_11->size(), Qt::KeepAspectRatio);
        QPixmap scaled_pixmap = pixmap.scaled(scaledSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        ui.label_11->setPixmap(scaled_pixmap);
        QString printime = QString::number((end_time_1 - start_time) / CLOCKS_PER_SEC) + " sec";
        ui.label_13->setText(printime);
        qDebug() << "Reserve the star sky is done!";
    }
}
void MyUI::loadImg()
{
    if (selected == false)
    {
        itemCount = files.size();
        ui.label_21->setText(files[now]);
        QString displayNumber = QString::number(now + 1) + "/" + QString::number(itemCount);
        ui.label_20->setText(displayNumber);
        start_time = clock();
        if (mode == true)
        {
            emit getImg((input_folderPath + "/" + files[now]), ui.label_9);
            emit getSegment((input_folderPath + "/" + files[now]), ui.label_10, ui.label_11, ui.label_12);
            emit getImg((input_folderPath + "/" + files[now]), ui.label_16, true);
        }
        else
        {
            emit getImg((input_folderPath + "/" + files[now]), ui.label_9);
            emit getSegment((input_folderPath + "/" + files[now]), ui.label_11);
        }
    }
    else
    {
        itemCount = files.size();
        ui.label_21->setText(files[now]);
        QString displayNumber = QString::number(now + 1) + "/" + QString::number(itemCount);
        ui.label_20->setText(displayNumber);
        start_time = clock();
        if (mode == true)
        {
            ui.label_21->setText(files[now]);
            emit getImg((input_folderPath + "/" + files[now]), ui.label_9);
            emit getSegment((input_folderPath + "/" + files[now]), ui.label_10, ui.label_11, ui.label_12);
            emit getImg((input_folderPath + "/" + files[now]), ui.label_16, true);
        }
        else
        {
            ui.label_21->setText(files[now]);
            emit getImg((input_folderPath + "/" + files[now]), ui.label_9);
            emit getSegment((input_folderPath + "/" + files[now]), ui.label_11);
        }
    }
}
void MyUI::searchFolder(const QString& path)
{
    QDir folder(path);
    if (!folder.exists()) {
        qDebug() << "Folder not found:" << path;
        return;
    }
    ui.pushButton_11->setEnabled(true);
    ui.pushButton_12->setEnabled(true);
    QStringList nameFilters;
    nameFilters << "*.jpg" << "*.jpeg";
    files = folder.entryList(nameFilters, QDir::Files);
    qDebug() << "Files in folder:" << path;
    foreach(const QString & file, files) {
        qDebug() << file;
    }
    emit getFiles();
}
void MyUI::input_browse()
{
    if (selected == false) {
        QFileDialog myFileDialog(this);
        input_folderPath = myFileDialog.getExistingDirectory(this, tr("Open Folder"), QDir::currentPath());
        if (input_folderPath.isEmpty()) {
            qDebug() << "No folder selected. Please choose a folder.";
            return;
        }
        refresh();
        touch_lock = false;
        ui.lineEdit->setText(input_folderPath);
        emit getInputPath(input_folderPath);
    }
    else {
        QFileDialog myFileDialog(this);
        QString input_filePath = myFileDialog.getOpenFileName(this, tr("Open File"), QDir::currentPath());
        if (input_filePath.isEmpty()) {
            qDebug() << "No files selected. Please choose a file.";
            return;
        }
        refresh();
        touch_lock = false;
        ui.lineEdit->setText(input_filePath);
        QFileInfo fileInfo(input_filePath);
        input_folderPath = fileInfo.path();
        QString filename = fileInfo.fileName();
        files << filename;
        emit getFiles();
    }
}
void MyUI::output_browse()
{
    QFileDialog myFileDialog(this);
    output_folderPath = myFileDialog.getExistingDirectory(this, tr("Open Folder"), QDir::currentPath());
    if (output_folderPath.isEmpty()) {
        qDebug() << "No folder selected. Please choose a folder.";
        return;
    }
    ui.lineEdit_2->setText(output_folderPath);
}
void MyUI::changeFolder()
{
    ui.label->setText("InputFolder :");
    selected = false;
    touch_lock = true;
    ui.lineEdit->setText("");
    ui.label_21->setText("");
    ui.label_23->setText("");
    ui.label_16->move(760, 400);
    ui.label_16->setFixedSize(480, 270);
    QPixmap blank("ui_img/blank.jpg");
    ui.label_9->setPixmap(blank);
    ui.label_10->setPixmap(blank);
    ui.label_11->setPixmap(blank);
    ui.label_12->setPixmap(blank);
    ui.label_16->setPixmap(blank);
    files.clear();
    now = 0;
    itemCount = 0;
    ui.label_20->setText("0/0");
    ui.pushButton_11->setEnabled(true);
    ui.pushButton_12->setEnabled(true);
    ui.pushButton_3->setEnabled(false);
    ui.pushButton_4->setEnabled(false);
    ui.pushButton_5->setEnabled(false);
    ui.pushButton_6->setEnabled(false);
    ui.pushButton_7->setEnabled(false);
    ui.pushButton_8->setEnabled(false);
}
void MyUI::changeFile()
{
    ui.label->setText("InputFile :");
    selected = true;
    touch_lock = true;
    ui.lineEdit->setText("");
    ui.label_21->setText("");
    ui.label_23->setText("");
    ui.label_16->move(760, 400);
    ui.label_16->setFixedSize(480, 270);
    QPixmap blank("ui_img/blank.jpg");
    ui.label_9->setPixmap(blank);
    ui.label_10->setPixmap(blank);
    ui.label_11->setPixmap(blank);
    ui.label_12->setPixmap(blank);
    ui.label_16->setPixmap(blank);
    files.clear();
    now = 0;
    itemCount = 0;
    ui.label_20->setText("0/0");
    ui.pushButton_11->setEnabled(false);
    ui.pushButton_12->setEnabled(false);
    ui.pushButton_3->setEnabled(false);
    ui.pushButton_4->setEnabled(false);
    ui.pushButton_5->setEnabled(false);
    ui.pushButton_6->setEnabled(false);
    ui.pushButton_7->setEnabled(false);
    ui.pushButton_8->setEnabled(false);
}
void MyUI::generateMode() {
    mode = true;
    touch_lock = true;
    ui.label_16->move(760, 400);
    ui.label_16->setFixedSize(480, 270);
    QPixmap blank("ui_img/blank.jpg");
    ui.label_9->setPixmap(blank);
    ui.label_10->setPixmap(blank);
    ui.label_11->setPixmap(blank);
    ui.label_12->setPixmap(blank);
    ui.label_16->setPixmap(blank);
    ui.lineEdit->setText("");
    ui.label_21->setText("");
    ui.label_23->setText("");
    ui.label_13->setText("");
    ui.label_14->setText("");
    ui.label_15->setText("");
    ui.label_20->setText("0/0");
    ui.label_5->setText("Circular");
    files.clear();
    now = 0;
    itemCount = 0;
    ui.label_6->show();
    ui.label_7->show();
    ui.label_10->show();
    ui.label_12->show();
    ui.label_14->show();
    ui.label_15->show();
    ui.label_16->show();
    ui.label_17->show();
    ui.pushButton_4->show();
    ui.pushButton_5->show();
    ui.pushButton_6->show();
    ui.pushButton_7->show();
    ui.pushButton_8->show();
    ui.pushButton_9->show();
    ui.pushButton_10->show();
    ui.pushButton_3->setEnabled(false);
    ui.pushButton_4->setEnabled(false);
    ui.pushButton_5->setEnabled(false);
    ui.pushButton_6->setEnabled(false);
    ui.pushButton_7->setEnabled(false);
    ui.pushButton_8->setEnabled(false);
}
void MyUI::restoreMode() {
    mode = false;
    touch_lock = true;
    ui.label_16->move(760, 400);
    ui.label_16->setFixedSize(480, 270);
    QPixmap blank("ui_img/blank.jpg");
    ui.label_9->setPixmap(blank);
    ui.label_10->setPixmap(blank);
    ui.label_11->setPixmap(blank);
    ui.label_12->setPixmap(blank);
    ui.label_16->setPixmap(blank);
    ui.lineEdit->setText("");
    ui.label_21->setText("");
    ui.label_23->setText("");
    ui.label_13->setText("");
    ui.label_14->setText("");
    ui.label_15->setText("");
    ui.label_20->setText("0/0");
    ui.label_5->setText("Starry Sky");
    files.clear();
    now = 0;
    itemCount = 0;
    ui.label_6->hide();
    ui.label_7->hide();
    ui.label_10->hide();
    ui.label_12->hide();
    ui.label_14->hide();
    ui.label_15->hide();
    ui.label_16->hide();
    ui.label_17->hide();
    ui.pushButton_4->hide();
    ui.pushButton_5->hide();
    ui.pushButton_6->hide();
    ui.pushButton_7->hide();
    ui.pushButton_8->hide();
    ui.pushButton_9->hide();
    ui.pushButton_10->hide();
    ui.pushButton_3->setEnabled(false);
    ui.pushButton_4->setEnabled(false);
    ui.pushButton_5->setEnabled(false);
    ui.pushButton_6->setEnabled(false);
    ui.pushButton_7->setEnabled(false);
    ui.pushButton_8->setEnabled(false);
}
void MyUI::refresh() {
    touch_lock = true;
    ui.label_16->move(760, 400);
    ui.label_16->setFixedSize(480, 270);
    QPixmap blank("ui_img/blank.jpg");
    ui.label_9->setPixmap(blank);
    ui.label_10->setPixmap(blank);
    ui.label_11->setPixmap(blank);
    ui.label_12->setPixmap(blank);
    ui.label_13->setText("");
    ui.label_14->setText("");
    ui.label_15->setText("");
    ui.label_16->setPixmap(blank);
    ui.lineEdit->setText("");
    ui.label_21->setText("");
    ui.label_23->setText("");
    ui.label_20->setText("0/0");
    files.clear();
    now = 0;
    itemCount = 0;
    ui.pushButton_3->setEnabled(true);
    ui.pushButton_4->setEnabled(true);
    ui.pushButton_5->setEnabled(true);
    ui.pushButton_6->setEnabled(true);
    ui.pushButton_7->setEnabled(true);
    ui.pushButton_8->setEnabled(true);
}