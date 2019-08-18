#include "deviceHandler.h"
//#include "serviceInfo.h"

#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QDirIterator>
#include <QDateTime>
#include <QTimer>
#include <QStandardPaths>

DeviceHandler::DeviceHandler(QObject *parent) {

}

void DeviceHandler::setDevice(DeviceInfo *device) {
    // clearMessages()
    m_currentDevice = device;

    // Disconnect and delete old connection
    if( m_control ) {
        m_control->disconnectFromDevice();
        delete m_control;
        m_control = nullptr;
    }

    // Create new controller and connect it if device available
    if( m_currentDevice ) {

        // Make connection
        m_control = QLowEnergyController::createCentral(m_currentDevice->getDevice(), this);
        m_control->setRemoteAddressType(m_addressType);

        connect(m_control, &QLowEnergyController::serviceDiscovered, this, &DeviceHandler::serviceDiscovered);

        connect(m_control, &QLowEnergyController::discoveryFinished, this, &DeviceHandler::serviceScanDone);

        connect(m_control, static_cast<void (QLowEnergyController::*)(QLowEnergyController::Error)>(&QLowEnergyController::error),
                this, [this](QLowEnergyController::Error error) {
            Q_UNUSED(error);
            qDebug() << "Cannot connect to remote device:" << error;
        });
        connect(m_control, &QLowEnergyController::connected, this, [this]() {
            qDebug() << "Controller connected. Search services...";
            m_control->discoverServices();
        });
        connect(m_control, &QLowEnergyController::disconnected, this, [this]() {
            qDebug() << "LowEnergy controller disconnected";
        });


        qDebug() << "Setting Device...";

        // Connect
        m_control->connectToDevice();

    }
}


void DeviceHandler::serviceDiscovered(const QBluetoothUuid &gatt) {
//    qDebug() << "Service discovered:" << gatt;
    if(gatt == QBluetoothUuid(QBluetoothUuid::BatteryService)) {
        qDebug() << "Service discovered:" << gatt;
        m_foundService = true;
    }
}

void DeviceHandler::serviceScanDone() {
    qDebug() << "Service scan completed!";

    // Connect to service discovered...
    if(m_foundService) {
        m_service = m_control->createServiceObject(QBluetoothUuid(QBluetoothUuid::BatteryService), this);
    }

    if(m_service) {
        qDebug() << "Connecting to service discovered..." << m_service->serviceName();
        connect(m_service, &QLowEnergyService::stateChanged, this, &DeviceHandler::serviceStateChanged);
        connect(m_service, &QLowEnergyService::characteristicChanged, this, &DeviceHandler::updateValue);
        m_service->discoverDetails();

        qDebug() << "Post discover details";
    }
    else {
        qDebug() << "ERROR: Service not found.";
    }
}

void DeviceHandler::serviceStateChanged(QLowEnergyService::ServiceState s)
{
    switch (s) {
    case QLowEnergyService::DiscoveringServices:
//        setInfo(tr("Discovering services..."));
        break;
    case QLowEnergyService::ServiceDiscovered:
    {
        qDebug() << "In service discovered...";
        const QLowEnergyCharacteristic hrChar = m_service->characteristic(QBluetoothUuid(QBluetoothUuid::BatteryLevel));

        if( !hrChar.isValid() ) {
            qDebug() << "hrChar is not valid...";
            return;
        }

        // POLLINGS - Perhaps would be better if I could use sockets...
        m_notificationDesc = hrChar.descriptor(QBluetoothUuid::ClientCharacteristicConfiguration);

        if (m_notificationDesc.isValid())
            m_service->writeDescriptor(m_notificationDesc, QByteArray::fromHex("0100"));

        break;
    }
    default:
        //nothing for now
        break;
    }

    emit aliveChanged();
}


void DeviceHandler::updateValue(const QLowEnergyCharacteristic &c, const QByteArray &value)
{
    qDebug() << "Value qbytearray value:" << int(value[0]);
    addMeasurement(int(value[0]));
}


