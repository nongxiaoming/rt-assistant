#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDialog>
#include <QSerialPort>
#include <QTimer>
#include <QToolButton>
#include <QtNetwork>

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QDialog
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    bool eventFilter(QObject *obj, QEvent *event);


private slots:

    void on_btnMenu_Max_clicked();

    void on_loadFW_Button_clicked();

    void on_writeFW_Button_clicked();

    void timeout();

    void btnClick1();

    void changeStyle(int index);

    void changeLanguage();

     void update_log_output(QString info);

     void progressBar_update(int val);

     void on_config_default_pushButton_clicked();

private:
    Ui::MainWindow *ui;

    void InitStyle();   //初始化无边框窗体

    bool max;           //是否处于最大化状态

    QRect location;     //鼠标移动窗体后的坐标位置

    void Translate(int index);

    void SaveConfig();

    void LoadConfig();

    QSettings *Config;

    QTimer *timer;

    quint32 fw_crc;


    quint8 check_ver;

    bool connected;

    QList<int> pixChar;

    QList<QToolButton *> btns1;


    int skin_index;
    int language_index;
};

#endif // MAINWINDOW_H
