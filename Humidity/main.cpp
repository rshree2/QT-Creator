#include "dialog.h"
#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QScrollArea>
#include <QtSerialPort/QSerialPortInfo>




int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//-----------------------------------------COM PORT INFORMATION----------------------------------------------------------
    auto layout = new QVBoxLayout;
        const auto infos = QSerialPortInfo::availablePorts();
        for (const QSerialPortInfo &info : infos) {
            QString s = QObject::tr("Port: ") + info.portName() + "\n"
                        + QObject::tr("Location: ") + info.systemLocation() + "\n"
                        + QObject::tr("Description: ") + info.description() + "\n"
                        + QObject::tr("Manufacturer: ") + info.manufacturer() + "\n"
                        + QObject::tr("Serial number: ") + info.serialNumber() + "\n"
                        + QObject::tr("Vendor Identifier: ") + (info.hasVendorIdentifier() ? QString::number(info.vendorIdentifier(), 16) : QString()) + "\n"
                        + QObject::tr("Product Identifier: ") + (info.hasProductIdentifier() ? QString::number(info.productIdentifier(), 16) : QString()) + "\n"
                        + QObject::tr("Busy: ") + (info.isBusy() ? QObject::tr("Yes") : QObject::tr("No")) + "\n";
            auto label = new QLabel(s);
            layout->addWidget(label);
        }

        auto workPage = new QWidget;
        workPage->setLayout(layout);
        QScrollArea area;
        area.setWindowTitle(QObject::tr("PORT INFO"));
        area.setWidget(workPage);
        area.show();
//-----------------------------------------LINE CHART--------------------------------------------------------------------
    Dialog w;
    w.show();
    w.setWindowTitle("Relative humidity and Voltage Reading");
    w.show();
    return a.exec();
}
