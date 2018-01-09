#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <player.h>
namespace Ui {
class MainWindow;
}
class MainWindow : public QMainWindow
{
    Q_OBJECT

signals:
    void sendCheckboxStatus(const bool chk_segmentation, const bool chk_stabilization);

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    //Display video frame in player UI
    void updatePlayerUI(QImage img, QImage img_result);
    void on_btn_Play_clicked();
    void on_actionLoad_Video_triggered();

private:
    Ui::MainWindow *ui;
    Player* myPlayer;
    QString filename;
    bool chk_segmentation = false, chk_stabilization = false; //checkbox statuses to be sent to 'player' thread for processing
    void playVideo();
};
#endif // MAINWINDOW_H
