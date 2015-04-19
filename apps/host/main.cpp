#include <iostream>
#include <boost/thread/thread.hpp>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <manAudioDevice/manAudioDeviceList.h>
#include <manAudioDevice/manAudioDevice.h>

int main(int argc, char *argv[])
{
	std::cout << "TestApp" << std::endl;

	manAudioDeviceList list;
	std::vector<std::string> deviceList = list.enumerateDevices();
	std::for_each(deviceList.begin(), deviceList.end(), [](std::string& element){ std::cout << element; });
	std::auto_ptr<manAudioDevice> device(list.create(deviceList.front()));
	device->start();

	QApplication app(argc, argv);
	QQmlApplicationEngine engine;
	engine.load(QUrl("qrc:/main.qml"));
	app.exec();

	return 0;
}
