#include "searchfrm.h"
#include "ui_searchfrm.h"
#include "api/myhelper.h"
#include "Communication.h"


SearchForm *SearchForm::_instance = 0;

SearchForm::SearchForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SearchForm)
{
    ui->setupUi(this);
    this->InitStyle();
    myHelper::FormInCenter(this);
    QObject::connect(Communication::Instance(),&Communication::MsgReady,this,&MsgReady);
}

SearchForm::~SearchForm()
{
    delete ui;
}
void SearchForm::InitStyle()
{
    this->setProperty("form", true);
    this->setProperty("CanMove", true);
    this->setWindowTitle(ui->lab_Title->text());
    ui->widget_title->setProperty("form", "title");
    //设置窗体标题栏隐藏
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint);
    //安装事件监听器,让标题栏识别鼠标双击
    ui->lab_Title->installEventFilter(this);

    IconHelper::Instance()->SetIcon(ui->btnMenu_Close, QChar(0xf00d));
    IconHelper::Instance()->SetIcon(ui->btnMenu_Min, QChar(0xf068));
    IconHelper::Instance()->SetIcon(ui->lab_Ico, QChar(0xf015));

    connect(ui->btnMenu_Close, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->btnMenu_Min, SIGNAL(clicked()), this, SLOT(showMinimized()));

    m_list = new QStandardItemModel();
    //m_list->setHorizontalHeaderItem(0,new QStandardItem(tr("序号")));
     m_list->setHorizontalHeaderItem(0,new QStandardItem(tr("设备名称")));
    m_list->setHorizontalHeaderItem(1,new QStandardItem(tr("序列号")));
    m_list->setHorizontalHeaderItem(2,new QStandardItem(tr("IP地址")));
    m_list->setHorizontalHeaderItem(3,new QStandardItem(tr("MAC地址")));

    ui->tableView->setModel(m_list);
        //设置表格的各列的宽度值
       // ui->tableView->setColumnWidth(0,100);
        ui->tableView->setColumnWidth(0,150);
        ui->tableView->setColumnWidth(1,178);
        ui->tableView->setColumnWidth(2,110);
        ui->tableView->setColumnWidth(3,120);
     ui->tableView->horizontalHeader()->setStretchLastSection(true);
     ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    timer = new QTimer(this);
    timer->setInterval (1000);
    QObject::connect (timer,SIGNAL(timeout()),this,SLOT(timeout()));
    QObject::connect (ui->tableView,SIGNAL(clicked(QModelIndex)),this,SLOT(tableview_click(QModelIndex)));
    this->count=0;
    //this->select_device=NULL;
    this->barwait = new ProgressBarWait(this);
    this->barwait->setGeometry(this->geometry().x()+this->geometry().width()/2-50,this->geometry().y()+this->geometry().height()/2-50,100,100);
    this->barwait->hide();

}

bool SearchForm::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonDblClick) {
       // this->on_btnMenu_Max_clicked();
        return true;
    }
    if(event->type() == QEvent::Show)
    {
       on_search_pushButton_clicked();
    }
    return QObject::eventFilter(obj, event);
}
void SearchForm::tableview_click (QModelIndex index)
{
   select_device= this->deviceList[index.row ()];
}
void SearchForm::timeout()
{
    this->retry++;
    Msg_Base msg;
    msg.payload.clear();
    msg.cmd = CMD_DISCOVERY;

    Communication::Instance()->SendDataBroadcast(msg.toByteArray());
    if(this->retry >=3){
    this->timer->stop();
    if(this->barwait!=NULL)
    {
        this->barwait->hide();
    }
    if(this->count >0)
    {
    ui->tableView->selectRow (0);
    select_device= this->deviceList[0];
    }else
    {
        ui->status_label->setText (tr("未搜索到任何设备"));
    }
    }
}
void SearchForm::MsgReady(quint8 board_id,QHostAddress &ip,QByteArray &data)
{
   quint8  cmd = data[3];
   switch(cmd)
   {
    case CMD_DISCOVERY:
       {
       if(ip.toString().trimmed().isEmpty())
       {
           return;
       }
       Discovery_Msg msg;
       msg.fromByteArray(data);
       Device_t dev;
       dev.Name = msg.Name;
       dev.MAC = tr("%1:%2:%3:%4:%5:%6").arg(QString::number(msg.mac[0],16)).arg(QString::number(msg.mac[1],16)).arg(QString::number(msg.mac[2],16))
               .arg(QString::number(msg.mac[3],16)).arg(QString::number(msg.mac[4],16)).arg(QString::number(msg.mac[5],16));
       dev.SerialNum = msg.SN;
       qDebug()<<ip.toString();
       dev.IP = ip;
       dev.sw_ver = msg.sw_ver;
       dev.build_utc = msg.build_date;

       foreach(Device_t d,this->deviceList)
       {
          if((d.MAC==dev.MAC)&&d.SerialNum==dev.SerialNum)
          {
              if(d.IP!=dev.IP)
              {
                  d.IP = dev.IP;
              }
              return;
          }
       }
       this->deviceList.append(dev);

       m_list->setItem (this->count,0,new QStandardItem(deviceList[this->count].Name));
       m_list->setItem (this->count,1,new QStandardItem(deviceList[this->count].SerialNum));
       m_list->setItem (this->count,2,new QStandardItem(deviceList[this->count].IP.toString ()));
       m_list->setItem (this->count,3,new QStandardItem(deviceList[this->count].MAC));
       m_list->item (this->count,0)->setTextAlignment (Qt::AlignCenter);
       m_list->item (this->count,1)->setTextAlignment (Qt::AlignCenter);
       m_list->item (this->count,2)->setTextAlignment (Qt::AlignCenter);
       m_list->item (this->count,3)->setTextAlignment (Qt::AlignCenter);
        this->count++;
       if(this->count >0)
       {
       ui->status_label->setText (tr("搜索到%1台设备").arg(QString::number(this->count)));
       }
       }
       break;
   }

}

void SearchForm::on_search_pushButton_clicked()
{
    try{
    this->count = 0;
    m_list->removeRows (0,m_list->rowCount ());
    ui->status_label->setText (tr("正在搜索设备..."));
    this->deviceList.clear ();


    }catch(QString exception)
    {
        qDebug()<<"Exception:"<<exception<<"\n";
        return;
    }
   Msg_Base msg;
   msg.payload.clear();
   msg.cmd = CMD_DISCOVERY;

   Communication::Instance()->SendDataBroadcast(msg.toByteArray());
   if(this->barwait!=NULL)
   {
    this->barwait->show();
   }
   timer->start (500);
   this->retry = 0;

}

void SearchForm::on_connect_pushButton_clicked()
{
  this->accept();
}
