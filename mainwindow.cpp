#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    myPlayer = new Player(this);
    QObject::connect(myPlayer, SIGNAL(processedImage(QImage,QImage)),
                                  this, SLOT(updatePlayerUI(QImage,QImage)));
    ui->setupUi(this);
}

void MainWindow::updatePlayerUI(QImage img, QImage img_result)
{
    if (!img.isNull())
    {
        ui->lbl_original->setAlignment(Qt::AlignCenter);
        ui->lbl_original->setPixmap(QPixmap::fromImage(img).scaled(ui->lbl_original->size(),Qt::KeepAspectRatio, Qt::FastTransformation));
        ui->lbl_result->setAlignment(Qt::AlignCenter);
        ui->lbl_result->setPixmap(QPixmap::fromImage(img_result).scaled(ui->lbl_result->size(),Qt::KeepAspectRatio, Qt::FastTransformation));
        emit sendCheckboxStatus(ui->chk_segmentation->isChecked(), ui->chk_stabilization->isChecked());
    }
}

MainWindow::~MainWindow()
{
    delete myPlayer;
    delete ui;
}

void MainWindow::playVideo() {
    if (filename.isEmpty()) {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Error");
        msgBox.setText("No video loaded");
        msgBox.exec();
        return;
    }
    if (myPlayer->isStopped())
    {
        myPlayer->Play();
        ui->btn_Play->setText(tr("Stop"));
    }else
    {
        myPlayer->Stop();
        ui->btn_Play->setText(tr("Play"));
    }
}

void MainWindow::on_btn_Play_clicked()
{
    playVideo();
}

void MainWindow::on_actionLoad_Video_triggered()
{
    filename = QFileDialog::getOpenFileName(this,
                                          tr("Open Video"), ".",
                                          tr("Video Files (*.avi *.mpg *.mp4)"));
    if (!filename.isEmpty()){
        if (!myPlayer->loadVideo(filename.toLatin1().data()))
        {
            QMessageBox msgBox;
            msgBox.setText("The selected video could not be opened!");
            msgBox.exec();
        }
        this->setWindowTitle(filename);
    }

}
