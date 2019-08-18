#ifndef OPTIONSHANDLER_H
#define OPTIONSHANDLER_H

#include <QObject>
#include <QVector>
#include <QModelIndex>

struct OptionItem
{
    QString username;
    QString weight;
    int unit;
    QString quantity;
    int poles;
};

class OptionsHandler : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int time READ getTime NOTIFY userChanged)
    Q_PROPERTY(int unit READ getUnit NOTIFY userChanged)

public:
    explicit OptionsHandler(QObject *parent = nullptr);

    QVector<OptionItem> users() const;

    bool setItemAt(int index, const OptionItem &item);

signals:
    void beforeAdd();
    void afterAdd();
    void beforeRemoved(int index);
    void afterRemoved();
    void userChanged();

public slots:
    void addUser(const QString username, const QString weight, const int unit, const QString quantity, const int poles);
    void removeUser(const int index);
    void setOptions(const int index);

    int getTime();
    int getUnit();

private:
    QVector<OptionItem> m_options;

//    QModelIndex m_index;

    int m_index;
};

#endif // OPTIONSHANDLER_H
