#ifndef DEVICEFINDER_H
#define DEVICEFINDER_H

#include "deviceHandler.h"

#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothDeviceInfo>
#include <QVariant>


class DeviceFinder: public QObject
{

    Q_OBJECT

    // var_name READ method NOTIFY var_emitted
    // Executes 'method' every time 'var_emitted' is emitted, returning 'var_name'
    Q_PROPERTY(bool scanning READ scanning NOTIFY scanningChanged)
    Q_PROPERTY(QVariant devices READ devices NOTIFY devicesChanged)

public:
    DeviceFinder(DeviceHandler *handler, QObject *parent = nullptr);
//    ~DeviceFinder();

    bool scanning() const;
    QVariant devices();

// Slots - metodos que se pueden acceder desde qml?
public slots:
    void startSearch();
    void connectToService(const QString &address);

private slots:
    void addDevice(const QBluetoothDeviceInfo&);
    void scanError(QBluetoothDeviceDiscoveryAgent::Error error);
    void scanFinished();

signals:
    void scanningChanged();
    void devicesChanged();


private:
    // QLowEnergyController
    void serviceDiscovered(const QBluetoothUuid &);
    void serviceScanDone();

private:
    DeviceHandler *m_deviceHandler;
    QBluetoothDeviceDiscoveryAgent *m_deviceDiscoveryAgent;
    QList<QObject*> m_devices;
};
#endif // DEVICEFINDER_H
