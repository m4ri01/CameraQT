#include "widget.h"
#include "ui_widget.h"
#include <QCamera>
#include <QCameraViewfinder>
#include <QVBoxLayout>
#include <QCameraImageCapture>
#include <QMenu>;
#include <QAction>;
#include <QFileDialog>


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    mCamera = new QCamera(this);
    mCameraViewfinder = new QCameraViewfinder(this);
    mCameraImageCapture = new QCameraImageCapture(mCamera,this);
    mLayout = new QVBoxLayout;
    mOptionMenu = new QMenu("option",this);
    mStartAction = new QAction("Start",this);
    mStopAction = new QAction("Stop",this);
    mCaptureAction = new QAction("Capture",this);

    mOptionMenu->addActions({mStartAction,mStopAction,mCaptureAction});
    ui->optionpushButton->setMenu(mOptionMenu);
    mCamera->setViewfinder(mCameraViewfinder);
    mLayout->addWidget(mCameraViewfinder);
    mLayout->setMargin(0);
    ui->scrollArea->setLayout(mLayout);

    connect(mStartAction,&QAction::triggered,[&](){
    mCamera->start();
    });
    connect(mStopAction,&QAction::triggered,[&](){
    mCamera->stop();
    });
    connect(mCaptureAction,&QAction::triggered,[&](){
    auto filename =  QFileDialog::getSaveFileName(this,"capture","/","image(*.jpg;*.jpeg)");
    if(filename.isEmpty()){
        return;
    }
    mCameraImageCapture->setCaptureDestination(QCameraImageCapture::CaptureToFile);
    QImageEncoderSettings imageEncoderSettings;
    imageEncoderSettings.setCodec("image/jpeg");
    imageEncoderSettings.setResolution(1600,1200);
    mCameraImageCapture->setEncodingSettings(imageEncoderSettings);
    mCamera->setCaptureMode(QCamera::CaptureStillImage);
    mCamera->start();
    mCamera->searchAndLock();
    mCameraImageCapture->capture(filename);
    mCamera->unlock();
    });
}

Widget::~Widget()
{
    delete ui;
}
