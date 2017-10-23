#include "dialog.h"
#include "ui_dialog.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include<string>
#include<QDebug>
#include<QMessageBox>
#include<qcustomplot.h>
#include<QtCore>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    timer=new QTimer(this);

    ui->setupUi(this);
    ui->humidity_lcdNumber->display("-------");
    ui->humidity_lcdNumber1->display("-------");
  //  connect(ui->start_pushbutton,SIGNAL(clicked(bool)),ui->time_lcdNumber,SLOT(display(double)))
    ui->plot->addGraph();
    ui->plot->graph(0)->setScatterStyle(QCPScatterStyle::ssCircle);
    ui->plot->graph(0)->setLineStyle(QCPGraph::lsNone);
    arduino =new QSerialPort(this);
    serialBuffer="";



   qDebug()<<"Number of ports :"<<QSerialPortInfo::availablePorts().length()<<"\n";

   foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()){
       qDebug()<<"Description"<< serialPortInfo.description()<<"\n";
       qDebug()<<"Vendor ID :"<< serialPortInfo.vendorIdentifier()<<"\n";
       qDebug()<<"Product ID :"<< serialPortInfo.productIdentifier()<<"\n";
       qDebug()<<"SerialNumber:"<< serialPortInfo.serialNumber()<<"\n";

     }

   bool arduino_is_availabe=false;
   QString arduino_portname;

   foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()){
       if (serialPortInfo.hasProductIdentifier() && serialPortInfo.hasVendorIdentifier()){
           if (serialPortInfo.productIdentifier()==arduino_product_id && serialPortInfo.vendorIdentifier()==arduino_vendor_id){
               arduino_is_availabe=true;
               arduino_portname=serialPortInfo.portName();
           }

       }
   }

   if(arduino_is_availabe){
       qDebug()<<"Found the port";
       arduino->setPortName(arduino_portname);
       arduino->open(QSerialPort::ReadOnly);
       arduino->setBaudRate(QSerialPort::Baud9600);
       arduino->setDataBits(QSerialPort::Data8);
       arduino->setFlowControl(QSerialPort::NoFlowControl);
       arduino->setParity(QSerialPort::NoParity);
       arduino->setStopBits(QSerialPort::OneStop);
       QObject::connect(arduino,SIGNAL(readyRead()),this,SLOT(readSerial()));
   }else{
       qDebug()<<"port cannot be found";
       QMessageBox::information(this, "serial port error", "cant open serial port to arduino");
   }


}

Dialog::~Dialog()
{

    if (arduino->isOpen()){
        arduino->close();
    }
     delete ui;
}
void Dialog::readSerial()
{

   QStringList buffersplit= serialBuffer.split(",");
   if (buffersplit.length()<3){
        serialData= arduino->readAll();
        serialBuffer+= QString::fromStdString(serialData.toStdString());
      }else{
        qDebug()<<buffersplit;
        while(i>1){
        Dialog::updateLCD(buffersplit[1],buffersplit[0]);
        humidity= (double) buffersplit[1];   //humidity is buffersplit[1]
        resistance= (double) buffersplit[0];  //voltage or resistance is buffersplit[0]
        connect(timer,SIGNAL(timeout()),this,SLOT(Plot_humidity(humidity,qv_time)));
        connect(timer,SIGNAL(timeout()),this,SLOT(Plot_resisitance(resistance,qv_time)));
        timer->start(1000);
     //   Dialog::Plot_humidity(humidity,temp_time)
     //   Dialog::Plot_resisitance(resistance,temp_time)
         break;

        }
        serialBuffer="";
        i++;
        }
}

void Dialog:: updateLCD(QString sensor_reading, QString sensor_reading1){
        ui->humidity_lcdNumber->display(sensor_reading);
         ui->humidity_lcdNumber1->display(sensor_reading1);
    }



void Dialog:: Plot_humidity(const QVector<double> humidity,const QVector <double>  qv_time){
    ui->plot->graph()->setData(humidity,qv_time);
    ui->plot->replot();
    ui->plot->update();

}
/*
void Dialog::Plot_resisitance(resistance, qv_time){
    ui->plot->graph()->setData(resistance,time);
    ui->plot->replot();
    ui->plot->update();

}
*/
