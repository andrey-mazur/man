#include <iostream>
#include <boost/thread/thread.hpp>
#include <QApplication>
#include <QDeclarativeView>
#if Q_OS_WIN
#include <asiosdk/common/asio.h>
#endif

int main(int argc, char *argv[])
{
  std::cout << "TestApp" << std::endl;

  QApplication app(argc, argv);
  QDeclarativeView * view = new QDeclarativeView();
  view->setSource(QUrl(""));
  view->show();
  app.exec();

  return 0;
}
