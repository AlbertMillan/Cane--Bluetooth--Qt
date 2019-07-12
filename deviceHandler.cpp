#include "deviceHandler.h"

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

        connect(m_control, &QLowEnergyController::serviceDiscovered,
                this, &DeviceHandler::serviceDiscovered);
        connect(m_control, &QLowEnergyController::discoveryFinished,
                this, &DeviceHandler::serviceScanDone);

        connect(m_control, static_cast<void (QLowEnergyController::*)(QLowEnergyController::Error)>(&QLowEnergyController::error),
                this, [this](QLowEnergyController::Error error) {
            Q_UNUSED(error);
            qDebug() << "Cannot connect to remote device.";
//            setError("Cannot connect to remote device.");
        });
        connect(m_control, &QLowEnergyController::connected, this, [this]() {
            qDebug() << "Controller connected. Search services...";
//            setInfo("Controller connected. Search services...");
            m_control->discoverServices();
        });
        connect(m_control, &QLowEnergyController::disconnected, this, [this]() {
            qDebug() << "LowEnergy controller disconnected";
//            setError("LowEnergy controller disconnected");
        });


        qDebug() << "Setting Device...";

        // Connect
        m_control->connectToDevice();

    }
}


void DeviceHandler::serviceDiscovered(const QBluetoothUuid &gatt) {
    qDebug() << "Service might have been discovered. Success!";
}

void DeviceHandler::serviceScanDone() {
    qDebug() << "Service scan completed!";
}
