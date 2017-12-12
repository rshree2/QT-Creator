#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include<QSerialPort>
#include<qcustomplot.h>
#include<QtCore>
#include <QFile>
#include <QCoreApplication>
#include <QTextStream>
#include <iostream>
#include <fstream>
#include<QtCore>
using namespace std;


namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

public slots:
   void add_Point_humidity( double hum);
   void add_Point_resistance(double res);
   void add_Point_temperature( double temp);
   void Plot_resisitance();
   void Plot_temperature();
private slots:
    void readSerial();
    void updateLCD(QString, QString,QString);
    void Plot_humidity();
    void Configure_Port();




private:
    Ui::Dialog *ui;
    QSerialPort *arduino;
    QTimer *timer;
    static const qint16 arduino_vendor_id=10755;
    static const qint16 arduino_product_id=67;
  //  static const QString arduino_serialNumber=95530343434351A09022;
    QString serialBuffer;
    QStringList buffersplit;
    QByteArray serialData;
    qint8 i=1;
    qint8 j=1;
    int time =0;
    QString sensor_reading, sensor_reading1,sensor_reading2;
    QVector <double> humidity,resistance,temperature,qv_time;
    bool arduino_is_available;
    bool is_port_opened;
    QString arduino_portname;
    QString stop= "STOP";
    QString start= "START";
    fstream file;






};

#endif // DIALOG_H

