#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "api/uihelper.h"
#include <QSerialPortInfo>

MainWindow::MainWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

     QString config_path = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    if(QFile::exists(config_path+"/settings.ini"))
    {
        Config = new QSettings(config_path+"/settings.ini", QSettings::IniFormat, this);
        Config->setIniCodec("UTF-8");
        LoadConfig();
    }else
    {
        Config = new QSettings(config_path+"/settings.ini", QSettings::IniFormat, this);
        Config->setIniCodec("UTF-8");
        this->language_index = 0;
        this->skin_index = 1;
        SaveConfig();
    }

    this->InitStyle();
    UIHelper::FormInCenter(this);

    ui->cfHexEdit->setReadOnly(true);
    this->connected = false;
    this->timer = new QTimer(this);



    QObject::connect(this->timer,SIGNAL(timeout()),this,SLOT(timeout()));

  this->check_ver = 0;

}

void MainWindow::timeout()
{

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::InitStyle()
{
    this->max = false;
    this->location = this->geometry();
    this->setProperty("Form", true);
    this->setProperty("CanMove", true);
    this->setWindowTitle(ui->lab_Title->text());
    //设置窗体标题栏隐藏
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint);
    //安装事件监听器,让标题栏识别鼠标双击
    ui->lab_Title->installEventFilter(this);

    IconHelper::Instance()->SetIcon(ui->btnMenu_Close, QChar(0xf00d));
    IconHelper::Instance()->SetIcon(ui->btnMenu_Max, QChar(0xf2d0));
    IconHelper::Instance()->SetIcon(ui->btnMenu_Min, QChar(0xf068));
    IconHelper::Instance()->SetIcon(ui->lab_Ico, QChar(0xf085));
    connect(ui->btnMenu_Close, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->btnMenu_Min, SIGNAL(clicked()), this, SLOT(showMinimized()));

    UIHelper::SetStyle(":/qss/dev.css");



    //准备导航的按钮集合
    //准备对应的图形字体集合
    //pixChar << 0xf0e4 << 0xf013 << 0xf1d8 << 0xf085  << 0xf072 << 0xf120 << 0xf021  << 0xf15c;
    //btns1 << ui->btn11 << ui->btn12 << ui->btn13 << ui->btn14 << ui->btn15 << ui->btn16 << ui->btn17<< ui->btn18;

    //设置对应的按钮的图标显示方案以及是否可以选中以及对应的单击事件
    int count = btns1.count();
    for (int i = 0; i < count; i++) {
        btns1.at(i)->setCheckable(true);
        btns1.at(i)->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        connect(btns1.at(i), SIGNAL(clicked(bool)), this, SLOT(btnClick1()));
    }

    IconHelper::Instance()->setStyle(ui->widget1, btns1, pixChar, 20, 40, 35, "left", 3,
                                     "#609EE9","#434D5C","#1D2530","#F5F7FA","#4B78AF");
    //默认选中哪个按钮
    //ui->btn11->click();

    //添加换肤菜单
    QStringList name;
    name << "银色" << "蓝色" << "浅蓝色" << "深蓝色" << "灰色" << "浅灰色" << "深灰色" << "黑色"
         << "浅黑色" << "深黑色" << "PS黑色" << "黑色扁平" << "白色扁平";

    ui->style_comboBox->addItems(name);
    QObject::connect(ui->style_comboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(changeStyle(int)));
    ui->style_comboBox->setCurrentIndex(this->skin_index);

}

void MainWindow::changeStyle(int index)
{
    QStringList file_name;
    file_name << ":/qss/silvery.css" << ":/qss/dev.css" << ":/qss/lightblue.css" << ":/qss/darkblue.css" << ":/qss/gray.css" << ":/qss/lightgray.css" << ":/qss/darkgray.css" << ":/qss/black.css"
         << ":/qss/lightblack.css" << ":/qss/darkblack.css" << ":/qss/psblack.css" << ":/qss/flatblack.css" << ":/qss/flatwhite.css";
    this->skin_index = index;
    UIHelper::SetStyle(file_name[index]);
}

void MainWindow::changeLanguage()
{

}
void MainWindow::Translate(int index)
{

}
void MainWindow::SaveConfig()
{
    Config->setValue("/system/skin", this->skin_index);
    Config->setValue("/system/language", this->language_index);

}

void MainWindow::LoadConfig()
{
    this->skin_index = Config->value("/system/skin").toInt();
    this->language_index = Config->value("/system/language").toInt();

}


void MainWindow::btnClick1()
{
    QToolButton *b = (QToolButton *)sender();
    QString name = b->text();

    int count = btns1.count();
    for (int i = 0; i < count; i++) {
        if (btns1.at(i) == b) {
            btns1.at(i)->setChecked(true);
            btns1.at(i)->setIcon(QIcon(IconHelper::Instance()->getPixmap(btns1.at(i), false)));
            ui->stackedWidget->setCurrentIndex(i);
        } else {
            btns1.at(i)->setChecked(false);
            btns1.at(i)->setIcon(QIcon(IconHelper::Instance()->getPixmap(btns1.at(i), true)));
        }
    }

}

void MainWindow::on_loadFW_Button_clicked()
{
    QFileDialog *fileDialog = new QFileDialog(this);//创建一个QFileDialog对象，构造函数中的参数可以有所添加。
       fileDialog->setWindowTitle(tr("打开固件"));//设置文件打开对话框的标题
       fileDialog->setAcceptMode(QFileDialog::AcceptOpen);//设置文件对话框为保存模式
       fileDialog->setFileMode(QFileDialog::AnyFile);//设置文件对话框弹出的时候显示任何文件，不论是文件夹还是文件
       fileDialog->setViewMode(QFileDialog::Detail);//文件以详细的形式显示，显示文件名，大小，创建日期等信息；
       fileDialog->setDirectory(".");//设置文件对话框打开时初始打开的位置
       fileDialog->setNameFilter(tr("固件文件(*.fw)"));//设置文件类型过滤器
       if (fileDialog->exec() == QDialog::Accepted)
       {
           //注意使用的是QFileDialog::Accepted或者QDialog::Accepted,不是QFileDialog::Accept
           QString path = fileDialog->selectedFiles()[0];
           QFileInfo fileinfo(path);
           QFile file(path);
           file.open(QIODevice::ReadOnly);
           quint32 file_size = file.size();
           QDataStream in(&file);
           char *buffer = new char[file_size];
           in.readRawData(buffer, file_size);
           file.close();

           delete[] buffer;
       }
}

void MainWindow::on_writeFW_Button_clicked()
{
    if(!this->connected)
    {
        UIHelper::ShowMessageBoxInfo(tr("请先连接设备后再操作"));
        return;
    }

}

void MainWindow::update_log_output(QString info)
{
    QString time= QDateTime::currentDateTime ().toString ("yyyy-MM-dd hh:mm:ss");
    ui->Log_textEdit->append (tr("[%1] %2").arg (time).arg (info));
}

void MainWindow::progressBar_update(int per)
{
  if(per > 100)
  {
      per = 100;
  }
  if(per <=100 && per > 0)
  {
   ui->progressBar->setValue(per);
  }
}

void MainWindow::on_config_default_pushButton_clicked()
{
    if(!this->connected)
    {
        UIHelper::ShowMessageBoxInfo(tr("设备未连接，请先连接设备。"));
        return;
    }

}
