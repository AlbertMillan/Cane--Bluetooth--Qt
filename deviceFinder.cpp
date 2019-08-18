#include "deviceFinder.h"
#include "deviceInfo.h"
#include <QDebug>

DeviceFinder::DeviceFinder(DeviceHandler *handler, QObject *parent) : QObject (parent), m_deviceHandler(handler) {
    m_deviceDiscoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);
    m_deviceDiscoveryAgent->setLowEnergyDiscoveryTimeout(10000);

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

    m_addresses.clear();

//    m_deviceHandler->setDevice(nullptr);
    qDeleteAll(m_devices);
    m_devices.clear();

    emit devicesChanged();

    m_deviceDiscoveryAgent->start(QBluetoothDeviceDiscoveryAgent::LowEnergyMethod);

    emit scanningChanged();
}

void DeviceFinder::connectToService() {
    m_deviceDiscoveryAgent->stop();

    QVector<DeviceInfo *> selectedDevices;
    for (QObject *entry : qAsConst(m_devices)) {
        auto device = qobject_cast<DeviceInfo *>(entry);
        if( device && m_addresses.contains(device->getAddress()) ) {
            qDebug() << "Device found:" << device->getName();
            selectedDevices.append(device);
            deviceInformation = device;
//            break;
        }
    }

//    for(int i = 0; i < selectedDevices.size(); i++) {
//        if( selectedDevices[i] ) {
//            qDebug() << "Connect to service..." << selectedDevices[i]->getName();
//            m_deviceHandler->setDevice(selectedDevices[i]);
//        }
//    }

    if( selectedDevices[0] ) {
        qDebug() << "Connect to service..." << selectedDevices[0]->getName();
        m_deviceHandler->setDevice(selectedDevices[0]);
    }

    //    clearMessages();
}

void DeviceFinder::storeAddress(const QString &address)
{
    if( m_addresses.isEmpty() ) {
        m_addresses.append(address);
        emit selectedChanged();
    }
    else if( m_addresses.size() < max_devices && !m_addresses.contains(address)) {
        m_addresses.append(address);
        emit selectedChanged();
    }
}

void DeviceFinder::removeAddress(const QString &address)
{
    if(m_addresses.removeOne(address)) {
        qDebug() << m_addresses.size();
        emit selectedChanged();
    }
}

void DeviceFinder::setMaxDevices(const int nPoles)
{
    max_devices = nPoles;
    emit selectedChanged();
}

void DeviceFinder::test()
{
    qDebug() << "Connect to service..." << deviceInformation->getName();
    secondDeviceHandler->setDevice(deviceInformation);
}


void DeviceFinder::addDevice(const QBluetoothDeviceInfo &device) {
//    qDebug() << "Found new device:" << device.name() << '(' << device.address().toString() << ')';

    // If device is LowEnergy-device, add to the list
    if( device.coreConfigurations() & QBluetoothDeviceInfo::LowEnergyCoreConfiguration ) {
        qDebug() << "Found new device:" << device.name() << '(' << device.address().toString() << ')';
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

bool DeviceFinder::limit() const
{
    return m_addresses.size() >= max_devices;
}

int DeviceFinder::selected() const
{
    return max_devices - m_addresses.size();
}
