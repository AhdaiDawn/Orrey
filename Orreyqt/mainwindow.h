#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QLCDNumber>
#include <QPushButton>
#include <QGridLayout>
#include<QDateTime>
#include<QScreen>
#include<QtWidgets/QSlider>
#include<QFileDialog>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QTextBrowser>

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
	void resizeEvent(QResizeEvent* ev) override;
	void closeEvent(QCloseEvent* e) override;

	void settingUI();

	QLCDNumber* FPSLcd;
	QPushButton* grabButton;
	QPushButton* quitButton;
	QPushButton* pauseButton;
	QPushButton* hideButton;
	VulkanWindow* vulkanwindow;
	QSlider* speedSlider;
	QTreeWidget* treeWidget;
	QTextBrowser* textBrowser;

	QRect lastGeometry;
};

#endif
