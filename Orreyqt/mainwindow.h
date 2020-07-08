#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include<QtWidgets/QSlider>
#include <QtWidgets/QTreeWidget>

QT_BEGIN_NAMESPACE
class QLCDNumber;
class QLabel;
class QPushButton;
class QCheckBox;
QT_END_NAMESPACE

class VulkanWindow;

class MainWindow : public QWidget
{
	Q_OBJECT
public slots:
	void onGrabRequested();
	void changeSpeed(int value);
public:
	MainWindow(VulkanWindow* vulkanwindow);
	~MainWindow();

private:

	void closeEvent(QCloseEvent* e) override;


	QLCDNumber* FPSLcd;
	QPushButton* grabButton;
	QPushButton* quitButton;
	QPushButton* pauseButton;
	QPushButton* hideButton;
	VulkanWindow* vulkanwindow;
	QSlider* speedSlider;
	QTreeWidget* treeWidget;
};

#endif
