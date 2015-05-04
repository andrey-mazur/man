#include <iostream>
#include <QApplication>
#include <QQmlApplicationEngine>
#include "manAudioEngine.h"


int main(int argc, char *argv[])
{
	std::cout << "TestApp" << std::endl;

	manAudioEngine audioEngine;

	QApplication app(argc, argv);
	QQmlApplicationEngine engine;
	engine.load(QUrl("qrc:/main.qml"));
	app.exec();

	return 0;
}
