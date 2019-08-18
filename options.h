#ifndef OPTIONS_H
#define OPTIONS_H

#include "optionsHandler.h"

#include <QObject>
#include <QVariant>

class Options : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QVariant users READ users NOTIFY usersChanged)

public:
    Options();

    QVariant users();

public slots:
    void addUser(const QString username, const QString weight,
                 const bool unit, const QString quantity,
                 const int poles);

signals:
    void usersChanged();

private:
    QList<OptionsHandler*> m_users;
};

#endif // OPTIONS_H
