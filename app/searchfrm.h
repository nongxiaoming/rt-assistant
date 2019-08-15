#ifndef SEARCH_FRM_H
#define SEARCH_FRM_H
#include <QtGui>
#include <QtCore>
#include <QDialog>
#include <QtNetwork>
#include <QTime>
#include "progressbarwait.h"
#include "typedefs.h"

namespace Ui {
class SearchForm;
}



class SearchForm : public QDialog
{
    Q_OBJECT

public:
    explicit SearchForm(QWidget *parent = 0);
    ~SearchForm();

    static SearchForm *Instance() {
        if (!_instance) {
          _instance = new SearchForm;
        }
        return _instance;
    }
    Device_t getSelectDevice()
      {
       return this->select_device;
      }

protected:
    bool eventFilter(QObject *obj, QEvent *event);

private slots:
    void MsgReady(quint8 board_id,QHostAddress &ip,QByteArray &data);

    void on_search_pushButton_clicked();

    void on_connect_pushButton_clicked();

    void timeout();

    void tableview_click (QModelIndex index);

private:
    Ui::SearchForm *ui;

    void InitStyle();   //初始化无边框窗体

    QList<Device_t> deviceList;

    QStandardItemModel *m_list;

    QTimer *timer;

    ProgressBarWait *barwait;

    quint32 count;

    static SearchForm *_instance;

    Device_t select_device;

    quint8 retry;
};

#endif // TESTDIALOG_H
