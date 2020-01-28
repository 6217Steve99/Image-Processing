﻿#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ImageProcessing.h"

void MainWindow::iniUI()
{
    setCentralWidget(ui->tabWidget);
    QVBoxLayout* pageVLayout = new QVBoxLayout;
    pageVLayout->addWidget(ui->grayscalePushButton);
    pageVLayout->addWidget(ui->radioButton);
    ui->page->setLayout(pageVLayout);
}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    iniUI();
    //loadSettings();
}

MainWindow::~MainWindow()
{
    delete ui;
}





//void MainWindow::on_inputPushButton_pressed()
//{
//    QString fileName = QFileDialog::getOpenFileName(this,
//        "Open Input Image",
//        QDir::currentPath(),
//        "Images (*.jpg *.png *.bmp)");
//    if (QFile::exists(fileName))
//    {
//        ui->inputLineEdit->setText(fileName);
//    }
//}

//void MainWindow::on_outputPushButton_pressed()
//{
//    QString fileName = QFileDialog::getOpenFileName(this,
//        "Select Output Image",
//        QDir::currentPath(),
//        "*.jpg;;*.png;;*.bmp");
//    if (!fileName.isEmpty())
//    {
//        ui->outputLineEdit->setText(fileName);
//        cv::Mat inImg, outImg;
//        inImg = cv::imread(ui->inputLineEdit->text().toStdString());
//        cv::imshow("image",inImg);
//    }
//}

//void MainWindow::saveSettings()
//{
//    QSettings settings("Packt",
//        "Image Processing",
//        this);
//    settings.setValue("inputLineEdit",
//        ui->inputLineEdit->text()
//    );
//    settings.setValue("outputLineEdit",
//        ui->outputLineEdit->text()
//    );
//}

//void MainWindow::loadSettings()
//{
//    QSettings settings("Packt",
//        "Image Processing",
//        this);
//    ui->inputLineEdit->setText(settings.value("inputLineEdit",
//        "").toString());
//    ui->outputLineEdit->setText(settings.value("outputLineEdit",
//        "").toString());
//}

//void MainWindow::closeEvent(QCloseEvent *event)
//{
//    int result = QMessageBox::warning(this,
//        "Exit",
//        "Are you sure you want to close this program?",
//        QMessageBox::Yes,
//        QMessageBox::No);
//    if (result == QMessageBox::Yes)
//    {
//        saveSettings();
//        event->accept();
//    }
//    else
//    {
//        event->ignore();
//    }
//}

void MainWindow::displayImage(QLabel *outputLabel, const cv::Mat& image)
{
    if (image.empty())return;
    outputLabel->setFixedWidth(image.cols);
    outputLabel->setFixedHeight(image.rows);
    QImage qimage(image.data,
        image.cols,
        image.rows,
        image.step,
        QImage::Format_RGB888);
    QSize picSize(outputLabel->width(),outputLabel->height());
    QPixmap outputQImage = QPixmap::fromImage(qimage.rgbSwapped()).scaled(picSize, Qt::KeepAspectRatio);
    outputLabel->setPixmap(outputQImage);
}

void MainWindow::displayImageAndLabel(QLabel* outputLabel, QLabel* outputExplainLabel, const cv::Mat& image, const QString &text)
{
    displayImage(outputLabel, image);
    outputExplainLabel->setText(text);
}

cv::Mat MainWindow::openImage()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        "Select the Image",
        QDir::currentPath(),
        "*.jpg;;*.png;;*.bmp");
    if (!fileName.isEmpty())
    {
        cv::Mat image = cv::imread(fileName.toStdString());
        return image;
    }
    else 
    {
        return cv::Mat();
    }
}

void MainWindow::on_Open_triggered()
{
    inputImage = openImage();
    if (!inputImage.empty())
    {
        displayImageAndLabel(ui->singleOriginLabel, ui->singleOriginExplainLabel, 
            inputImage, codec->toUnicode("原图"));
    }
}

