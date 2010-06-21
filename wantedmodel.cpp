#include "wantedmodel.h"

#include <QList>
#include <QHash>
#include <QByteArray>
#include <QSqlDatabase>
#include <QSqlError>
#include <QStringList>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QModelIndex>

#include <stdexcept>

#include "logging.h"

class WantedModelPrivate {
public:
    QSqlTableModel* tablemodel;
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
     * @note This does not seem to work properly in sqlite3.
     *
     * @return true if required tables are missing, false otherwise.
     */
    bool databaseTablesMissing() {
        LOG_DEBUG("Database tables: " << QSqlDatabase::database().tables());
        return !QSqlDatabase::database().tables().contains(WANTED_ITEMS_TABLE_NAME.toUpper());
    }

    /*! Create database tables required by application.
     */
    void createDatabaseTables() {
        LOG_DEBUG("Creating database tables, ignoring errors");
        QSqlQuery q("CREATE TABLE WANTED_ITEMS(name VARCHAR(128) NOT NULL PRIMARY KEY, amount INTEGER)");
        q.exec();
    }

    /*! Check whether the given item exists in the wanted list.
      * @param item name of the item.
      * @return true if item exists in the default DB, false otherwise.
      */
    bool existsInWantedItems(const QString& item) {
        QSqlQuery q(QString("select count() from %1 where name='%2'").arg(WANTED_ITEMS_TABLE_NAME).arg(item));
        if (q.exec()) {
            q.next();
            if (q.value(0).toInt() > 0) {
                return true;
            }
            return false;
        } else {
            LOG_ERROR("query error: " << q.lastError());
            throw std::runtime_error(QString("Query error: %1").arg(q.lastError().text()).toStdString());
        }
    }

    /*! Increment wanted amount for given item by one.
      * @param item item name.
      */
    void incrementAmountForItem(const QString& item) {
        QSqlQuery q1(QString("select amount from %1 where name='%2'").arg(WANTED_ITEMS_TABLE_NAME).arg(item));
        if (q1.exec()) {
            q1.next();
            QSqlQuery q2(QString("update %1 set amount=%2 where name='%3'").arg(WANTED_ITEMS_TABLE_NAME).arg(q1.value(0).toInt() + 1).arg(item));
            if (!q2.exec()) {
                LOG_ERROR("Failed to update amount of " << item << ": " << q2.lastError());
            }
        } else {
            LOG_ERROR("Failed to fetch original amount of " << item << ": " << q1.lastError());
        }
    }

    /*! Insert new item into wanted items list.
      * @param item item name.
      */
    void addToWantedItems(const QString& item) {
        QSqlQuery q(QString("insert into %1 (name, amount) values ('%2', 1)").arg(WANTED_ITEMS_TABLE_NAME).arg(item));
        if (!q.exec()) {
            LOG_ERROR("Failed to insert new item " << item << ": " << q.lastError());
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
    d->tablemodel = new QSqlTableModel(this);
    d->tablemodel->setTable(WANTED_ITEMS_TABLE_NAME);
    d->tablemodel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    d->tablemodel->setHeaderData(0, Qt::Horizontal, "Label");
    d->tablemodel->setHeaderData(1, Qt::Horizontal, "Quantity");
    d->tablemodel->select();
}

WantedModel::~WantedModel()
{
    QSqlDatabase::database().close();
    QSqlDatabase::removeDatabase(QSqlDatabase::database().connectionName());
    delete d;
}

int WantedModel::rowCount(const QModelIndex &parent) const
{
    return d->tablemodel->rowCount(parent);
}

QVariant WantedModel::data(const QModelIndex &idx, int role) const
{
    if (idx.isValid() && !idx.parent().isValid()) {
        LOG_TRACE("Get data for " << idx);
        if (role == LabelRole) {
            QModelIndex dbidx = d->tablemodel->index(idx.row(), 0);
            LOG_TRACE("Label data: " << dbidx);
            return d->tablemodel->data(dbidx);
        } else if (role == QuantityRole) {
            QModelIndex dbidx = d->tablemodel->index(idx.row(), 1);
            LOG_TRACE("Quantity data: " << dbidx);
            return d->tablemodel->data(dbidx);
        }
    }
    return QVariant();
}

QVariant WantedModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return d->tablemodel->headerData(section, orientation, role);
}

void WantedModel::addItem(const QString &text)
{
    LOG_DEBUG("Add item " << text);
    if (existsInWantedItems(text)) {
        LOG_TRACE("Item already in things, increase amount");
        incrementAmountForItem(text);
    } else {
        LOG_TRACE("New item, insert");
        addToWantedItems(text);
    }
}
