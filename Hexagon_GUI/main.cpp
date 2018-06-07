#include "../Hexagon/Hexagon_Interface.h"
#include "Common_Strings.h"
#include "Main_Window.h"
#include <QApplication>
#include <QPluginLoader>
#include <QMessageBox>

void Show_Plugin_Error_Message() {
    QMessageBox::critical(NULL, Common_Strings::STRING_HEXAGON, Common_Strings::STRING_HEXAGON +
                             " is unable to load the backend plugin! The plugin may not be valid.",
                          Common_Strings::STRING_OK);
}

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    //Load the Hexagon Plugin
    QPluginLoader *pluginLoader = new QPluginLoader(Common_Strings::STRING_PLUGIN_LOCATION);
    QObject *validPlugin = pluginLoader->instance();
    if (!validPlugin) {
        Show_Plugin_Error_Message();
        return 1;
    }
    Hexagon_Interface *hexagonPlugin = qobject_cast<Hexagon_Interface*>(validPlugin);
    if (!hexagonPlugin) {
        Show_Plugin_Error_Message();
        return 1;
    }

    //Show the Main Window
    Main_Window w(NULL, hexagonPlugin);
    w.show();

    return a.exec();
}
