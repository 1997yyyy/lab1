#include <QApplication>
#include "colorconverter.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    ColorConverter converter;
    converter.show();
    return app.exec();
}
