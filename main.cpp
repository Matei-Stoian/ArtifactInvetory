#include <QApplication>
#include <QPushButton>
#include "include/domain/Artifact.h"
#include "include/ui/MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return QApplication::exec();
}
