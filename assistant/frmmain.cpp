#include "frmmain.h"
#include "ui_frmmain.h"
#include "api/uihelper.h"
#include <QSerialPortInfo>
#include "searchfrm.h"
#include "aes.h"

frmMain::frmMain(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmMain)
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

    //QObject::connect(Communication::Instance(),&Communication::MsgReady,this,&data_parse_ready);

  this->check_ver = 0;

}

void frmMain::timeout()
{

}

frmMain::~frmMain()
{
    delete ui;
}

void frmMain::InitStyle()
{
    this->location = this->geometry();
    this->setProperty("form", true);
    this->setProperty("CanMove", true);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(windowFlags() | Qt::WindowMinMaxButtonsHint  | Qt::WindowCloseButtonHint | Qt::CustomizeWindowHint);
    UIHelper::SetStyle(":/qss/blue.css");

    //设置对应载体的属性 可自行注释看效果
    ui->widget1->setProperty("flag", "left");


    //设置按钮的固定高度
    QStringList qss;
    qss.append(QString("QWidget#widget1 QAbstractButton{min-height:%1px;max-height:%1px;}").arg(32));
    this->setStyleSheet(qss.join(""));
    //准备导航的按钮集合
    //准备对应的图形字体集合
    pixChar << 0xf109 << 0xf013 << 0xf021 << 0xf1b3  << 0xf011;
    btns1 << ui->btn11 << ui->btn12 << ui->btn13 << ui->btn14 << ui->btn15;

    //设置对应的按钮的图标显示方案以及是否可以选中以及对应的单击事件
    int count = btns1.count();
    for (int i = 0; i < count; i++) {
        btns1.at(i)->setCheckable(true);
        btns1.at(i)->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        connect(btns1.at(i), SIGNAL(clicked(bool)), this, SLOT(btnClick1()));
    }

    //调用方法,方法中的对应参数可以查看具体源码
    //IconHelper::Instance()->setStyle(ui->widget1, btns1, pixChar, 15, 35, 25, "left", 4);

    IconHelper::Instance()->setStyle(ui->widget1, btns1, pixChar, 20, 40, 35, "left", 3,
                                     "#609EE9","#434D5C","#1D2530","#F5F7FA","#4B78AF");
    //默认选中哪个按钮
    ui->btn11->click();

    //添加换肤菜单
    QStringList name;
    name << "银色" << "蓝色" << "浅蓝色" << "深蓝色" << "灰色" << "浅灰色" << "深灰色" << "黑色"
         << "浅黑色" << "深黑色" << "PS黑色" << "黑色扁平" << "白色扁平";

    this->m_menu = new QMenu(this);

    QMenu* skenMenu = this->m_menu->addMenu (QIcon(":/res/image/skens_32.png"),tr("更换皮肤"));

    QActionGroup *skenGroup = new QActionGroup(this);

    quint8 i = 0;
    foreach (QString str, name) {
        QAction *action = skenMenu->addAction(str);
        skenGroup->addAction(action);
        action->setCheckable(true);
        this->skin_action_list.append(action);
        if(i==this->skin_index)
        {
            action->setChecked(true);
        }
        connect(action, SIGNAL(triggered(bool)), this, SLOT(changeStyle()));
        i++;
    }
    this->changeStyle(this->skin_index);
    skenGroup->setExclusive (true);

    //加入语言选择菜单
    QMenu* langMenu = this->m_menu->addMenu (QIcon(":/res/image/skens_32.png"),tr("语言选择"));
    QActionGroup *langGroup = new QActionGroup(this);
    QStringList lang_str_list;
    lang_str_list << tr("简体中文") << tr("English");
    i = 0;
    foreach (QString str, lang_str_list) {
        QAction *action = langMenu->addAction(str);
        langGroup->addAction(action);
        action->setCheckable(true);
        if(i==this->language_index)
        {
          action->setChecked(true);
        }
        connect(action, SIGNAL(triggered(bool)), this, SLOT(changeLanguage()));
        this->language_action_list.append(action);
        i++;
    }
    langGroup->setExclusive (true);

    m_menu->addSeparator();
    m_menu->addAction(QIcon(":/res/image/new_32.png"),tr("新版特性"));
    m_menu->addAction(QIcon(":/res/image/update_32.png"),tr("检查更新"));
    m_menu->addSeparator();
   QAction *m_about = new QAction(QIcon(":/res/image/us_32.png"),tr("关于软件"),this);
   //connect(m_about,SIGNAL(triggered()), this, SLOT(action_about_clicked()));
    m_menu->addAction(m_about);
    QAction* m_support = new QAction(QIcon(":/res/image/about_32.png"),tr("技术支持"), this);
    m_menu->addAction(m_support);
}
void frmMain::showRightMenu()
{
    m_menu->exec(QCursor::pos());
}
void frmMain::changeStyle()
{
    int i = 0;
    foreach(QAction *act,this->skin_action_list)
    {
        if(act->isChecked())
        {
            if(this->skin_index!=i)
            {
                this->skin_index = i;
                changeStyle(i);
            }

        }
        i++;
    }

}

