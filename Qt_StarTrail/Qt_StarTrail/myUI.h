#pragma once
#include <QtWidgets/QWidget>
#include "ui_UI.h"

class MyUI : public QWidget
{
    Q_OBJECT

public:
    MyUI(QWidget *parent = nullptr);

signals:
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
    void loadImg();

private:
    Ui::MyUIClass ui;
    QStringList files;
    int now = 0;
    int itemCount = 0;
    QString input_folderPath;
    bool selected = true;
};
