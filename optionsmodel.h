#ifndef OPTIONSMODEL_H
#define OPTIONSMODEL_H

#include <QAbstractListModel>

class OptionsHandler;

class OptionsModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(OptionsHandler *list READ list WRITE setList)

public:
    explicit OptionsModel(QObject *parent = nullptr);

    enum {
        UsernameRole,
        WeightRole,
        UnitRole,
        QuantityRole,
        PoleRole
    };

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    virtual QHash<int, QByteArray> roleNames() const override;

    OptionsHandler *list() const;
    void setList(OptionsHandler *list);

private:
    OptionsHandler *m_list;
};

#endif // OPTIONSMODEL_H
