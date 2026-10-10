#include <QApplication>

#include "appManager.cpp";

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    AppManager manager;

    manager.show();

    return app.exec();
}
