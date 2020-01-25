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
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    //void on_inputPushButton_pressed();

    //void on_outputPushButton_pressed();

    void on_Open_triggered();

    void on_histogramRadioButton_pressed();

    void on_Save_triggered();

private:
    Ui::MainWindow *ui;
    void iniUI();
    cv::Mat inputImage;
    cv::Mat outputImage;
    void displayImage(QLabel *outputLabel, const cv::Mat& image);
    void displayImageAndLabel(QLabel* outputLabel,QLabel* outputExplainLabel,const cv::Mat& image,const QString &text);
    cv::Mat openImage(cv::Mat& image);
    QTextCodec* codec = QTextCodec::codecForName("GBK");

    //void loadSettings();
    //void saveSettings();


protected:
    //void closeEvent(QCloseEvent* event);
};
#endif // MAINWINDOW_H
