#include "uimainwindow.h"

#include <QApplication>

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);

  QCoreApplication::setOrganizationName("waj334");
  QCoreApplication::setOrganizationDomain("waj334.com");
  QCoreApplication::setApplicationName("Open Stackup");
  QCoreApplication::setApplicationVersion("0.0.0");

  //Create the main window
  UIMainWindow window;
  window.show();

  return app.exec();;
}