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

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);

	VulkanWindow* vulkanwindow = new VulkanWindow();

	MainWindow mainwindow(vulkanwindow);
	mainwindow.show();

	vulkanwindow->startRender();


	return a.exec();
}