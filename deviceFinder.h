#ifndef DEVICEFINDER_H
#define DEVICEFINDER_H

#include "deviceHandler.h"

#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothDeviceInfo>
#include <QVariant>
#include <QVector>


class DeviceFinder: public QObject
{

    Q_OBJECT

    // var_name READ method NOTIFY var_emitted
    // Executes 'method' every time 'var_emitted' is emitted, returning 'var_name'
    Q_PROPERTY(bool scanning READ scanning NOTIFY scanningChanged)
    Q_PROPERTY(QVariant devices READ devices NOTIFY devicesChanged)
    Q_PROPERTY(bool deviceLimitReached READ limit NOTIFY selectedChanged)
    Q_PROPERTY(int selected READ selected NOTIFY selectedChanged)

public:
    DeviceFinder(DeviceHandler *handler, QObject *parent = nullptr);
//    ~DeviceFinder();

    bool scanning() const;
    QVariant devices();
    bool limit() const;
    int selected() const;

// Slots - metodos que se pueden acceder desde qml?
public slots:
    void startSearch();
    void connectToService();
    void storeAddress(const QString &address);
    void removeAddress(const QString &address);
    void setMaxDevices(const int nPoles);

    void test();

private slots:
    void addDevice(const QBluetoothDeviceInfo&);
    void scanError(QBluetoothDeviceDiscoveryAgent::Error error);
    void scanFinished();

signals:
    void scanningChanged();
    void devicesChanged();
    void selectedChanged();


private:
    // QLowEnergyController
    void serviceDiscovered(const QBluetoothUuid &);
    void serviceScanDone();

private:
    DeviceHandler *m_deviceHandler;

    QBluetoothDeviceDiscoveryAgent *m_deviceDiscoveryAgent;
    QList<QObject*> m_devices;
    QVector<QString> m_addresses;
    int max_devices = 2;

    DeviceHandler *secondDeviceHandler = new DeviceHandler();
    DeviceInfo *deviceInformation;
};
#endif // DEVICEFINDER_H
