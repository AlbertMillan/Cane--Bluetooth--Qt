#include "optionsHandler.h"
#include <QDebug>

OptionsHandler::OptionsHandler()
{
//    for (int i=0; i < 3; i++) {
//        m_options.append("");
//    }
}


void OptionsHandler::setText(const int position, QString text)
{
    m_options[position] = text;
    qDebug() << m_options[position];
}


QString OptionsHandler::getText(const int position)
{
    return m_options[position];
}

void OptionsHandler::storeOptions(const QString username, const QString weight, const bool unit, const QString quantity, const int poles)
{
    m_options.append(username);
    m_options.append(weight);
    m_options.append(quantity);

    m_unit = unit ? PASOS : TIEMPO;

    m_poles = poles;

    qDebug() << "Storing options.";
}
