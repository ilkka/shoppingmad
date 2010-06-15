#include "wantedmodel.h"

#include <QList>
#include <QHash>
#include <QByteArray>
#include <QSqlDatabase>
#include <QSqlError>
#include <QStringList>
#include <QSqlQuery>

#include <stdexcept>

#include "logging.h"

class WantedModelPrivate {
public:
    QList<QString> things;
    QHash<QString, int> amounts;
};

namespace {
    /*! Name of database file.
     */
    const QString WANTED_ITEMS_DB_NAME = "shoppingmad.sqlite";

    /*! Name of wanted items table in database.
     */
    const QString WANTED_ITEMS_TABLE_NAME = "wanted_items";

    /*! Open database connection.
     *
     * This function opens the database connection and creates the DB
     * if necessary.
     */
    void openDatabaseConnection() {
        LOG_DEBUG("Opening database connection");
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(WANTED_ITEMS_DB_NAME);
        db.open();
        if (!db.isOpen()) {
            LOG_ERROR("Database not open, last error " << db.lastError());
            throw std::runtime_error("Could not open database");
        }
    }

    /*! Check for required database tables.
     *
     * @return true if required tables are missing, false otherwise.
     */
    bool databaseTablesMissing() {
       return !QSqlDatabase().tables().contains(WANTED_ITEMS_TABLE_NAME);
    }

    /*! Create database tables required by application.
     */
    void createDatabaseTables() {
        LOG_DEBUG("Creating database tables");
        QSqlQuery q("CREATE TABLE WANTED_ITEMS(name VARCHAR(128) NOT NULL PRIMARY KEY, amount INTEGER)");
        if (!q.exec()) {
            LOG_ERROR("Could not create tables: " << q.lastError());
            throw std::runtime_error("Could not create database tables");
        }
    }
}

WantedModel::WantedModel(QObject *parent) :
    QAbstractListModel(parent), d(new WantedModelPrivate)
{
    openDatabaseConnection();
    if (databaseTablesMissing()) {
        createDatabaseTables();
    }
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
