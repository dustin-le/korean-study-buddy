#include "MainWindow.h"
#include "study.h"
#include <QApplication>
#include <time.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    std::srand(std::time(NULL));

    w.show();

    return a.exec();
}
