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

using namespace std;
using namespace cv;

MyUI::MyUI(QWidget *parent) : QWidget(parent)
{
    ui.setupUi(this);
    setFixedSize(1290, 720);
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
                touch_x = round(mouseEvent->x() * (origin_height - 1) / (seg_height - 1));
                touch_y = round(mouseEvent->y() * (origin_width - 1) / (seg_width - 1));
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
    emit getPos(ceil(star_x * (seg_height - 1) / (origin_height - 1)), ceil(star_y * (seg_width - 1) / (origin_width - 1)), star_x, star_y, penR);
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
            emit getPos(ceil(touch_x * (seg_height - 1) / (origin_height - 1)), ceil(touch_y * (seg_width - 1) / (origin_width - 1)), touch_x, touch_y, penR);
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
        emit getPos(ceil(star_x * (seg_height - 1) / (origin_height - 1)), ceil(star_y * (seg_width - 1) / (origin_width - 1)), star_x, star_y, penR);
    }
    else
    {
        QSize scaledSize0 = image.size().scaled(label->size(), Qt::KeepAspectRatio);
        QPixmap scaledPixmap = image.scaled(scaledSize0, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        label->setPixmap(scaledPixmap);
    }
}
void MyUI::DoSeg(const QString& filepath, QLabel* label, QLabel* label_2, QLabel* label_3)
{
    string filename = filepath.toStdString();
    Mat img = imread(filename, IMREAD_COLOR);
    Mat resize_img;
    Func.Resize(img, resize_img, 640);
    origin_height = resize_img.rows;
    origin_width = resize_img.cols;

    //Mat segimg;
    //Func.background(resize_img, segimg);
    //qDebug() << "Segment done !";

    Mat star_img;
    Func.filterAurora_1(resize_img, star_img);
    qDebug() << "Generate star image done !";

    //Mat mask;
    //Func.getMask(resize_img, mask);
    //qDebug() << "get Mask !";
    //Mat result;
    //multiply(star_img, mask, result);

    Point max_star;
    vector<StarInfo> stars = Func.calculateStar(star_img, max_star);
    qDebug() << "Max Star x:" << max_star.x << ", y:" << max_star.y;
    star_x = int(max_star.x);
    star_y = int(max_star.y);
    touch_x = star_x;
    touch_y = star_y;

    QPixmap pixmap = QPixmap::fromImage(QImage(star_img.data, star_img.cols, star_img.rows, star_img.step, QImage::Format_BGR888));
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
void MyUI::refresh() {
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
    ui.label_20->setText("0/0");
    files.clear();
    now = 0;
    itemCount = 0;
}