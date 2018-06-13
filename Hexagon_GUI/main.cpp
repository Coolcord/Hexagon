#include "../Hexagon/Hexagon_Interface.h"
#include "Common_Strings.h"
#include "Error_Messages.h"
#include "Main_Window.h"
#include <QApplication>
#include <QPluginLoader>
#include <QMessageBox>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    //Load the Hexagon Plugin
    QString unableToLoadPluginMessage = Common_Strings::STRING_HEXAGON+" is unable to load the backend plugin! Make sure the plugin is in the plugins folder!";
    QPluginLoader *pluginLoader = new QPluginLoader(Common_Strings::STRING_PLUGIN_LOCATION);
    QObject *validPlugin = pluginLoader->instance();
    Error_Messages errorMessages;
    if (!validPlugin) {
        errorMessages.Show_Error(unableToLoadPluginMessage);
        return 1;
    }
    Hexagon_Interface *hexagonPlugin = qobject_cast<Hexagon_Interface*>(validPlugin);
    if (!hexagonPlugin) {
        errorMessages.Show_Error(unableToLoadPluginMessage);
        return 1;
    }

    //Show the Main Window
    Main_Window w(NULL, hexagonPlugin, &errorMessages);
    w.show();

    return a.exec();
}
