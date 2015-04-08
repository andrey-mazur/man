#include <iostream>
#include <boost/thread/thread.hpp>
#include <asiosdk/common/asio.h>
#include <QGuiApplication>

int main(int argc, char *argv[])
{
  std::cout << "TestApp" << std::endl;

  QGuiApplication app(argc, argv);
  app.exec();

  return 0;
}
