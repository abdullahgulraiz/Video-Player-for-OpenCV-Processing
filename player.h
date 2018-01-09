#ifndef PLAYER_H
#define PLAYER_H
#include <QMutex>
#include <QThread>
#include <QImage>
#include <QWaitCondition>
#include <QElapsedTimer>
#include <QMessageBox>
#include <QFile>
#include <QCoreApplication>
#include <QMap>
#include <QtDebug>
#include <string.h>
#include "opencv2/opencv.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace cv;
class Player : public QThread
{    Q_OBJECT
 private:
    bool stop;
    QMutex mutex;
    QWaitCondition condition;
    Mat frame, frame_processed;
    int frameRate;
    VideoCapture capture;
    Mat RGBframe, RGBframe_processed;
    QImage img, img_result;
    bool chk_segmentation = false, chk_stabilization = false;
    Mat processImage(Mat&);
 signals:
 //Signal to output frame to be displayed
      void processedImage(const QImage &image, const QImage &image_result);
 protected:
     void run();
     void msleep(int ms);
private slots:
      void receiveCheckboxStatus(bool chk_segmentation, bool chk_stabilization);
 public:
    //Constructor
    Player(QObject *parent = 0);
    //Destructor
    ~Player();
    //Load a video from memory
    bool loadVideo(String filename);
    //Play the video
    void Play();
    //Stop the video
    void Stop();
    //check if the player has been stopped
    bool isStopped() const;
};
#endif // VIDEOPLAYER_H
