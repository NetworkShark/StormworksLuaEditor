#include "mainwindow.h"
#include "structs.h"

#include <QApplication>

using namespace std;

#define configFilePath "mainConfig.conf"
Settings globalSettings;

int main(int argc, char *argv[])
{
    Settings* newSettings = Settings::readConfig(configFilePath);
    if (newSettings) {
        globalSettings = *newSettings;
    }
    else {
        globalSettings.init();
        globalSettings.fileSourcePath = configFilePath;
        globalSettings.writeConfig(configFilePath);
    }

    QApplication a(argc, argv);
    MainWindow w;
    w.globalSettings = &globalSettings;

    w.show();
    return a.exec();
}
