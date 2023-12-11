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
#include <QButtonGroup>
#include <QFileInfo>


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
    ui.label_16->setPixmap(scaledPixmap);
}
void MyUI::loadImg()
{
    itemCount = files.size();
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