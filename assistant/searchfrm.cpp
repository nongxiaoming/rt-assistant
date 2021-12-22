#include "searchfrm.h"
#include "ui_searchfrm.h"
#include "api/myhelper.h"


SearchForm *SearchForm::_instance = 0;

SearchForm::SearchForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SearchForm)
{
    ui->setupUi(this);
    this->InitStyle();
    myHelper::FormInCenter(this);
    //QObject::connect(Communication::Instance(),&Communication::MsgReady,this,&MsgReady);
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
