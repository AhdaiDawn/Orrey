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
#include <QtWidgets/QTextBrowser>

#define WIDTH 1500
#define HEIGHT 1000
#define FULLSCREEN false

MainWindow::MainWindow(VulkanWindow* vwindow) :vulkanwindow(vwindow)
{

	QGridLayout* gridLayout = new QGridLayout;

	QWidget* wrapper = QWidget::createWindowContainer(vulkanwindow);
	wrapper->setFocusPolicy(Qt::StrongFocus);
	wrapper->setFocus();
	QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	sizePolicy.setHorizontalStretch(1);
	sizePolicy.setVerticalStretch(1);
	sizePolicy.setHeightForWidth(wrapper->sizePolicy().hasHeightForWidth());
	wrapper->setSizePolicy(sizePolicy);
	gridLayout->addWidget(wrapper, 0, 0, 9, 1);

	FPSLcd = new QLCDNumber(3);
	FPSLcd->setSegmentStyle(QLCDNumber::Filled);
	FPSLcd->setSegmentStyle(QLCDNumber::Flat);
	FPSLcd->setStyleSheet("border: 1px solid green; color: green; background: silver;");
	FPSLcd->display(0);
	gridLayout->addWidget(FPSLcd, 3, 2, 1, 1);

	FPSLabel = new QLabel();
	gridLayout->addWidget(FPSLabel, 3, 1, 1, 1);

	speedSlider = new QSlider();
	speedSlider->setFocusPolicy(Qt::NoFocus);
	speedSlider->setMinimum(0);
	speedSlider->setMaximum(100);
	speedSlider->setSingleStep(5);
	speedSlider->setOrientation(Qt::Horizontal);
	speedSlider->setValue(1);
	gridLayout->addWidget(speedSlider, 2, 2, 1, 3);

	speedLabel = new QLabel();
	gridLayout->addWidget(speedLabel, 2, 1, 1, 1);

	grabButton = new QPushButton();
	grabButton->setFocusPolicy(Qt::NoFocus);
	gridLayout->addWidget(grabButton, 7, 3, 1, 2);

	quitButton = new QPushButton();
	quitButton->setFocusPolicy(Qt::NoFocus);
	gridLayout->addWidget(quitButton, 8, 3, 1, 2);

	pauseButton = new QPushButton();
	pauseButton->setFocusPolicy(Qt::NoFocus);
	gridLayout->addWidget(pauseButton, 8, 1, 1, 2);

	hideButton = new QPushButton();
	hideButton->setFocusPolicy(Qt::NoFocus);
	gridLayout->addWidget(hideButton, 7, 1, 1, 2);

	planetSizeLcd = new QLCDNumber(2);
	planetSizeLcd->setSegmentStyle(QLCDNumber::Filled);
	planetSizeLcd->setSegmentStyle(QLCDNumber::Flat);
	planetSizeLcd->setStyleSheet("border: 1px solid green; color: green; background: silver;");
	gridLayout->addWidget(planetSizeLcd, 4, 2, 1, 1);

	subSizeButton = new QPushButton();
	gridLayout->addWidget(subSizeButton, 6, 1, 1, 2);

	addSizeButton = new QPushButton();
	gridLayout->addWidget(addSizeButton, 5, 1, 1, 2);

	planetSizeLabel = new QLabel();
	gridLayout->addWidget(planetSizeLabel, 4, 1, 1, 1);

	astroidNumlabel = new QLabel();
	gridLayout->addWidget(astroidNumlabel, 3, 3, 1, 2);

	astroidNumLcd = new QLCDNumber(8);
	astroidNumLcd->setSegmentStyle(QLCDNumber::Filled);
	astroidNumLcd->setSegmentStyle(QLCDNumber::Flat);
	astroidNumLcd->setStyleSheet("border: 1px solid green; color: green; background: silver;");
	astroidNumLcd->display(0);

	gridLayout->addWidget(astroidNumLcd, 4, 3, 1, 2);

	addNumButton = new QPushButton();
	gridLayout->addWidget(addNumButton, 5, 3, 1, 2);

	subNumButton = new QPushButton();
	gridLayout->addWidget(subNumButton, 6, 3, 1, 2);

	textBrowser = new QTextBrowser();
	gridLayout->addWidget(textBrowser, 1, 1, 1, 4);

	treeWidget = new QTreeWidget();
	gridLayout->addWidget(treeWidget, 0, 1, 1, 4);

	setLayout(gridLayout);

	setupUI();

	connect(vulkanwindow, &VulkanWindow::updateFPSLcd, FPSLcd, [=] {
		if (abs(vulkanwindow->getFps() - lastFPS) > 10) {
			lastFPS = vulkanwindow->getFps();
			FPSLcd->display(vulkanwindow->getFps());
		}
		});

	connect(addSizeButton, &QPushButton::clicked, vulkanwindow, &VulkanWindow::addObjectSize);
	connect(subSizeButton, &QPushButton::clicked, vulkanwindow, &VulkanWindow::subObjectSize);
	connect(vulkanwindow, &VulkanWindow::updateObjectSizeLCD, planetSizeLcd, [=] {planetSizeLcd->display(vulkanwindow->getObjectSize()); });

	connect(addNumButton, &QPushButton::clicked, vulkanwindow, &VulkanWindow::addAstroidObjectNum);
	connect(subNumButton, &QPushButton::clicked, vulkanwindow, &VulkanWindow::subAstroidObjectNum);
	connect(vulkanwindow, &VulkanWindow::updateAstroidObjectNumLCD, astroidNumLcd, [=] {astroidNumLcd->display(vulkanwindow->getObjectNum()); });

	connect(speedSlider, &QSlider::valueChanged, this, &MainWindow::changeSpeed);
	connect(grabButton, &QPushButton::clicked, this, &MainWindow::onGrabRequested);
	connect(quitButton, &QPushButton::clicked, qApp, &QCoreApplication::quit);

	connect(pauseButton, &QPushButton::clicked, this, [=] {
		if (vulkanwindow->togglePaused())
			pauseButton->setText("Start");
		else
			pauseButton->setText("Pause");
		});

	connect(hideButton, &QPushButton::clicked, this, [=] {
		if (vulkanwindow->hideOrbits())
			hideButton->setText("ShowOrbit");
		else
			hideButton->setText("HideOrbit");
		});

	//	lastGeometry = QRect(0, 0, 2560, 1600);
	lastGeometry = QRect(0, 0, WIDTH, HEIGHT);
	this->setGeometry(lastGeometry);
}

