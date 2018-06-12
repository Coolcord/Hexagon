#include <QCoreApplication>
#include "../Hexagon/Hexagon_Interface.h"
#include "../Hexagon_GUI/Common_Strings.h"
#include "Test_Cases.h"
#include <QDebug>
#include <QPluginLoader>

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    //Load the Hexagon Plugin
    QString unableToLoadPluginMessage = Common_Strings::STRING_HEXAGON+" is unable to load the backend plugin! The plugin may not be valid.";
    QPluginLoader *pluginLoader = new QPluginLoader(Common_Strings::STRING_PLUGIN_LOCATION);
    QObject *validPlugin = pluginLoader->instance();
    if (!validPlugin) {
        qDebug() << unableToLoadPluginMessage;
        return 1;
    }
    Hexagon_Interface *hexagonPlugin = qobject_cast<Hexagon_Interface*>(validPlugin);
    if (!hexagonPlugin) {
        qDebug() << unableToLoadPluginMessage;
        return 1;
    }

    Test_Cases(hexagonPlugin).Run_Test_Cases();
    return a.exec();
}
