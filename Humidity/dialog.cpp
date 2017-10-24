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
  //  ui->plot->graph(0)->setAdaptiveSampling(0);                                                     // this line of code crashes the program! dont know why!
    ui->plot->addGraph();
    ui->plotA->addGraph();
    ui->plot->graph()->setPen(QPen(Qt::blue));
    ui->plotA->graph(0)->setPen(QPen(Qt::red));
    ui->plot->xAxis->setLabel("Time(Seconds)");
    ui->plot->yAxis->setLabel("Relative Humidity(%)");
    ui->plotA->xAxis->setLabel("Time(Seconds)");
    ui->plotA->yAxis->setLabel("Resistance");
    ui->plot->graph()->setScatterStyle(QCPScatterStyle::ssCircle);
    ui->plot->graph()->setLineStyle(QCPGraph::lsLine);
    ui->plotA->graph(0)->setScatterStyle(QCPScatterStyle::ssCircle);
    ui->plotA->graph(0)->setLineStyle(QCPGraph::lsLine);
    ui->plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    ui->plotA->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
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

       // humidity= hum.toDouble();   //humidity is buffersplit[1]
       // resistance= res.toDouble();  //voltage or resistance is buffersplit[0]
        time+=1;
   //     connect(timer,SIGNAL(timeout()),this,SLOT(Plot_humidity(double humidity,double time)));
   //     connect(timer,SIGNAL(timeout()),this,SLOT(Plot_resisitance(double res,double time)));
        timer->start(1000);
   //   Plot_humidity(ui->humidity_lcdNumber->display(sensor_reading),time);
    //  Dialog::Plot_resisitance(ui->humidity_lcdNumber1->display(sensor_reading1),time);
         break;

        }
        serialBuffer="";
        i++;
        }
}

void Dialog:: updateLCD(QString sensor_reading, QString sensor_reading1){
        ui->humidity_lcdNumber->display(sensor_reading);
         ui->humidity_lcdNumber1->display(sensor_reading1);
         ui->time_lcdNumber->display(time);
    add_Point_humidity(ui->time_lcdNumber->value(),ui->humidity_lcdNumber->value());
//    add_Point_resistance(ui->time_lcdNumber->value(),ui->humidity_lcdNumber1->value());


    }


void Dialog:: Plot_humidity(){

    ui->plot->graph()->setData(qv_time,humidity);
    ui->plot->replot();
    ui->plot->update();
    ui->plot->graph()->rescaleAxes();


}
/*
void Dialog::Plot_resisitance(){
    ui->plotA->graph(0)->setData(qv_time,resistance);
    ui->plotA->replot();
    ui->plotA->update();
    ui->plotA->graph(0)->rescaleAxes();
}
*/
void Dialog:: add_Point_humidity(double time,double hum){
    humidity.append(hum);
    qv_time.append(time);
    Plot_humidity();
}
/*
void Dialog:: add_Point_resistance(double time,double res){
    humidity.append(res);
    qv_time.append(time);
    Plot_resisitance();


}

*/
