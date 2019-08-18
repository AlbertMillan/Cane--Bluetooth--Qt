#include "optionshandler.h"

OptionsHandler::OptionsHandler(QObject *parent) : QObject(parent)
{
//    m_options.append({ QStringLiteral("Josh"), 50, 1, 300, 1 });
}

QVector<OptionItem> OptionsHandler::users() const
{
    return m_options;
}

bool OptionsHandler::setItemAt(int index, const OptionItem &item)
{
    if( index < 0 || index >= m_options.size())
        return false;

    const OptionItem &oldOptions = m_options.at(index);
    if( item.username == oldOptions.username && item.weight == oldOptions.weight &&
        item.quantity == oldOptions.quantity && item.poles == oldOptions.poles
      )
    {
        return false;
    }

    m_options[index] = item;
    return true;

}

void OptionsHandler::addUser(const QString username, const QString weight, const int unit, const QString quantity, const int poles)
{
    emit beforeAdd();

    OptionItem newUser;
    newUser.username = username;
    newUser.weight = weight;
    newUser.quantity = quantity;
    newUser.unit = unit;
    newUser.poles = poles;

    m_options.append(newUser);

    emit afterAdd();
}

void OptionsHandler::removeUser(const int index)
{
    emit beforeRemoved(index);
    m_options.removeAt(index);
    emit afterRemoved();
}

void OptionsHandler::setOptions(const int index)
{
    m_index = index;
    emit userChanged();
}

int OptionsHandler::getTime()
{
    return m_options[m_index].quantity.toInt();
}

int OptionsHandler::getUnit()
{
    return m_options[m_index].unit;
}