void DeviceHandler::startMeasurement() {
    qDebug() << "Starting Measurement";
    m_measuring = true;

    if(m_unit == TIME) {
        QTimer *timer = new QTimer(this);
        timer->setSingleShot(true);

        connect(timer, &QTimer::timeout, this, &DeviceHandler::stopMeasurement);
        timer->start(m_quantity * 60 * 1000);
    }
    else {
        m_stepCounter->reset();
        connect(m_stepCounter, &Counter::valueChanged, this, &DeviceHandler::checkSteps);
    }
//        emit measuringChanged();
}

void DeviceHandler::stopMeasurement() {
    if(m_measuring) {
        qDebug() << "Stopping Measurement";
        m_service->writeDescriptor(m_notificationDesc, QByteArray::fromHex("0000"));
        m_measuring = false;
        this->saveToFile();
//        emit measuringChanged();
    }
}

void DeviceHandler::addMeasurement(int value) {
    m_prevValue = m_currentValue;
    m_currentValue = value;
    m_measurements << value;


    if(m_unit == STEPS && m_currentValue == m_lowestStep && m_prevValue != m_lowestStep) {
        m_stepCounter->increment();
    }


//    m_stop = QDateTime::currentMSecsSinceEpoch();
//    m_timestamps << QDateTime::currentMSecsSinceEpoch();

    //    emit statsChanged();
}

void DeviceHandler::checkSteps(int value)
{
    if(value >= m_quantity) {
        stopMeasurement();
    }
}

void DeviceHandler::setUnitMeasurement(const int &unit, const int &quantity)
{
    m_unit = (unit == STEPS ? STEPS : TIME);
    m_quantity = quantity;
}

//void DeviceHandler::setStep(const int &value)
//{
//    m_lowestStep = value;
//}

void DeviceHandler::saveToFile()
{
//    setInfo("Guardando valores...");

    QDateTime now = QDateTime::currentDateTime();
    QString timestamp = now.toString(QLatin1String("yyyyMMdd-hhmmss"));

//    auto path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
//    fileName = path + "/" + QString::fromLatin1("%1.csv").arg(timestamp);
    fileName = QString::fromLatin1("%1.csv").arg(timestamp);

    QFile file(fileName);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        qDebug() << "Error guardando valores en archivo";
        return;
    }

    QTextStream stream(&file);

    for(int i = 0; i<m_measurements.size(); i++){
//        stream << m_measurements[i] << "," << m_timestamps[i] << endl ;
        stream << m_measurements[i] << endl ;
    }

    stream << endl;

    file.close();
    qDebug() << "Archivo guardado";

    QFileInfo info(fileName);
    qDebug() << info.absoluteFilePath();

    QDir dir(".");
    qDebug() << dir.absolutePath();

    printFileList();

//    removeFolderContents();

//    printFileList();

//    setInfo(QString("%1 valores guardados en %2").arg(m_measurements.size()).arg(file.fileName()));

    // SEND FILE TO PC GOES HERE
}

void DeviceHandler::removeFolderContents()
{
    QDir dir(".");
    dir.setNameFilters(QStringList() << "*.csv");
    dir.setFilter(QDir::Files);
    foreach(QString dirFile, dir.entryList())
    {
        dir.remove(dirFile);
    }
}

void DeviceHandler::printFileList() {
    QDir dir(".");

    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    dir.setSorting(QDir::Size | QDir::Reversed);

    QFileInfoList list = dir.entryInfoList();
    qDebug() << "     Bytes Filename";
    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);
        qDebug() << qPrintable(QString("%1 %2").arg(fileInfo.size(), 10)
                                                    .arg(fileInfo.fileName()));
    }
}



bool DeviceHandler::measuring() {
    return m_measuring;
}


bool DeviceHandler::alive() const
{
    if( m_service ) {
        return m_service->state() == QLowEnergyService::ServiceDiscovered;
    }

    return false;
}
