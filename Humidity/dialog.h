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

private slots:
    void readSerial();
    void updateLCD(QString sensor_reading, QString sensor_reading1);
    void Plot_humidity(const QVector<double> humidity,const QVector <double> qv_time);
    void Plot_resisitance(const QVector<double> resistance,const QVector<double> qv_time);


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
    qint16 time=0;
    QString sensor_reading, sensor_reading1;
    QVector<double> qv_time,humidity,temp_time,resistance;

};

#endif // DIALOG_H