void frmMain::changeStyle(int index)
{
    QStringList file_name;
    file_name << ":/qss/silvery.css" << ":/qss/blue.css" << ":/qss/lightblue.css" << ":/qss/darkblue.css" << ":/qss/gray.css" << ":/qss/lightgray.css" << ":/qss/darkgray.css" << ":/qss/black.css"
         << ":/qss/lightblack.css" << ":/qss/darkblack.css" << ":/qss/psblack.css" << ":/qss/flatblack.css" << ":/qss/flatwhite.css";
    this->skin_index = index;
    UIHelper::SetStyle(file_name[index]);
}
void frmMain::changeLanguage()
{
    int i = 0;
    foreach(QAction *act,this->language_action_list)
    {
        if(act->isChecked())
        {
            if(this->language_index!=i)
            {
            this->language_index = i;
            this->Translate(i);
            }
        }
        i++;
    }
}
void frmMain::Translate(int index)
{

}
void frmMain::SaveConfig()
{
    Config->setValue("/system/skin", this->skin_index);
    Config->setValue("/system/language", this->language_index);

}

void frmMain::LoadConfig()
{
    this->skin_index = Config->value("/system/skin").toInt();
    this->language_index = Config->value("/system/language").toInt();

}


void frmMain::btnClick1()
{
    QToolButton *b = (QToolButton *)sender();
    QString name = b->text();

    int count = btns1.count();
    for (int i = 0; i < count; i++) {
        if (btns1.at(i) == b) {
            btns1.at(i)->setChecked(true);
            btns1.at(i)->setIcon(QIcon(IconHelper::Instance()->getPixmap(btns1.at(i), false)));
            ui->stackedWidget->setCurrentIndex(i);
            if(i==4)
            {
                if(!this->connected)
                {
                    UIHelper::ShowMessageBoxInfo(tr("设备未连接，请先连接设备。"));
                    return;
                }
               if(UIHelper::ShowMessageBoxQuesion(tr("确定要重启设备吗？"))==QDialog::Accepted)
               {
               }
            }
        } else {
            btns1.at(i)->setChecked(false);
            btns1.at(i)->setIcon(QIcon(IconHelper::Instance()->getPixmap(btns1.at(i), true)));
        }
    }

}

void frmMain::on_loadFW_Button_clicked()
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

void frmMain::on_writeFW_Button_clicked()
{
    if(!this->connected)
    {
        UIHelper::ShowMessageBoxInfo(tr("请先连接设备后再操作"));
        return;
    }

}

void frmMain::update_log_output(QString info)
{
    QString time= QDateTime::currentDateTime ().toString ("yyyy-MM-dd hh:mm:ss");
    ui->Log_textEdit->append (tr("[%1] %2").arg (time).arg (info));
}

void frmMain::progressBar_update(int per)
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

void frmMain::on_config_default_pushButton_clicked()
{
    if(!this->connected)
    {
        UIHelper::ShowMessageBoxInfo(tr("设备未连接，请先连接设备。"));
        return;
    }

}
