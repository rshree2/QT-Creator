#include "dialog.h"
#include "ui_dialog.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include<string>
#include<QDebug>
#include<QMessageBox>
#include<qcustomplot.h>
#include<QtCore>
#include <QFile>
#include <QCoreApplication>
#include <QTextStream>
#include <QString>
#include <iostream>
#include <fstream>
#include<QtCore>

using namespace std;

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{//---------------------------------GLOBAL VARIABLES----------------------------------------------------------
    timer=new QTimer(this);
    ui->setupUi(this);
    ui->humidity_lcdNumber->display("-------");
    ui->humidity_lcdNumber1->display("-------");
    ui->temperature_lcdNumber->display("-------");
    ui->plot->addGraph();
    ui->plotA->addGraph();
    ui->plot_temp->addGraph();
    ui->plot->graph()->setPen(QPen(Qt::blue));
    ui->plotA->graph()->setPen(QPen(Qt::red));
    ui->plot_temp->graph()->setPen(QPen(Qt::green));
    ui->plot->xAxis->setLabel("Time(Seconds)");
    ui->plot->yAxis->setLabel("Relative Humidity(%)");
    ui->plotA->xAxis->setLabel("Time(Seconds)");
    ui->plotA->yAxis->setLabel("Temperature(*C)");
    ui->plot_temp->xAxis->setLabel("Time(Seconds)");
    ui->plot_temp->yAxis->setLabel("Resistance(ohms)");
    ui->plot->graph()->setScatterStyle(QCPScatterStyle::ssCircle);
    ui->plot->graph()->setLineStyle(QCPGraph::lsLine);
    ui->plotA->graph()->setScatterStyle(QCPScatterStyle::ssCircle);
    ui->plotA->graph()->setLineStyle(QCPGraph::lsLine);
    ui->plot_temp->graph()->setScatterStyle(QCPScatterStyle::ssCircle);
    ui->plot_temp->graph()->setLineStyle(QCPGraph::lsLine);
    ui->plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    ui->plotA->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    ui->plot_temp->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    arduino =new QSerialPort(this);
    serialBuffer="";
  if(!file.is_open())
    {
        qDebug()<<"File not Found,retry........";

    }

   qDebug()<<"Number of ports :"<<QSerialPortInfo::availablePorts().length()<<"\n";
       foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()){
            qDebug()<<"Description"<< serialPortInfo.description()<<"\n";
            qDebug()<<"Vendor ID :"<< serialPortInfo.vendorIdentifier()<<"\n";
            qDebug()<<"Product ID :"<< serialPortInfo.productIdentifier()<<"\n";
            qDebug()<<"SerialNumber:"<< serialPortInfo.serialNumber()<<"\n";
        }
        is_port_opened=false;
        arduino_is_available=false;
        QObject::connect(ui->start_pushButton,SIGNAL(released()),this,SLOT(Configure_Port()));
        QObject::connect(timer,SIGNAL(timeout()),this,SLOT(updateLCD(QString sensor_reading,QString sensor_reading1,QString sensor_reading2)));

    }
//--------------------------------------------------------------------------------------------------------------------------------
/*
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
*/

//--------------------------------------------------------------------------------------------------------------------------------

Dialog::~Dialog()
{
    if (arduino->isOpen()){
        arduino->close();
    }
     delete ui;
}

void Dialog::readSerial()
{
    buffersplit= serialBuffer.split("\n");
    if (buffersplit.length()<4){                                                                         // compare  the length to 4 since we are getting 3 different values (RH, Resistance, Temperature)
       serialData= arduino->readAll();
        serialBuffer+= QString::fromStdString(serialData.toStdString());
      }else{
        qDebug()<<buffersplit << time;

       if(i>1){
           Dialog::updateLCD(buffersplit[0],buffersplit[1],buffersplit[2]);

        }
        serialBuffer="";
        i=2;
        }
    }

void Dialog:: updateLCD(QString sensor_reading, QString sensor_reading1, QString sensor_reading2){
    qDebug() << "size: " << buffersplit.size() << "\n";
         ui->humidity_lcdNumber->display(sensor_reading);
         ui->humidity_lcdNumber1->display(sensor_reading1);
         ui->temperature_lcdNumber->display(sensor_reading2);
        time+=1;
         ui->time_lcdNumber->display(time);
         if(j==1){
             file<<"TIME"<<"\t"<<"RH%"<<"\t"<<"TEMPERATURE"<<"\t"<<"RESISTANCE"<<endl;
         }
         j=2;
         qv_time.append(ui->time_lcdNumber->value());
         add_Point_humidity(ui->humidity_lcdNumber->value());
         add_Point_resistance(ui->humidity_lcdNumber1->value());
         add_Point_temperature(ui->temperature_lcdNumber->value());
         file<<ui->time_lcdNumber->value()<<"\t";
         file<<ui->humidity_lcdNumber->value()<<"\t";
         file<<ui->humidity_lcdNumber1->value()<<"\t\t";
         file<<ui->temperature_lcdNumber->value()<<endl;
    }


void Dialog:: Plot_humidity(){
    ui->plot->graph()->addData(qv_time,humidity);
    ui->plot->replot();
    ui->plot->update();
    ui->plot->graph()->rescaleAxes();
}

void Dialog::Plot_resisitance(){
    ui->plotA->graph()->addData(qv_time,resistance);
    ui->plotA->replot();
    ui->plotA->update();
    ui->plotA->graph()->rescaleAxes();
}

void Dialog::Plot_temperature(){
    ui->plot_temp->graph()->addData(qv_time,temperature);
    ui->plot_temp->replot();
   // qDebug()<<qv_time;
    ui->plot_temp->update();
    ui->plot_temp->graph()->rescaleAxes();

}

void Dialog:: add_Point_humidity(double hum){
    humidity.append(hum);
    Plot_humidity();
}

void Dialog:: add_Point_resistance(double res){
    resistance.append(res);
    Plot_resisitance();
}

void Dialog::add_Point_temperature(double temp){
     temperature.append(temp);
     Plot_temperature();
    }


void Dialog::Configure_Port()
{  if(!is_port_opened){
    file.open("/home/roshan/Documents/test_test.txt",ios_base::out |ios_base::app);
    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()){
        if (serialPortInfo.hasProductIdentifier() && serialPortInfo.hasVendorIdentifier()){
            if (serialPortInfo.productIdentifier()==arduino_product_id && serialPortInfo.vendorIdentifier()==arduino_vendor_id){
                arduino_is_available=true;
                arduino_portname=serialPortInfo.portName();
            }

        }
    }

    if(arduino_is_available){
        qDebug()<<"Found the port";
        arduino->setPortName(arduino_portname);
        arduino->open(QSerialPort::ReadOnly);
        arduino->setBaudRate(QSerialPort::Baud9600);
        arduino->setDataBits(QSerialPort::Data8);
        arduino->setFlowControl(QSerialPort::NoFlowControl);
        arduino->setParity(QSerialPort::NoParity);
        arduino->setStopBits(QSerialPort::OneStop);
        QObject::connect(arduino,SIGNAL(readyRead()),this,SLOT(readSerial()));
        timer->start(1000);
        ui->start_pushButton->setText(stop);
    } else{
        qDebug()<<"port cannot be found";
        QMessageBox::information(this, "serial port error", "cant open serial port to arduino");

    }

is_port_opened=true;

    }
else{
        qDebug()<<"closed the port....";
        arduino->close();
        is_port_opened=false;
        ui->start_pushButton->setText(start);
        file.close();


}
}







