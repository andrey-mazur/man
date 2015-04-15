#include <iostream>
#include <boost/thread/thread.hpp>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <manAudioDevice/manAudioDeviceList.h>

int main(int argc, char *argv[])
{
	std::cout << "TestApp" << std::endl;

	QApplication app(argc, argv);
	QQmlApplicationEngine engine;
	engine.load(QUrl("qrc:/main.qml"));
	app.exec();

	return 0;
}
