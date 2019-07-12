#include "deviceFinder.h"
#include "deviceInfo.h"
#include <QDebug>

DeviceFinder::DeviceFinder(DeviceHandler *handler, QObject *parent) : QObject (parent), m_deviceHandler(handler) {
    m_deviceDiscoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);
    m_deviceDiscoveryAgent->setLowEnergyDiscoveryTimeout(5000);

    // Vincula accion (e.g. 'deviceDiscovered') con metodos internos (e.g. 'addDevice')
    connect(m_deviceDiscoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered, this, &DeviceFinder::addDevice);
    connect(m_deviceDiscoveryAgent, static_cast<void (QBluetoothDeviceDiscoveryAgent::*)(QBluetoothDeviceDiscoveryAgent::Error)>(&QBluetoothDeviceDiscoveryAgent::error),
            this, &DeviceFinder::scanError);

    // Ejecuta 'scanFinished' en caso de terminar o cancelar operacion.
    connect(m_deviceDiscoveryAgent, &QBluetoothDeviceDiscoveryAgent::finished, this, &DeviceFinder::scanFinished);
    connect(m_deviceDiscoveryAgent, &QBluetoothDeviceDiscoveryAgent::canceled, this, &DeviceFinder::scanFinished);

//    m_deviceDiscoveryAgent->start();
}


bool DeviceFinder::scanning() const {
    return m_deviceDiscoveryAgent->isActive();
}


void DeviceFinder::startSearch() {
//    clearMessages();
//    m_deviceHandler->setDevice(nullptr);
    qDeleteAll(m_devices);
    m_devices.clear();

    emit devicesChanged();

    m_deviceDiscoveryAgent->start();

    emit scanningChanged();
}

void DeviceFinder::connectToService(const QString &address) {
    m_deviceDiscoveryAgent->stop();

    DeviceInfo *currentDevice = nullptr;
    for (QObject *entry : qAsConst(m_devices)) {
        auto device = qobject_cast<DeviceInfo *>(entry);
        if( device && device->getAddress() == address ) {
            qDebug() << "Device found:" << device->getName();
            currentDevice = device;
            break;
        }
    }

    if( currentDevice ) {
        qDebug() << "Connect to service...";
        m_deviceHandler->setDevice(currentDevice);
    }
//    clearMessages();
}


void DeviceFinder::addDevice(const QBluetoothDeviceInfo &device) {

    qDebug() << "Found new device:" << device.name() << '(' << device.address().toString() << ')';


    // If device is LowEnergy-device, add to the list
    if( device.coreConfigurations() & QBluetoothDeviceInfo::LowEnergyCoreConfiguration ) {
        m_devices.append(new DeviceInfo(device));
//        setInfo(tr("Low Energy device found. Scanning more..."));
        emit devicesChanged();
    }
}


void DeviceFinder::scanError(QBluetoothDeviceDiscoveryAgent::Error error)
{
    if (error == QBluetoothDeviceDiscoveryAgent::PoweredOffError) {
        qDebug() << "Turn on bluetooth for it to work";
    }
    else if (error == QBluetoothDeviceDiscoveryAgent::InputOutputError){
        qDebug() << "Writing or reading from the device resulted in an error.";
    }
    else {
        qDebug() << "Unknown error ocurred.";
    }
}

void DeviceFinder::scanFinished()
{
    if (m_devices.isEmpty()){
        qDebug() << "No Low Energy devices found.";
    }
    else {
        qDebug() << "Scanning done.";
    }

    emit scanningChanged();
    emit devicesChanged();
}


QVariant DeviceFinder::devices()
{
    // Returns a copy 'm_devices' (see Q_PROPERTY in 'deviceFinder.h')
    return QVariant::fromValue(m_devices);
}
