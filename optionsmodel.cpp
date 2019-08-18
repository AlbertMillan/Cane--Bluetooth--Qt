#include "optionsmodel.h"
#include "optionshandler.h"

OptionsModel::OptionsModel(QObject *parent)
    : QAbstractListModel(parent),
      m_list(nullptr)
{
}

int OptionsModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid() || !m_list)
        return 0;

    // FIXME: Implement me!
    return m_list->users().size();
}

QVariant OptionsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || !m_list)
        return QVariant();

    const OptionItem item = m_list->users().at(index.row());
    switch (role) {
    case UsernameRole:
        return QVariant(item.username);
    case WeightRole:
        return QVariant(item.weight);
    case UnitRole:
        return QVariant(item.unit);
    case QuantityRole:
        return QVariant(item.quantity);
    case PoleRole:
        return QVariant(item.poles);
    }

    return QVariant();
}

bool OptionsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!m_list)
        return false;

    OptionItem item = m_list->users().at(index.row());

    switch (role) {
    case UsernameRole:
        item.username = value.toString();
        break;
    case WeightRole:
        item.weight = value.toInt();
        break;
    case UnitRole:
        item.unit = value.toInt();
        break;
    case QuantityRole:
        item.quantity = value.toInt();
        break;
    case PoleRole:
        item.quantity = value.toInt();
        break;
    }

    if (m_list->setItemAt(index.row(), item)) {
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }
    return false;
}

Qt::ItemFlags OptionsModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable;
}

QHash<int, QByteArray> OptionsModel::roleNames() const
{
    QHash<int, QByteArray> names;
    names[UsernameRole] = "username";
    names[WeightRole] = "weight";
    names[UnitRole] = "unit";
    names[QuantityRole] = "quantity";
    names[PoleRole] = "poles";
    return names;
}

OptionsHandler *OptionsModel::list() const
{
    return m_list;
}

void OptionsModel::setList(OptionsHandler *list)
{
    beginResetModel();

    if(m_list)
        m_list->disconnect(this);

    m_list = list;

    if ( m_list ) {
        connect(m_list, &OptionsHandler::beforeAdd, this, [this] {
            const int index = m_list->users().size();
            beginInsertRows(QModelIndex(), index, index);
        });

        connect(m_list, &OptionsHandler::afterAdd, this, [this] {
            endInsertRows();
        });

        connect(m_list, &OptionsHandler::beforeRemoved, this, [this](int index) {
            beginRemoveRows(QModelIndex(), index, index);
        });

        connect(m_list, &OptionsHandler::afterRemoved, this, [this] {
            endRemoveRows();
        });
    }

    endResetModel();
}
