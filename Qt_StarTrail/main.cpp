#include "myUI.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MyUI w;
    w.show();
    return a.exec();
}
