#ifndef DEVICEHANDLER_H
#define DEVICEHANDLER_H

#include "deviceInfo.h"
#include "counter.h"

#include <QLowEnergyController>
#include <QLowEnergyService>
#include <QLowEnergyCharacteristic>


class DeviceHandler : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool alive READ alive NOTIFY aliveChanged)

public:
    enum class AddressType {
        PublicAddress,
        RandomAddress
    };
    Q_ENUM(AddressType)

    DeviceHandler(QObject *parent = nullptr);

    void setDevice(DeviceInfo *device);

    bool measuring();
    bool alive() const;

 public slots:
    void startMeasurement();
    void stopMeasurement();
    void setUnitMeasurement(const int &unit, const int &quantity);
//    void setStep(const int &value);

signals:
    void aliveChanged();
    void stepChanged();

private:
    //QLowEnergyController
    void serviceDiscovered(const QBluetoothUuid &);
    void serviceScanDone();

    //QLowEnergyService
    void serviceStateChanged(QLowEnergyService::ServiceState s);
    void updateValue(const QLowEnergyCharacteristic &c, const QByteArray &value);

private:
    void addMeasurement(int value);
    void checkSteps(int value);
    void saveToFile();
    void removeFolderContents();
    void printFileList();

    QLowEnergyController *m_control = nullptr;
    QLowEnergyService *m_service = nullptr;
    DeviceInfo *m_currentDevice = nullptr;
    QLowEnergyDescriptor m_notificationDesc;
    QLowEnergyController::RemoteAddressType m_addressType = QLowEnergyController::PublicAddress;

    QVector<int> m_measurements;

    int m_currentValue;
    int m_prevValue;

    bool m_foundService = false;
    bool m_measuring = false;

    QString fileName;

    const int STEPS = 1;
    const int TIME = 2;
    int m_lowestStep = 19;
    Counter *m_stepCounter = new Counter();
    int m_quantity;
    int m_unit;
};


#endif // DEVICEHANDLER_H
