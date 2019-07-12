#ifndef DEVICEHANDLER_H
#define DEVICEHANDLER_H

#include "deviceInfo.h"

#include <QLowEnergyController>
#include <QLowEnergyService>


class DeviceHandler : public QObject
{
    Q_OBJECT

public:
    enum class AddressType {
        PublicAddress,
        RandomAddress
    };
    Q_ENUM(AddressType)

    DeviceHandler(QObject *parent = nullptr);

    void setDevice(DeviceInfo *device);
//    void setAddressType(AddressType type);
//    AddressType addressType() const;


private:
    //QLowEnergyController
    void serviceDiscovered(const QBluetoothUuid &);
    void serviceScanDone();

private:
    QLowEnergyController *m_control = nullptr;
    QLowEnergyService *m_service = nullptr;
    DeviceInfo *m_currentDevice = nullptr;
    QLowEnergyDescriptor m_notificationDesc;
    QLowEnergyController::RemoteAddressType m_addressType = QLowEnergyController::PublicAddress;
};


#endif // DEVICEHANDLER_H
