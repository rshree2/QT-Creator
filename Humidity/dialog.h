#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include<QSerialPort>
#include<qcustomplot.h>
#include<QtCore>
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
   void add_Point_humidity(double time, double hum);
   void add_Point_resistance(double time, double res);
   void Plot_resisitance();
private slots:
    void readSerial();
    void updateLCD(QString sensor_reading, QString sensor_reading1);
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
    QByteArray serialData;
    qint8 i=1;
    int time =0;
    QString sensor_reading, sensor_reading1;
    QVector <double> humidity,resistance;
    QVector <double> qv_time;
    bool arduino_is_available;
    bool is_port_opened;
    QString arduino_portname;
    QString stop= "STOP";
    QString start= "START";
};

#endif // DIALOG_H

