#include "graphsdigitiser.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    graphsdigitiser w;
    w.show();

    return a.exec();
}
