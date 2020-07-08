#include <iostream>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <QApplication>

#include "mainwindow.h"
#include "vulkanwindow.h"

const uint32_t WIDTH = 1920/2;
const uint32_t HEIGHT = 1080/2;

int main(int argc, char* argv[]) {
	QApplication a(argc, argv);

	VulkanWindow* vulkanwindow = new VulkanWindow();

	MainWindow mainwindow(vulkanwindow);
//		mainwindow.resize(WIDTH, HEIGHT);
	mainwindow.show();

	vulkanwindow->Run();


	return a.exec();
}