void MainWindow::on_histogramRadioButton_pressed()
{
    if (ui->Iterative->isChecked())
    {
        if (outputImage.empty())
        {
            currentImage = inputImage;
        }
        else
        {
            currentImage = outputImage;
        }
        
    }
    else
    {
        currentImage = inputImage;
    }
    if (ui->grayscaleCheckBox->isChecked())
    {
        cv::cvtColor(currentImage, currentImage, CV_BGR2GRAY);
    }
    if (currentImage.empty())
    {
        int result = QMessageBox::warning(this,
                    "Warning",
                    codec->toUnicode("您还未打开图片，是否现在打开需要处理的图片?"),
                    QMessageBox::Yes,
                    QMessageBox::No);
        if (result == QMessageBox::Yes)
        {
            inputImage = openImage();
            currentImage = inputImage;
            if (!currentImage.empty())
            {
                displayImageAndLabel(ui->singleOriginLabel, ui->singleOriginExplainLabel, 
                    inputImage, codec->toUnicode("原图"));
                outputImage = getImageOfHistogram(currentImage);
                displayImageAndLabel(ui->singleOutputLabel, ui->singleOutputExplainLabel, 
                    outputImage, codec->toUnicode("直方图"));
            }
            ui->histogramRadioButton->setChecked(true);
        }
        
    }
    else
    {
        outputImage = getImageOfHistogram(currentImage);
        displayImageAndLabel(ui->singleOutputLabel, ui->singleOutputExplainLabel,
            outputImage, codec->toUnicode("直方图"));
    }
}

void MainWindow::on_Save_triggered()
{
    if (outputImage.empty())
    {
        QMessageBox::warning(this,
            "Warning",
            codec->toUnicode("您还处理过图片，请先处理图片"),
            QMessageBox::Yes,
            QMessageBox::No);
    }
    else
    {
        QString fileName = QFileDialog::getSaveFileName(this,
            "Save the Image",
            QDir::currentPath(),
            "*.jpg;;*.png;;*.bmp");
        cv::imwrite(fileName.toStdString(), outputImage);
    }
}
void MainWindow::on_edrodeRadioButton_pressed()
{
    if (ui->Iterative->isChecked())
    {
        if (outputImage.empty())
        {
            currentImage = inputImage;
        }
        else
        {
            currentImage = outputImage;
        }

    }
    else
    {
        currentImage = inputImage;
    }
    if (ui->grayscaleCheckBox->isChecked())
    {
        cv::cvtColor(currentImage, currentImage, CV_BGR2GRAY);
    }
    if (currentImage.empty())
    {
        int result = QMessageBox::warning(this,
            "Warning",
            codec->toUnicode("您还未打开图片，是否现在打开需要处理的图片?"),
            QMessageBox::Yes,
            QMessageBox::No);
        if (result == QMessageBox::Yes)
        {
            inputImage = openImage();
            currentImage = inputImage;
            if (!currentImage.empty())
            {
                displayImageAndLabel(ui->singleOriginLabel, ui->singleOriginExplainLabel,
                    inputImage, codec->toUnicode("原图"));
                cv::erode(currentImage, outputImage, cv::Mat());
                displayImageAndLabel(ui->singleOutputLabel, ui->singleOutputExplainLabel,
                    outputImage, codec->toUnicode("腐蚀图像"));
            }
            ui->edrodeRadioButton->setChecked(true);
        }
    }
    else
    {
        cv::erode(currentImage, outputImage, cv::Mat());
        displayImageAndLabel(ui->singleOutputLabel, ui->singleOutputExplainLabel,
            outputImage, codec->toUnicode("腐蚀图像"));
    }
}

