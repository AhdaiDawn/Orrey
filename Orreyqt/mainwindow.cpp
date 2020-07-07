#include "mainwindow.h"
#include "vulkanwindow.h"
#include <QApplication>
#include <QLabel>
#include <QLCDNumber>
#include <QPushButton>
#include <QGridLayout>
#include<QDateTime>
#include<QScreen>
#include<QtWidgets/QSlider>
#include<QFileDialog>
#include <QtWidgets/QTreeWidget>

MainWindow::MainWindow(VulkanWindow* vwindow) :vulkanwindow(vwindow) {
	QWidget* wrapper = QWidget::createWindowContainer(vulkanwindow);
	wrapper->setFocusPolicy(Qt::StrongFocus);
	wrapper->setFocus();
	//	wrapper->setGeometry(QRect(0, 0, 1920, 1080));

	FPSLcd = new QLCDNumber(2);
	FPSLcd->setSegmentStyle(QLCDNumber::Filled);
	FPSLcd->setSegmentStyle(QLCDNumber::Flat);
	FPSLcd->setStyleSheet("border: 1px solid green; color: green; background: silver;");
	FPSLcd->display(0);


	speedSlider = new QSlider();
	speedSlider->setFocusPolicy(Qt::NoFocus);
	speedSlider->setMinimum(0);
	speedSlider->setMaximum(100);
	speedSlider->setSingleStep(5);
speedSlider->setTickPosition(QSlider::TicksBothSides); 
	speedSlider->setValue(1);

	grabButton = new QPushButton(tr("&Screenshot"));
	grabButton->setFocusPolicy(Qt::NoFocus);

	quitButton = new QPushButton(tr("&Quit"));
	quitButton->setFocusPolicy(Qt::NoFocus);
	pauseButton = new QPushButton(tr("&Pause"));
	pauseButton->setFocusPolicy(Qt::NoFocus);


	treeWidget = new QTreeWidget();
	new QTreeWidgetItem(treeWidget);
	new QTreeWidgetItem(treeWidget);
	new QTreeWidgetItem(treeWidget);
	QTreeWidgetItem* __qtreewidgetitem = new QTreeWidgetItem(treeWidget);
	new QTreeWidgetItem(__qtreewidgetitem);

	QTreeWidgetItem* ___qtreewidgetitem = treeWidget->headerItem();
	___qtreewidgetitem->setText(0, QApplication::translate("Form", "\345\244\252\351\230\263\347\263\273", nullptr));

	const bool __sortingEnabled = treeWidget->isSortingEnabled();
	treeWidget->setSortingEnabled(false);
	QTreeWidgetItem* ___qtreewidgetitem1 = treeWidget->topLevelItem(0);
	___qtreewidgetitem1->setText(0, QApplication::translate("Form", "\345\244\252\351\230\263", nullptr));
	QTreeWidgetItem* ___qtreewidgetitem2 = treeWidget->topLevelItem(1);
	___qtreewidgetitem2->setText(0, QApplication::translate("Form", "\346\260\264\346\230\237", nullptr));
	QTreeWidgetItem* ___qtreewidgetitem3 = treeWidget->topLevelItem(2);
	___qtreewidgetitem3->setText(0, QApplication::translate("Form", "\351\207\221\346\230\237", nullptr));
	QTreeWidgetItem* ___qtreewidgetitem4 = treeWidget->topLevelItem(3);
	___qtreewidgetitem4->setText(0, QApplication::translate("Form", "\345\234\260\347\220\203", nullptr));
	QTreeWidgetItem* ___qtreewidgetitem5 = ___qtreewidgetitem4->child(0);
	___qtreewidgetitem5->setText(0, QApplication::translate("Form", "\346\234\210\347\220\203", nullptr));
	treeWidget->setSortingEnabled(__sortingEnabled);


	QGridLayout* layout = new QGridLayout;
	//	layout->addWidget(treeWidget, 1, 3);
	layout->addWidget(FPSLcd, 1, 3);
	layout->addWidget(speedSlider, 3, 3);
	layout->addWidget(grabButton, 4, 3);
	layout->addWidget(pauseButton, 5, 3);
	layout->addWidget(quitButton, 6, 3);
	layout->addWidget(wrapper, 0, 0, 7, 3);
	setLayout(layout);

	connect(vulkanwindow, &VulkanWindow::updateFPSLcd, FPSLcd, [=] {FPSLcd->display(vulkanwindow->getFps()); });
	connect(speedSlider, &QSlider::valueChanged, this, &MainWindow::changeSpeed);
	connect(grabButton, &QPushButton::clicked, this, &MainWindow::onGrabRequested);
	connect(quitButton, &QPushButton::clicked, qApp, &QCoreApplication::quit);
	connect(pauseButton, &QPushButton::clicked, vulkanwindow, &VulkanWindow::togglePaused);

}

void MainWindow::closeEvent(QCloseEvent* e) {
	qApp->quit();
	//	delete vulkanwindow;
}

MainWindow::~MainWindow()
{
}

void MainWindow::onGrabRequested()
{
	QScreen* screen = vulkanwindow->screen();

	QFileDialog fd(this);
	fd.setAcceptMode(QFileDialog::AcceptSave);
	fd.setDefaultSuffix(".jpg");

	QString filePathName = "Screenshot-";
	filePathName += QDateTime::currentDateTime().toString("yyyy-MM-dd hh-mm-ss-zzz");
	filePathName += ".jpg";
	fd.selectFile(filePathName);

	if (fd.exec() == QDialog::Accepted) {
		screen->grabWindow(vulkanwindow->winId()).save(fd.selectedFiles().first());
	}
}

void MainWindow::changeSpeed(int value) {
	vulkanwindow->m_speed = speedSlider->value();
}
