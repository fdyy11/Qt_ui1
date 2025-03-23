#include "modbus.h"
#include <QVariant>
#include <QDebug>

Modbus::Modbus(QObject *parent) : QObject(parent)
{
    this->initModbus();
}

Modbus::~Modbus()
{

}

//初始化
void Modbus::initModbus()
{
    myClient = new QModbusTcpClient();
    //connect(myClient,SIGNAL(stateChanged()),this,SLOT(slot_stateChanged()));
    connect(myClient,&QModbusClient::stateChanged,this,&Modbus::slot_stateChanged);
}

//连接到modbus设备
void Modbus::connectToModbus(QString ip,int port)
{
    if(!myClient)
    {
        return;
    }

    //判断当前连接状态是否为断开状态
    if(myClient->state() != QModbusDevice::ConnectedState)
    {
        //配置ModbusTcp的连接参数IP+Port
        QVariant ipVariant(ip);
        QVariant portVariant(port);

        myClient->setConnectionParameter(QModbusDevice::NetworkAddressParameter,ipVariant);
        myClient->setConnectionParameter(QModbusDevice::NetworkPortParameter,portVariant);
        myClient->connectDevice();
    }
    //else
    //{
    //    myClient->disconnectDevice();
    //}
}

//读取modbus设备各寄存器数据
//typeNum:1_线圈 2_离散输入 3_保持 4_输入
bool Modbus::readModbusData(int typeNum,int startAdd,quint16 numbers)
{
    if(myClient->state() != QModbusDevice::ConnectedState)
    {
        return false;
    }

    //确定寄存器类型
    QModbusDataUnit ReadUnit;
    if(typeNum == 1)
    {
        ReadUnit = QModbusDataUnit(QModbusDataUnit::Coils,startAdd,numbers);
    }
    else if(typeNum == 2)
    {
        ReadUnit = QModbusDataUnit(QModbusDataUnit::DiscreteInputs,startAdd,numbers);
    }
    else if(typeNum == 3)
    {
        ReadUnit = QModbusDataUnit(QModbusDataUnit::HoldingRegisters,startAdd,numbers);
    }
    else if(typeNum == 4)
    {
        ReadUnit = QModbusDataUnit(QModbusDataUnit::InputRegisters,startAdd,numbers);
    }
    else
    {
        LOGDEBUG<<"读取寄存器类型错误";
        return false;
    }
    LOGDEBUG<<"readModbusData typeNum:"<<typeNum;

    //多读
    if(auto *reply = myClient->sendReadRequest(ReadUnit,1))
    {
        if(!reply->isFinished())
        {
            if((typeNum == 1) || (typeNum == 2))
            {
                QObject::connect(reply,&QModbusReply::finished,this,&Modbus::slot_readReadyCoils);   //读取线圈
            }
            if((typeNum == 3) || (typeNum == 4))
            {
                QObject::connect(reply,&QModbusReply::finished,this,&Modbus::slot_readReadyRegisters);   //读取寄存器
            }
            //reply->deleteLater();
            return true;
        }
        else
        {
            reply->deleteLater();
            return false;
        }
    }
    else
    {
        LOGDEBUG<<"读取错误:" + myClient->errorString();
                                      return false;
    }
}

//对modbus设备各寄存器写入数据
//typeNum:1_线圈 2_保持 (这两类寄存器可读可写,其余的只读)
bool Modbus::writeModbusData(int typeNum,int startAdd,int writeNum)
{
    if(myClient->state() != QModbusDevice::ConnectedState)
    {
        return false;
    }

    //确定寄存器类型
    QModbusDataUnit writeUnit;
    if(typeNum == 1)
    {
        writeUnit = QModbusDataUnit(QModbusDataUnit::Coils,startAdd,1);   //写入一个数据
        writeUnit.setValue(0,writeNum);

        //单写
        //bool ok;
        //quint16 hexData = writeData.toInt(&ok,16);   //转16进制
    }
    else if(typeNum == 2)
    {
        writeUnit = QModbusDataUnit(QModbusDataUnit::HoldingRegisters,startAdd,1);   //写入两个数据
        quint16 uData16[2] = {0};
        uData16[0] = writeNum & 0xffff;
        //uData16[1] = (writeNum >> 16) & 0xffff;
        writeUnit.setValue(0,uData16[0]);
        //writeUnit.setValue(1,uData16[1]);
        //LOGDEBUG<<"uData16[0]:"<<uData16[0]<<"   uData16[1]:"<<uData16[1]<<"   writeNum:"<<writeNum;
    }
    else
    {
        LOGDEBUG<<"写入寄存器类型错误";
        return false;
    }
    //LOGDEBUG<<"writeModbusData typeNum:"<<typeNum<<"   writeNum:"<<writeNum;
    if(auto *reply = myClient->sendWriteRequest(writeUnit,1))
    {
        if(!reply->isFinished())
        {
            connect(reply,&QModbusReply::finished,this,[reply]()
                    {
                        if(reply->error() == QModbusDevice::NoError)
                        {
                            reply->deleteLater();
                            return true;
                        }
                        else
                        {
                            LOGDEBUG<<"写入返回错误:"<<reply->error();
                                                                     reply->deleteLater();
                            return false;
                        }
                    });
        }
        else
        {
            reply->deleteLater();
            return false;
        }
    }
    else
    {
        LOGDEBUG<<"写入错误:" + myClient->errorString();
                                      return false;
    }
    return true;
}

