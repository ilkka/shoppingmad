#include "wantedmodel.h"

#include <QList>
#include <QHash>
#include <QByteArray>

#include "logging.h"

class WantedModelPrivate {
public:
    QList<QString> things;
    QHash<QString, int> amounts;
};

WantedModel::WantedModel(QObject *parent) :
    QAbstractListModel(parent), d(new WantedModelPrivate)
{
    QHash<int, QByteArray> rolenames;
    rolenames[LabelRole] = "label";
    rolenames[QuantityRole] = "quantity";
    setRoleNames(rolenames);
    d->things.append("Milk");
    d->things.append("Cookies");
    Q_FOREACH(const QString& key, d->things) {
        d->amounts.insert(key, 1);
    }
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
    return d->things.size();
}

QVariant WantedModel::data(const QModelIndex &index, int role) const
{
    if (index.isValid() && !index.parent().isValid()) {
        QString key = d->things.at(index.row());
        LOG_TRACE("Key for " << index.row() << "," << index.column() << ": " << key);
        if (role == LabelRole) {
            return QVariant(key);
        } else if (role == QuantityRole) {
            return QVariant(d->amounts.value(key));
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
    if (d->things.contains(text)) {
        LOG_TRACE("Item already in things, increase amount");
        d->amounts.insert(text, d->amounts.value(text) + 1);
        emit dataChanged(index(d->things.indexOf(text)), index(d->things.indexOf(text)));
    } else {
        LOG_TRACE("New item, insert");
        beginInsertRows(QModelIndex(), d->things.size(), d->things.size());
        d->things.append(text);
        d->amounts.insert(text, 1);
        endInsertRows();
    }
}
