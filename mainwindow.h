﻿#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qfiledialog.h>
#include <qdir.h>
#include <qfile.h>
#include <qmessagebox.h>
#include <qevent.h>
#include <qsettings.h>
#include <opencv2/opencv.hpp>
#include <QTextCodec>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget* parent = nullptr);
	~MainWindow();


private slots:
	//void on_inputPushButton_pressed();

	//void on_outputPushButton_pressed();

	void on_Open_triggered();

	void on_histogramRadioButton_pressed();

	void on_Save_triggered();

	void on_edrodeRadioButton_pressed();

	void on_dilateRadioButton_pressed();

	void on_restorePushButton_pressed();

	void on_colorInversionRadioButton_pressed();

	void on_loseColorRadioButton_pressed();

	void on_GrayscaleRadioButton_pressed();

    void on_equalizeHistRadioButton_pressed();

    void on_adaptiveThresholdRadioButton_pressed();

    void on_openRadioButton_pressed();

    void on_closeRadioButton_pressed();

    void on_gradientRadioButton_pressed();

    void on_hapTopRadioButton_pressed();

    void on_blackHatRadioButton_pressed();

    void on_comboBox_currentIndexChanged(int index);

    void on_parameterspinBoxA1_valueChanged(int arg1);

    void on_parameterspinBoxA2_valueChanged(int arg1);

    void on_blurRadioButton_pressed();

    void on_gaussianBlurRadioButton_pressed();

    void on_medianBlurRadioButton_pressed();

private:
	Ui::MainWindow* ui;
	void iniUI();
	cv::Mat inputImage;
	cv::Mat outputImage;
	cv::Mat currentImage;
	cv::Mat inputGrayImage;
	cv::Mat openImage();
	QTextCodec* codec = QTextCodec::codecForName("GBK");

	bool MainWindow::checkImgandIterative();

	//void loadSettings();
	//void saveSettings();


protected:
	//void closeEvent(QCloseEvent* event);
};
#endif // MAINWINDOW_H
