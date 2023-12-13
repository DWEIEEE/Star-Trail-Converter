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

using namespace std;
using namespace cv;

MyUI::MyUI(QWidget *parent) : QWidget(parent)
{
    ui.setupUi(this);
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
    connect(ui.pushButton, SIGNAL(clicked()), SLOT(input_browse()));
    connect(ui.pushButton_2, SIGNAL(clicked()), SLOT(output_browse()));
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
    if (mode == true)
    {
        emit getImg((input_folderPath + "/" + files[now]), ui.label_9);
        emit getSegment((input_folderPath + "/" + files[now]), ui.label_10, ui.label_11, ui.label_12);
        emit getImg((input_folderPath + "/" + files[now]), ui.label_16);
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
    if (mode == true)
    {
        emit getImg((input_folderPath + "/" + files[now]), ui.label_9);
        emit getSegment((input_folderPath + "/" + files[now]), ui.label_10, ui.label_11, ui.label_12);
        emit getImg((input_folderPath + "/" + files[now]), ui.label_16);
    }
    else
    {
        emit getImg((input_folderPath + "/" + files[now]), ui.label_9);
        emit getSegment((input_folderPath + "/" + files[now]), ui.label_11);
    }
}
void MyUI::postImg(const QString& filepath, QLabel* label)
{
    QPixmap image(filepath);
    QSize scaledSize0 = image.size().scaled(label->size(), Qt::KeepAspectRatio);
    QPixmap scaledPixmap = image.scaled(scaledSize0, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    label->setPixmap(scaledPixmap);
}
void MyUI::DoSeg(const QString& filepath, QLabel* label, QLabel* label_2, QLabel* label_3)
{
    string filename = filepath.toStdString();
    Mat img = imread(filename, IMREAD_COLOR);
    Mat outimg = img.clone();
    Func.background(img, outimg);
    QPixmap pixmap = QPixmap::fromImage(QImage(outimg.data, outimg.cols, outimg.rows, outimg.step, QImage::Format_BGR888));
    QSize scaledSize = pixmap.size().scaled(label->size(), Qt::KeepAspectRatio);
    QPixmap scaled_pixmap = pixmap.scaled(scaledSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    label->setPixmap(scaled_pixmap);
    if (label_2 != nullptr){
        label_2->setPixmap(scaled_pixmap);
    }
    if (label_3 != nullptr) {
        label_3->setPixmap(scaled_pixmap);
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
        if (mode = true)
        {
            emit getImg((input_folderPath + "/" + files[now]), ui.label_9);
            emit getSegment((input_folderPath + "/" + files[now]), ui.label_10, ui.label_11, ui.label_12);
            emit getImg((input_folderPath + "/" + files[now]), ui.label_16);
        }
        else
        {
            emit getImg((input_folderPath + "/" + files[now]), ui.label_9);
            emit getSegment((input_folderPath + "/" + files[now]), ui.label_11);
        }
    }
    else
    {
        if (mode = true)
        {
            ui.label_21->setText(files[now]);
            emit getImg((input_folderPath + "/" + files[now]), ui.label_9);
            emit getSegment((input_folderPath + "/" + files[now]), ui.label_10, ui.label_11, ui.label_12);
            emit getImg((input_folderPath + "/" + files[now]), ui.label_16);
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
    QString output_folderPath = myFileDialog.getExistingDirectory(this, tr("Open Folder"), QDir::currentPath());
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
    ui.lineEdit->setText("");
    ui.label_21->setText("");
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
}
void MyUI::changeFile()
{
    ui.label->setText("InputFile :");
    selected = true;
    ui.lineEdit->setText("");
    ui.label_21->setText("");
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
}
void MyUI::generateMode() {
    mode = true;
    QPixmap blank("ui_img/blank.jpg");
    ui.label_9->setPixmap(blank);
    ui.label_10->setPixmap(blank);
    ui.label_11->setPixmap(blank);
    ui.label_12->setPixmap(blank);
    ui.label_16->setPixmap(blank);
    ui.lineEdit->setText("");
    ui.label_21->setText("");
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
}
void MyUI::restoreMode() {
    mode = false;
    QPixmap blank("ui_img/blank.jpg");
    ui.label_9->setPixmap(blank);
    ui.label_10->setPixmap(blank);
    ui.label_11->setPixmap(blank);
    ui.label_12->setPixmap(blank);
    ui.label_16->setPixmap(blank);
    ui.lineEdit->setText("");
    ui.label_21->setText("");
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
}