void MainWindow::on_dilateRadioButton_pressed()
{
    if (ui->Iterative->isChecked())
    {
        if (outputImage.empty())
        {
            currentImage = inputImage;
        }
        else
        {
            currentImage = outputImage;
        }

    }
    else
    {
        currentImage = inputImage;
    }
    if (ui->grayscaleCheckBox->isChecked())
    {
        cv::cvtColor(currentImage, currentImage, CV_BGR2GRAY);
    }
    if (currentImage.empty())
    {
        int result = QMessageBox::warning(this,
            "Warning",
            codec->toUnicode("您还未打开图片，是否现在打开需要处理的图片?"),
            QMessageBox::Yes,
            QMessageBox::No);
        if (result == QMessageBox::Yes)
        {
            inputImage = openImage();
            currentImage = inputImage;
            if (!currentImage.empty())
            {
                displayImageAndLabel(ui->singleOriginLabel, ui->singleOriginExplainLabel,
                    inputImage, codec->toUnicode("原图"));
                cv::dilate(currentImage, outputImage, cv::Mat());
                displayImageAndLabel(ui->singleOutputLabel, ui->singleOutputExplainLabel,
                    outputImage, codec->toUnicode("膨胀图像"));
            }
            ui->dilateRadioButton->setChecked(true);
        }

    }
    else
    {
        cv::dilate(currentImage, outputImage, cv::Mat());
        displayImageAndLabel(ui->singleOutputLabel, ui->singleOutputExplainLabel,
            outputImage, codec->toUnicode("膨胀图像"));
    }
}

void MainWindow::on_restorePushButton_pressed()
{
    currentImage = inputImage;
    outputImage = cv::Mat();
    displayImageAndLabel(ui->singleOutputLabel, ui->singleOutputExplainLabel,
        inputImage, codec->toUnicode("原图"));
    ui->histogramRadioButton->setCheckable(false);
    ui->histogramRadioButton->setCheckable(true);
    ui->edrodeRadioButton->setCheckable(false);
    ui->edrodeRadioButton->setCheckable(true);
    ui->dilateRadioButton->setCheckable(false);
    ui->dilateRadioButton->setCheckable(true);
}

void MainWindow::on_colorInversionRadioButton_pressed()
{

}

void MainWindow::on_loseColorRadioButton_pressed()
{
    if (ui->Iterative->isChecked())
    {
        if (outputImage.empty())
        {
            currentImage = inputImage;
        }
        else
        {
            currentImage = outputImage;
        }

    }
    else
    {
        currentImage = inputImage;
    }
    if (currentImage.empty())
    {
        int result = QMessageBox::warning(this,
            "Warning",
            codec->toUnicode("您还未打开图片，是否现在打开需要处理的图片?"),
            QMessageBox::Yes,
            QMessageBox::No);
        if (result == QMessageBox::Yes)
        {
            inputImage = openImage();
            currentImage = inputImage;
            if (!currentImage.empty())
            {
                displayImageAndLabel(ui->singleOriginLabel, ui->singleOriginExplainLabel,
                    inputImage, codec->toUnicode("原图"));
                colorReduce(currentImage, outputImage);
                displayImageAndLabel(ui->singleOutputLabel, ui->singleOutputExplainLabel,
                    outputImage, codec->toUnicode("减色图像"));
            }
            ui->loseColorRadioButton->setChecked(true);
        }

    }
    else
    {
        colorReduce(currentImage, outputImage);
        displayImageAndLabel(ui->singleOutputLabel, ui->singleOutputExplainLabel,
            outputImage, codec->toUnicode("减色图像"));
    }
}

void MainWindow::on_grayscaleCheckBox_stateChanged(int arg1)
{
    //if (ui->grayscaleCheckBox->isChecked())
    //{
    //    cv::cvtColor(currentImage, currentImage, CV_BGR2GRAY);
    //    cv::cvtColor(outputImage, outputImage, CV_BGR2GRAY);
    //    displayImage(ui->singleOutputLabel, outputImage);
    //    cv::cvtColor(srcImage, dstImage, cv::COLOR_BGR2GRAY);
    //    //cv::imshow("test",dstImage);
    //    img = QImage((const unsigned char*)(dstImage.data), dstImage.cols, dstImage.rows, dstImage.cols * dstImage.channels(),
    //        QImage::Format_Grayscale8);

    //}
}


