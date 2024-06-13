#ifndef UDBPROCESS_H
#define UDBPROCESS_H

#include <QProcess>

struct udb_device
{
  QString serial;
  QString mtu;
  QString version;
  QString state;
};

class UdbProcess : public QProcess
{
    Q_OBJECT

public:
    enum UDB_EXEC_RESULT
    {
        UER_SUCCESS_START,        // 启动成功
        UER_ERROR_START,          // 启动失败
        UER_SUCCESS_EXEC,         // 执行成功
        UER_ERROR_EXEC,           // 执行失败
        UER_ERROR_MISSING_BINARY, // 找不到文件
    };

    explicit UdbProcess(QObject *parent = nullptr);
    virtual ~UdbProcess();

    void execute(const QString &serial, const QStringList &args);
    void push(const QString &serial, const QString &local, const QString &remote);
    void install(const QString &serial, const QString &local);
    void removePath(const QString &serial, const QString &path);
    bool isRuning();
    void setShowTouchesEnabled(const QString &serial, bool enabled);
    QList<udb_device> getDevicesSerialFromStdOut();
    QString getStdOut();
    QString getErrorOut();

    static const QString &getUdbPath();

    static bool StartShell(const QString &serial);

signals:
    void udbProcessResult(UDB_EXEC_RESULT processResult);

private:
    void initSignals();

private:
    QString m_standardOutput = "";
    QString m_errorOutput = "";
    static QString s_adbPath;
};

#endif // UDBPROCESS_H