//监听TCP连接的状态,若状态发生改变,发出对应的信号
void Modbus::slot_stateChanged()
{
    LOGDEBUG<<myClient->state();
    if(myClient->state() == QModbusDevice::UnconnectedState)
    {
                                      emit signal_stateChanged(false);
    }
    else if(myClient->state() == QModbusDevice::ConnectedState)
    {
        emit signal_stateChanged(true);
    }

}

//接收到读取线圈/离散输入寄存器请求后执行的槽函数
void Modbus::slot_readReadyCoils()
{
    QVector<quint16> vAllData;
    QModbusReply *reply = qobject_cast<QModbusReply *>(sender());
    if(!reply)
    {
        LOGDEBUG<<"读取线圈/离散输入寄存器错误";
        return;
    }
    if(reply->error() == QModbusDevice::NoError)
    {
        const QModbusDataUnit unit = reply->result();
        vAllData = unit.values();
        LOGDEBUG<<"111 "<<vAllData[0];
        emit signal_readCoils(vAllData);
    }
    else
    {
        LOGDEBUG<<"线圈/离散输入寄存器回复错误:"<<reply->error();
    }
    reply->deleteLater();
}

//接收到读取保持/输入寄存器请求后执行的槽函数
void Modbus::slot_readReadyRegisters()
{
    QModbusReply *reply = qobject_cast<QModbusReply *>(sender());
    if(!reply)
    {
        LOGDEBUG<<"读取保持/输入寄存器错误";
        return;
    }
    if(reply->error() == QModbusDevice::NoError)
    {
        const QModbusDataUnit unit = reply->result();
        auto valueList = unit.values();
        int nSize = valueList.size();
        qDebug()<<"valueList: "<<valueList;
        qDebug()<<"nSize: "<<nSize;
        if(nSize == 20)
        {
            quint16 uData16[20] = {0};
            uData16[0] = valueList[0];
            uData16[1] = valueList[1];
            uData16[2] = valueList[2];
            uData16[3] = valueList[3];
            uData16[4] = valueList[4];
            uData16[7] = valueList[7];


            int resultNum0 = uData16[0];
            int resultNum1 = uData16[1];
            int resultNum2 = uData16[2];
            int resultNum3 = uData16[3];
            int resultNum4 = uData16[4];
            int resultNum7 = uData16[7];



            LOGDEBUG<<"uData16[0]:"<<uData16[0]
                     <<"   uData16[1]:"<<uData16[1]
                     <<"   uData16[2]:"<<uData16[2]
                     <<"   uData16[3]:"<<uData16[3]
                     <<"   uData16[4]:"<<uData16[4]
                     <<"   uData16[5]:"<<uData16[7]
;

            emit signal_readRegisters5(resultNum0,resultNum1,resultNum2,resultNum3,resultNum4,resultNum7);
        }
        else if(nSize == 1)
        {
            quint16 uData16[20] = {0};
            uData16[0] = valueList[0];

            int resultNum = uData16[0];
            LOGDEBUG<<"uData16[0]:"<<uData16[0]<<"   uData16[1]:"<<uData16[1];
            LOGDEBUG<<"   resultNum:"<<resultNum;
            emit signal_readRegisters(resultNum);
        }
        else
        {
            LOGDEBUG<<"保持寄存器返回数据错误,个数:"<<nSize;
        }
    }
    else
    {
        LOGDEBUG<<"保持/输入寄存器回复错误:"<<reply->error();
    }
    reply->deleteLater();
}


