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
	void closeEvent(QCloseEvent* e) override;
	void resizeEvent(QResizeEvent* ev) override;

	void setupUI();

	VulkanWindow* vulkanwindow;
	QLCDNumber* FPSLcd;
	QPushButton* grabButton;
	QPushButton* quitButton;
	QPushButton* pauseButton;
	QPushButton* hideButton;
	QSlider* speedSlider;
	QTreeWidget* treeWidget;
	QTextBrowser* textBrowser;
	QLabel* FPSLabel;
	QPushButton* addNumButton;
	QPushButton* subSizeButton;
	QPushButton* addSizeButton;
	QLCDNumber* planetSizeLcd;
	QLabel* speedLabel;
	QLabel* planetSizeLabel;
	QPushButton* subNumButton;
	QLabel* astroidNumlabel;
	QLCDNumber* astroidNumLcd;

	QRect lastGeometry;
	int lastFPS=0;
};

#endif
