#ifndef FRMMAIN_H
#define FRMMAIN_H

#include <QDialog>
#include <QSerialPort>
#include <QTimer>
#include <QToolButton>
#include <QtNetwork>

namespace Ui
{
    class frmMain;
}

class frmMain : public QDialog
{
    Q_OBJECT

public:
    explicit frmMain(QWidget *parent = 0);
    ~frmMain();


private slots:

    void on_loadFW_Button_clicked();

    void on_writeFW_Button_clicked();

    void on_close();

    void timeout();

    void btnClick1();

    void changeStyle();

    void changeLanguage();

     void showRightMenu();

     void update_log_output(QString info);

     void progressBar_update(int val);

     void on_config_default_pushButton_clicked();

private:
    Ui::frmMain *ui;

     QMenu *m_menu;

    void InitStyle();   //初始化无边框窗体

    QRect location;     //鼠标移动窗体后的坐标位置

    void changeStyle(int index);

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

    QList<QAction *> skin_action_list;

    QList<QAction *> language_action_list;

    QHostAddress DeviceIP;

    int skin_index;
    int language_index;
};

#endif // FRMMAIN_H
