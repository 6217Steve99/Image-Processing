#include "QOpencvView.h"


QOpencvView::QOpencvView(QWidget *parent) :
QOpenGLWidget(parent)
{
    mBgColor = QColor::fromRgb(100, 100, 100);
}

void QOpencvView::initializeGL()
{
    makeCurrent();
    initializeOpenGLFunctions();

    float r = ((float)mBgColor.darker().red())/255.0f;
    float g = ((float)mBgColor.darker().green())/255.0f;
    float b = ((float)mBgColor.darker().blue())/255.0f;
    glClearColor(r,g,b,1.0f);
}

void QOpencvView::resizeGL(int width, int height)
{
    makeCurrent();
    glViewport(0, 0, (GLint)width, (GLint)height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(0, width, -height, 0, 0, 1);

    glMatrixMode(GL_MODELVIEW);

    recalculatePosition();

    emit imageSizeChanged(mRenderWidth, mRenderHeight);

    updateScene();
}

void QOpencvView::updateScene()
{
    if (this->isVisible()) update();
}

void QOpencvView::paintGL()
{
    makeCurrent();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderImage();
}

void QOpencvView::renderImage()
{

	drawMutex.lock();
    makeCurrent();

    glClear(GL_COLOR_BUFFER_BIT);

    if (!mRenderQtImg.isNull())
    {
        glLoadIdentity();

        glPushMatrix();
        {
            if (mResizedImg.width() <= 0)
            {
                if (mRenderWidth == mRenderQtImg.width() && mRenderHeight == mRenderQtImg.height())
                    mResizedImg = mRenderQtImg;
                else
                    mResizedImg = mRenderQtImg.scaled(QSize(mRenderWidth, mRenderHeight),
                                                      Qt::IgnoreAspectRatio,
                                                      Qt::SmoothTransformation);
            }

            // ---> Centering image in draw area

            glRasterPos2i(mRenderPosX, mRenderPosY);

            glPixelZoom(1, -1);
			
            glDrawPixels(mResizedImg.width(), mResizedImg.height(), GL_RGBA, GL_UNSIGNED_BYTE, mResizedImg.bits());
			
        }
        glPopMatrix();

        // end
        glFlush();
    }

	drawMutex.unlock();
}

void QOpencvView::recalculatePosition()
{
    mImgRatio = (float)mOrigImage.cols/(float)mOrigImage.rows;

    mRenderWidth = this->size().width();
    mRenderHeight = floor(mRenderWidth / mImgRatio);

    if (mRenderHeight > this->size().height())
    {
        mRenderHeight = this->size().height();
        mRenderWidth = floor(mRenderHeight * mImgRatio);
    }

    mRenderPosX = floor((this->size().width() - mRenderWidth) / 2);
    mRenderPosY = -floor((this->size().height() - mRenderHeight) / 2);

    mResizedImg = QImage();
}

bool QOpencvView::showImage(const cv::Mat& image)
{
	drawMutex.lock();
    if (image.channels() == 3)
        cvtColor(image, mOrigImage, CV_BGR2RGBA);
    else if (image.channels() == 1)
        cvtColor(image, mOrigImage, CV_GRAY2RGBA);
	else if (image.channels() == 4)
		mOrigImage = image;
    else return false;

    mRenderQtImg = QImage((const unsigned char*)(mOrigImage.data),
                          mOrigImage.cols, mOrigImage.rows,
                          mOrigImage.step1(), QImage::Format_RGB32);
	
    recalculatePosition();

    updateScene();
	drawMutex.unlock();
    return true;
}
