#include "wantedmodel.h"

#include <QHash>
#include <QByteArray>

#include "logging.h"

class WantedModelPrivate {
public:
    QHash<QString, int> items;
};

WantedModel::WantedModel(QObject *parent) :
    QAbstractListModel(parent), d(new WantedModelPrivate)
{
    QHash<int, QByteArray> rolenames;
    rolenames[LabelRole] = "label";
    rolenames[QuantityRole] = "quantity";
    setRoleNames(rolenames);
    d->items.insert("Milk", 1);
    d->items.insert("Cookies", 1);
}

WantedModel::~WantedModel()
{
    delete d;
}

int WantedModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0;
    }
    return d->items.size();
}

QVariant WantedModel::data(const QModelIndex &index, int role) const
{
    if (index.isValid()) {
        if (role == LabelRole) {
            return QVariant(d->items.key(index.row()));
        } else if (role == QuantityRole) {
            return QVariant(d->items.value(d->items.key(index.row())));
        }
    }
    return QVariant();
}

QVariant WantedModel::headerData(int section, Qt::Orientation /*orientation*/, int role) const
{
    if (role == Qt::DisplayRole) {
        if (section == 0) {
            return QVariant("Label");
        } else if (section == 1) {
            return QVariant("Quantity");
        }
    }
    return QVariant();
}

void WantedModel::addItem(const QString &text)
{
    LOG_DEBUG("Add item " << text);
//    beginInsertRows(QModelIndex(), d->items.size(), d->items.size());
//    d->items.append(text);
    endInsertRows();
}
