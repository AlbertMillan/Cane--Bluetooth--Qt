#include "options.h"
#include <QDebug>

Options::Options()
{

}

void Options::addUser(const QString username, const QString weight, const bool unit, const QString quantity, const int poles)
{
    OptionsHandler *newUser = new OptionsHandler();
    OptionsHandler *test = new OptionsHandler();
    newUser->storeOptions(username, weight, unit, quantity, poles);
    test->storeOptions(username, weight, unit, quantity, poles);


    m_users.append(newUser);
    m_users.append(test);

    emit usersChanged();
}

QVariant Options::users() {
    qDebug() << QVariant::fromValue(m_users);
    return QVariant::fromValue(m_users);
}
