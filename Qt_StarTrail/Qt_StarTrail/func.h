#pragma once
#include <QString>
#include <QtWidgets/QWidget>
#include "ui_UI.h"

class Func : public QWidget
{
    Q_OBJECT

    public:
        Func(QWidget* parent = nullptr);
        void loadImg(QString);

    private:
        Ui::MyUIClass ui;
};
