#pragma once
#include <QtWidgets/QWidget>
#include "ui_UI.h"
#include "func.h"

class MyUI : public QWidget
{
    Q_OBJECT

public:
    explicit MyUI(QWidget *parent = nullptr);

signals:
    void getSegment(const QString& string, QLabel* label);
    void getImg(const QString& string, QLabel* label);
    void getInputPath(const QString& string);
    void getFiles();

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
    void postSeg(const QString& string, QLabel* label);
    void postImg(const QString& string, QLabel* label);
    void loadImg();

private:
    Ui::MyUIClass ui;
    Func Func;
    QStringList files;
    int now = 0;
    int itemCount = 0;
    QString input_folderPath;
    bool selected = true;
    bool mode = true;
};
