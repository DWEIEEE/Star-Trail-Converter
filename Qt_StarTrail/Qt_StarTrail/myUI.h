#pragma once
#include <QtWidgets/QWidget>
#include <QMouseEvent>
#include "ui_UI.h"
#include "func.h"

class MyUI : public QWidget
{
    Q_OBJECT

public:
    explicit MyUI(QWidget* parent = nullptr);
    bool eventFilter(QObject* obj, QEvent* event);

signals:
    void getSegment(const QString& string, QLabel* label, QLabel* label_2 = nullptr, QLabel* label_3 = nullptr);
    void getImg(const QString& string, QLabel* label, bool resize = false);
    void getInputPath(const QString& string);
    void getPos(int x, int y, int tx, int ty, QPen pen);
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
    void DoSeg(const QString& string, QLabel* label, QLabel* label_2 = nullptr, QLabel* label_3 = nullptr);
    void postImg(const QString& string, QLabel* label, bool resize = false);
    void loadImg();
    void drawPos(int x, int y, int tx, int ty, QPen pen);
    void backPos();
    void newPos();
    void refresh();

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
    int seg_height;
    int seg_width;
    int touch_x;
    int touch_y;
    int star_x;
    int star_y;
    QPen penO;
    QPen penR;
};
