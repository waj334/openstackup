#include "uimainwindow.h"
#include "pluginserver.h"
#include <QApplication>

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);

  QCoreApplication::setOrganizationName("waj334");
  QCoreApplication::setOrganizationDomain("waj334.com");
  QCoreApplication::setApplicationName("Open Stackup");
  QCoreApplication::setApplicationVersion("0.0.0");

  int ret = -1;

  //Initialize and start the plugin server
  PluginServer pluginServer;

  if (pluginServer.start()) {
    //Create the main window
    UIMainWindow window;
    window.show();

    ret = app.exec();;
  }

  return ret;
}