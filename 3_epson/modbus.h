#ifndef MODBUS_H
#define MODBUS_H

#include <QObject>
#include <QModbusTcpClient>
#include <QModbusDataUnit>
#include <QDebug>

#define LOGDEBUG qDebug()<<__FILE__<<__LINE__

class Modbus : public QObject
{
    Q_OBJECT
public:
    explicit Modbus(QObject *parent = nullptr);
    ~Modbus();

    void initModbus();

    void connectToModbus(QString ip,int port);

    bool readModbusData(int typeNum,int startAdd,quint16 numbers);
    bool writeModbusData(int typeNum,int startAdd,int writeNum);

signals:
    void signal_stateChanged(bool flag);
    void signal_readCoils(QVector<quint16> vAllData);
    void signal_readRegisters(int resultNum);
    void signal_readRegisters5(int resultNum1,int resultNum2,int resultNum3,int resultNum4,int resultNum5,int resultNum6);

private slots:
    void slot_stateChanged();
    void slot_readReadyCoils();
    void slot_readReadyRegisters();

private:
    QModbusTcpClient *myClient;

};
#endif // MODBUS_H
