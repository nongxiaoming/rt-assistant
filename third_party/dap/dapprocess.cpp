#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QProcess>

#include "dapprocess.h"
#include "config.h"

QString UdbProcess::s_adbPath = "";

UdbProcess::UdbProcess(QObject *parent) : QProcess(parent)
{
    initSignals();
}

UdbProcess::~UdbProcess()
{
    if (isRuning()) {
        close();
    }
}

const QString &UdbProcess::getUdbPath()
{
    if (s_adbPath.isEmpty()) {
        s_adbPath = QString::fromLocal8Bit(qgetenv("UDB_TOOL_PATH"));
        QFileInfo fileInfo(s_adbPath);
        if (s_adbPath.isEmpty() || !fileInfo.isFile()) {
            s_adbPath = Config::getInstance().getAdbPath();
        }
        fileInfo = s_adbPath;
        if (s_adbPath.isEmpty() || !fileInfo.isFile()) {
            s_adbPath = QCoreApplication::applicationDirPath() + "/udb";
        }
        qInfo("udb path: %s", QDir(s_adbPath).absolutePath().toUtf8().data());
    }
    return s_adbPath;
}

 bool UdbProcess::StartShell(const QString &serial)
{
     return true;
}
void UdbProcess::initSignals()
{
    QObject::connect(this, static_cast<void (QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished), this, [this](int exitCode, QProcess::ExitStatus exitStatus) {
        if (NormalExit == exitStatus && 0 == exitCode) {
            emit udbProcessResult(UER_SUCCESS_EXEC);
        } else {
            emit udbProcessResult(UER_ERROR_EXEC);
        }
        qDebug() << "udb return " << exitCode << "exit status " << exitStatus;
    });

     QObject::connect(this, &QProcess::errorOccurred, this, [this](QProcess::ProcessError error) {
        if (QProcess::FailedToStart == error) {
            emit udbProcessResult(UER_ERROR_MISSING_BINARY);
        } else {
            emit udbProcessResult(UER_ERROR_START);
            QString err = QString("qprocess start error:%1 %2").arg(program()).arg(arguments().join(" "));
            qCritical() << err.toStdString().c_str();
        }
    });

     QObject::connect(this, &QProcess::readyReadStandardError, this, [this]() {
        QString tmp = QString::fromUtf8(readAllStandardError()).trimmed();
        m_errorOutput += tmp;
        qWarning() << QString("UdbProcess::error:%1").arg(tmp).toStdString().data();
    });

     QObject::connect(this, &QProcess::readyReadStandardOutput, this, [this]() {
        QString tmp = QString::fromUtf8(readAllStandardOutput()).trimmed();
        m_standardOutput += tmp;
        qInfo() << QString("UdbProcess::out:%1").arg(tmp).toStdString().data();
    });

     QObject::connect(this, &QProcess::started, this, [this]() { emit udbProcessResult(UER_SUCCESS_START); });
}

void UdbProcess::execute(const QString &serial, const QStringList &args)
{
    m_standardOutput = "";
    m_errorOutput = "";
    QStringList udbArgs;
    if (!serial.isEmpty()) {
        udbArgs << "-s" << serial;
    }
    udbArgs << args;
    qDebug() << getUdbPath() << udbArgs.join(" ");
    start(getUdbPath(), udbArgs);
}

bool UdbProcess::isRuning()
{
    if (QProcess::NotRunning == state()) {
        return false;
    } else {
        return true;
    }
}

void UdbProcess::setShowTouchesEnabled(const QString &serial, bool enabled)
{
    QStringList args;
    args << "shell"
            << "settings"
            << "put"
            << "system"
            << "show_touches";
    args << (enabled ? "1" : "0");
    execute(serial, args);
}

QList<udb_device> UdbProcess::getDevicesSerialFromStdOut()
{
    // get devices serial by udb devices
    QList<udb_device> device_list;
    device_list.clear();
    bool has_device = false;
#if (QT_VERSION >= QT_VERSION_CHECK(5, 13, 0))
    QStringList devicesInfoList = m_standardOutput.split(QRegExp("\r\n|\n"), Qt::SkipEmptyParts);
#else
    QStringList devicesInfoList = m_standardOutput.split(QRegExp("\r\n|\n"), QString::SkipEmptyParts);
#endif
    for (QString deviceInfo : devicesInfoList) {
#if (QT_VERSION >= QT_VERSION_CHECK(5, 13, 0))
        QStringList deviceInfos = deviceInfo.split(QRegExp("\\s+"), Qt::SkipEmptyParts);
#else
        QStringList deviceInfos = deviceInfo.split(QRegExp("\\s+"), QString::SkipEmptyParts);
#endif
        if (has_device) {
            if(deviceInfos.count()>=4)
            {
            udb_device device;
            device.serial = deviceInfos[0];
            device.mtu = deviceInfos[1];
            device.version = deviceInfos[2];
            device.state = deviceInfos[3];
            device_list.append(device);
            }
        }
        if((has_device==false)&&(deviceInfos.count() > 3)&&(deviceInfos[0].compare("serial")==0)&&(deviceInfos[1].compare("mtu")==0)&&(deviceInfos[2].compare("version")==0))
        {
            has_device = true;
        }
    }
    return device_list;
}

QString UdbProcess::getStdOut()
{
    return m_standardOutput;
}

QString UdbProcess::getErrorOut()
{
    return m_errorOutput;
}

void UdbProcess::push(const QString &serial, const QString &local, const QString &remote)
{
    QStringList args;
    args << "push";
    args << local;
    args << remote;
    execute(serial, args);
}

void UdbProcess::install(const QString &serial, const QString &local)
{
    QStringList args;
    args << "install";
    args << "-r";
    args << local;
    execute(serial, args);
}

void UdbProcess::removePath(const QString &serial, const QString &path)
{
    QStringList args;
    args << "shell";
    args << "rm";
    args << path;
    execute(serial, args);
}
