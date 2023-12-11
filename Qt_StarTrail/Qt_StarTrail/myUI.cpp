#include "myUI.h"
#include "func.h"
#include <QFileDialog>
#include <QStringList>
#include <QDebug>
#include <QDir>
#include <QStyle>
#include <QPixmap>
#include <QImage>
#include <QImageReader>

MyUI::MyUI(QWidget *parent) : QWidget(parent)
{
    ui.setupUi(this);
    ui.radioButton->setChecked(true);
    ui.pushButton_11->setEnabled(false);
    ui.pushButton_12->setEnabled(false);
    QImageReader::setAllocationLimit(0);
    connect(ui.pushButton, SIGNAL(clicked()), SLOT(input_browse()));
    connect(ui.pushButton_2, SIGNAL(clicked()), SLOT(output_browse()));
    connect(ui.pushButton_11, SIGNAL(clicked()), SLOT(pushLeft()));
    connect(ui.pushButton_12, SIGNAL(clicked()), SLOT(pushRight()));
    connect(this, &MyUI::getInputPath, this, &MyUI::searchFolder);
    connect(this, &MyUI::getFiles, this, &MyUI::loadImg);
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
    QPixmap image(input_folderPath + "/" + files[now]);
    QSize scaledSize0 = image.size().scaled(ui.label_9->size(), Qt::KeepAspectRatio);
    QPixmap scaledPixmap0 = image.scaled(scaledSize0, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui.label_9->setPixmap(scaledPixmap0);
    ui.label_10->setPixmap(scaledPixmap0);
    ui.label_11->setPixmap(scaledPixmap0);
    ui.label_12->setPixmap(scaledPixmap0);
    QSize scaledSize = image.size().scaled(ui.label_16->size(), Qt::KeepAspectRatio);
    QPixmap scaledPixmap = image.scaled(scaledSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui.label_16->setPixmap(scaledPixmap);
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
    QPixmap image(input_folderPath + "/" + files[now]);
    QSize scaledSize0 = image.size().scaled(ui.label_9->size(), Qt::KeepAspectRatio);
    QPixmap scaledPixmap0 = image.scaled(scaledSize0, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui.label_9->setPixmap(scaledPixmap0);
    ui.label_10->setPixmap(scaledPixmap0);
    ui.label_11->setPixmap(scaledPixmap0);
    ui.label_12->setPixmap(scaledPixmap0);
    QSize scaledSize = image.size().scaled(ui.label_16->size(), Qt::KeepAspectRatio);
    QPixmap scaledPixmap = image.scaled(scaledSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    //qDebug() << "label :" << ui.label_16->height() << ui.label_16->width();
    //qDebug() << "scaledPixmap :" << scaledPixmap.height() << scaledPixmap.width();
    ui.label_16->setPixmap(scaledPixmap);
}
void MyUI::loadImg()
{
    itemCount = files.size();
    qDebug() << "Number of items in QStringList:" << itemCount;
    ui.label_21->setText(files[now]);
    QString displayNumber = QString::number(now + 1) + "/" + QString::number(itemCount);
    ui.label_20->setText(displayNumber);
    QPixmap image(input_folderPath + "/" + files[now]);
    QSize scaledSize0 = image.size().scaled(ui.label_9->size(), Qt::KeepAspectRatio);
    QPixmap scaledPixmap0 = image.scaled(scaledSize0, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui.label_9->setPixmap(scaledPixmap0);
    ui.label_10->setPixmap(scaledPixmap0);
    ui.label_11->setPixmap(scaledPixmap0);
    ui.label_12->setPixmap(scaledPixmap0);
    QSize scaledSize = image.size().scaled(ui.label_16->size(), Qt::KeepAspectRatio);
    QPixmap scaledPixmap = image.scaled(scaledSize, Qt::KeepAspectRatio);
    //qDebug() << "label :" << ui.label_16->height() << ui.label_16->width();
    //qDebug() << "scaledPixmap :" << scaledPixmap.height() << scaledPixmap.width();
    ui.label_16->setPixmap(scaledPixmap);
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
    QFileDialog myFileDialog(this);
    input_folderPath = myFileDialog.getExistingDirectory(this, tr("Open Folder"), QDir::currentPath());
    if (input_folderPath.isEmpty()) {
        qDebug() << "No folder selected. Please choose a folder.";
        return;
    }
    ui.lineEdit->setText(input_folderPath);
    emit getInputPath(input_folderPath);
    //Func myFunc;
    //myFunc.loadImg(input_folderPath);
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
