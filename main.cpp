#include "ujlui.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("UJLSDE");
    a.setOrganizationName("zungle");
    ujlui w;
    w.show();
    return a.exec();
}