void MainWindow::setupUI()
{
	QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
	sizePolicy.setHorizontalStretch(0);
	sizePolicy.setVerticalStretch(0);
	sizePolicy.setHeightForWidth(FPSLcd->sizePolicy().hasHeightForWidth());
	FPSLcd->setSizePolicy(sizePolicy);

	sizePolicy.setHeightForWidth(planetSizeLcd->sizePolicy().hasHeightForWidth());
	planetSizeLcd->setSizePolicy(sizePolicy);

	sizePolicy.setHeightForWidth(astroidNumlabel->sizePolicy().hasHeightForWidth());
	astroidNumlabel->setSizePolicy(sizePolicy);

	sizePolicy.setHeightForWidth(astroidNumLcd->sizePolicy().hasHeightForWidth());
	astroidNumLcd->setSizePolicy(sizePolicy);

	sizePolicy.setHeightForWidth(speedLabel->sizePolicy().hasHeightForWidth());
	speedLabel->setSizePolicy(sizePolicy);

	QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Minimum);
	sizePolicy2.setHorizontalStretch(0);
	sizePolicy2.setVerticalStretch(0);
	sizePolicy2.setHeightForWidth(speedSlider->sizePolicy().hasHeightForWidth());
	speedSlider->setSizePolicy(sizePolicy2);

	FPSLabel->setText(QCoreApplication::translate("Form", "FPS\357\274\232", nullptr));
	addNumButton->setText(QCoreApplication::translate("Form", "AddNum", nullptr));
	subSizeButton->setText(QCoreApplication::translate("Form", "SubAU", nullptr));
	addSizeButton->setText(QCoreApplication::translate("Form", "AddAU", nullptr));
	hideButton->setText(QCoreApplication::translate("Form", "HideOrbit", nullptr));
	speedLabel->setText(QCoreApplication::translate("Form", "\351\200\237\345\272\246", nullptr));
	pauseButton->setText(QCoreApplication::translate("Form", "Pause", nullptr));
	planetSizeLabel->setText(QCoreApplication::translate("Form", "\350\267\235\347\246\273\347\274\251\346\224\276\357\274\232", nullptr));
	subNumButton->setText(QCoreApplication::translate("Form", "SubNum", nullptr));
	grabButton->setText(QCoreApplication::translate("Form", "Screenshot", nullptr));
	quitButton->setText(QCoreApplication::translate("Form", "Quit", nullptr));
	astroidNumlabel->setText(QCoreApplication::translate("Form", "<html><head/><body><p align=\"center\">\345\260\217\350\241\214\346\230\237\346\225\260\351\207\217</p></body></html>", nullptr));

	QFont font;
	font.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
	font.setPointSize(10);
	font.setBold(true);
	font.setWeight(75);
	QTreeWidgetItem* __qtreewidgetitem = new QTreeWidgetItem();
	__qtreewidgetitem->setFont(0, font);
	treeWidget->setHeaderItem(__qtreewidgetitem);
	new QTreeWidgetItem(treeWidget);
	new QTreeWidgetItem(treeWidget);
	new QTreeWidgetItem(treeWidget);
	QTreeWidgetItem* __qtreewidgetitem1 = new QTreeWidgetItem(treeWidget);
	new QTreeWidgetItem(__qtreewidgetitem1);
	new QTreeWidgetItem(treeWidget);
	new QTreeWidgetItem(treeWidget);
	new QTreeWidgetItem(treeWidget);
	new QTreeWidgetItem(treeWidget);
	new QTreeWidgetItem(treeWidget);

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
	QTreeWidgetItem* ___qtreewidgetitem6 = treeWidget->topLevelItem(4);
	___qtreewidgetitem6->setText(0, QApplication::translate("Form", "\347\201\253\346\230\237", nullptr));
	QTreeWidgetItem* ___qtreewidgetitem7 = treeWidget->topLevelItem(5);
	___qtreewidgetitem7->setText(0, QApplication::translate("Form", "\346\234\250\346\230\237", nullptr));
	QTreeWidgetItem* ___qtreewidgetitem8 = treeWidget->topLevelItem(6);
	___qtreewidgetitem8->setText(0, QApplication::translate("Form", "\345\234\237\346\230\237", nullptr));
	QTreeWidgetItem* ___qtreewidgetitem9 = treeWidget->topLevelItem(7);
	___qtreewidgetitem9->setText(0, QApplication::translate("Form", "\345\244\251\347\216\213\346\230\237", nullptr));
	QTreeWidgetItem* ___qtreewidgetitem10 = treeWidget->topLevelItem(8);
	___qtreewidgetitem10->setText(0, QApplication::translate("Form", "\346\265\267\347\216\213\346\230\237", nullptr));
	treeWidget->setSortingEnabled(__sortingEnabled);

	textBrowser->setHtml(QApplication::translate("Form", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
		"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
		"p, li { white-space: pre-wrap; }\n"
		"</style></head><body style=\" font-family:'SimSun'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
		"<p style=\" margin-top:0px; margin-bottom:15px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:28px; line-height:24px; background-color:#ffffff;\"><span style=\" font-family:'arial,\345\256\213\344\275\223,sans-serif'; font-size:14px; color:#333333; background-color:#ffffff;\">\345\244\252\351\230\263\347\263\273\357\274\210Solar System\357\274\211\357\274\214\346\230\257\350\264\250\351\207\217\345\276\210\345\244\247\347\232\204\345\244\252\351\230\263\357\274\214\344\273\245\345\205\266\345\267\250\345\244\247\347\232\204\345\274\225\345\212\233\347\273\264\346\214\201\347\235\200\345\221\250\350\276\271</span><a href=\"https://baike.baidu.com/item/%E8%A1%8C%E6"
		"%98%9F/15991\"><span style=\" font-family:'arial,\345\256\213\344\275\223,sans-serif'; font-size:14px; text-decoration: underline; color:#136ec2;\">\350\241\214\346\230\237</span></a><span style=\" font-family:'arial,\345\256\213\344\275\223,sans-serif'; font-size:14px; color:#333333;\">\343\200\201</span><a href=\"https://baike.baidu.com/item/%E5%8D%AB%E6%98%9F/21511\"><span style=\" font-family:'arial,\345\256\213\344\275\223,sans-serif'; font-size:14px; text-decoration: underline; color:#136ec2;\">\345\215\253\346\230\237</span></a><span style=\" font-family:'arial,\345\256\213\344\275\223,sans-serif'; font-size:14px; color:#333333;\">\343\200\201</span><a href=\"https://baike.baidu.com/item/%E5%B0%8F%E8%A1%8C%E6%98%9F/68902\"><span style=\" font-family:'arial,\345\256\213\344\275\223,sans-serif'; font-size:14px; text-decoration: underline; color:#136ec2;\">\345\260\217\350\241\214\346\230\237</span></a><span style=\" font-family:'arial,\345\256\213\344\275\223,sans-serif'; font-size:14px; color:#333333;\">"
		"\345\222\214</span><a href=\"https://baike.baidu.com/item/%E5%BD%97%E6%98%9F/22704\"><span style=\" font-family:'arial,\345\256\213\344\275\223,sans-serif'; font-size:14px; text-decoration: underline; color:#136ec2;\">\345\275\227\346\230\237</span></a><span style=\" font-family:'arial,\345\256\213\344\275\223,sans-serif'; font-size:14px; color:#333333;\">\347\273\225\345\205\266\350\277\220\350\275\254\347\232\204</span><a href=\"https://baike.baidu.com/item/%E5%A4%A9%E4%BD%93%E7%B3%BB%E7%BB%9F/8891801\"><span style=\" font-family:'arial,\345\256\213\344\275\223,sans-serif'; font-size:14px; text-decoration: underline; color:#136ec2;\">\345\244\251\344\275\223\347\263\273\347\273\237</span></a><span style=\" font-family:'arial,\345\256\213\344\275\223,sans-serif'; font-size:14px; color:#333333;\">\343\200\202</span></p></body></html>", nullptr));

}

void MainWindow::closeEvent(QCloseEvent*)
{
	qApp->quit();
}

void MainWindow::resizeEvent(QResizeEvent*) {
	vulkanwindow->recreateSwapchain();
}

MainWindow::~MainWindow()
{
	delete vulkanwindow;
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

	if (fd.exec() == QDialog::Accepted)
	{
		screen->grabWindow(vulkanwindow->winId()).save(fd.selectedFiles().first());
	}
}

void MainWindow::changeSpeed(int)
{
	vulkanwindow->m_speed = speedSlider->value();
}

