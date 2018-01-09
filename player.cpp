#include "player.h"

Player::Player(QObject *parent)
 : QThread(parent)
{
    stop = true;
    QObject::connect(parent, SIGNAL(sendCheckboxStatus(bool,bool)),this, SLOT(receiveCheckboxStatus(bool,bool)));
}

bool Player::loadVideo(String filename) {
    capture.open(filename);
    if (capture.isOpened())
    {
        frameRate = (int) capture.get(CV_CAP_PROP_FPS);
        return true;
    }
    else
        return false;
}

void Player::Play()
{
    if (!isRunning()) {
        if (isStopped()){
            stop = false;
        }
        start(LowPriority);
    }
}

void Player::run()
{
    int delay = (1000/frameRate);
    while(!stop){
        if (!capture.read(frame))
        {
            stop = true;
        }

        frame_processed = frame;
        frame_processed = processImage(frame_processed);

        if (frame.channels()== 3){
            cv::cvtColor(frame, RGBframe, CV_BGR2RGB);
            cv::cvtColor(frame, RGBframe_processed, CV_BGR2RGB);
            img = QImage((const unsigned char*)(RGBframe.data),RGBframe.cols,RGBframe.rows,QImage::Format_RGB888);
        }
        else
        {
            img = QImage((const unsigned char*)(frame.data), frame.cols,frame.rows,QImage::Format_Indexed8);
        }

        if (frame_processed.channels()== 3){
            cv::cvtColor(frame_processed, RGBframe_processed, CV_BGR2RGB);
            img_result = QImage((const unsigned char*)(RGBframe_processed.data),RGBframe_processed.cols,RGBframe_processed.rows,QImage::Format_RGB888);
        }
        else
        {
            img_result = QImage((const unsigned char*)(frame_processed.data), frame_processed.cols,frame_processed.rows,QImage::Format_Indexed8);
        }

        emit processedImage(img, img_result);
        this->msleep(delay);
    }
}

Player::~Player()
{
    mutex.lock();
    stop = true;
    capture.release();
    condition.wakeOne();
    mutex.unlock();
    wait();
}
void Player::Stop()
{
    stop = true;
}
void Player::msleep(int ms){
    struct timespec ts = { ms / 1000, (ms % 1000) * 1000 * 1000 };
    nanosleep(&ts, NULL);
}

void Player::receiveCheckboxStatus(bool chk_segmentation, bool chk_stabilization)
{
    this->chk_segmentation = chk_segmentation;
    this->chk_stabilization = chk_stabilization;
}

bool Player::isStopped() const{
    return this->stop;
}

Mat Player::processImage(Mat& frame) {
    Mat output = frame;
    if (chk_segmentation) {
        inRange(frame, cv::Scalar(0, 125, 0), cv::Scalar(255, 200, 255), output);
    }
    if (chk_stabilization) {

    }
    return output;
}
