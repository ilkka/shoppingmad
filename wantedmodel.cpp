#include "wantedmodel.h"

#include <QList>
#include <QHash>
#include <QByteArray>
#include <QSqlDatabase>
#include <QSqlError>
#include <QStringList>
#include <QSqlQuery>
#include <QModelIndex>

#include <stdexcept>

#include "logging.h"

class WantedModelPrivate {
public:
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
        QSqlQuery q("CREATE TABLE WANTED_ITEMS(label VARCHAR(128) NOT NULL PRIMARY KEY, quantity INTEGER)");
        q.exec();
    }

    /*! Check whether the given item exists in the wanted list.
      * @param item name of the item.
      * @return true if item exists in the default DB, false otherwise.
      */
    bool existsInWantedItems(const QString& item) {
        QSqlQuery q(QString("select count() from %1 where label='%2'").arg(WANTED_ITEMS_TABLE_NAME).arg(item));
        if (q.exec()) {
            q.next();
            if (q.value(0).toInt() > 0) {
                return true;
            }
            return false;
        } else {
            LOG_ERROR("query error: " << q.lastError());
            return false;
        }
    }

    /*! Increment wanted quantity for given item by one.
      * @param item item name.
      */
    void incrementQuantityForItem(const QString& item) {
        QSqlQuery q1(QString("select quantity from %1 where label='%2'").arg(WANTED_ITEMS_TABLE_NAME).arg(item));
        if (q1.exec()) {
            q1.next();
            QSqlQuery q2(QString("update %1 set quantity=%2 where label='%3'").arg(WANTED_ITEMS_TABLE_NAME).arg(q1.value(0).toInt() + 1).arg(item));
            if (!q2.exec()) {
                LOG_ERROR("Failed to update quantity of " << item << ": " << q2.lastError());
            }
        } else {
            LOG_ERROR("Failed to fetch original quantity of " << item << ": " << q1.lastError());
        }
    }

    /*! Insert new item into wanted items list.
      * @param item item name.
      */
    void addToWantedItems(const QString& item) {
        QSqlQuery q(QString("insert into %1 (label, quantity) values ('%2', 1)").arg(WANTED_ITEMS_TABLE_NAME).arg(item));
        if (!q.exec()) {
            LOG_ERROR("Failed to insert new item " << item << ": query " << q.lastQuery() << " failed with error " << q.lastError());
        }
    }
}

WantedModel* WantedModel::create(QObject *parent)
{
    if (!QSqlDatabase::database().isOpen()) {
        openDatabaseConnection();
        if (databaseTablesMissing()) {
            createDatabaseTables();
        }
    }
    return new WantedModel(parent);
}

WantedModel::WantedModel(QObject *parent) :
    QSqlTableModel(parent), d(new WantedModelPrivate)
{
    QHash<int, QByteArray> rolenames;
    rolenames[LabelRole] = "label";
    rolenames[QuantityRole] = "quantity";
    setRoleNames(rolenames);
    setTable(WANTED_ITEMS_TABLE_NAME);
    setEditStrategy(QSqlTableModel::OnManualSubmit);
    setHeaderData(0, Qt::Horizontal, "Label");
    setHeaderData(1, Qt::Horizontal, "Quantity");
    select();
}

WantedModel::~WantedModel()
{
    QSqlDatabase::database().close();
    QSqlDatabase::removeDatabase(QSqlDatabase::database().connectionName());
    delete d;
}

QVariant WantedModel::data(const QModelIndex &idx, int role) const
{
    if (idx.isValid() && !idx.parent().isValid()) {
        LOG_TRACE("Get data for " << idx);
        if (role == LabelRole) {
            QModelIndex dbidx = index(idx.row(), 0);
            LOG_TRACE("Label data: " << dbidx);
            return QSqlTableModel::data(dbidx);
        } else if (role == QuantityRole) {
            QModelIndex dbidx = index(idx.row(), 1);
            LOG_TRACE("Quantity data: " << dbidx);
            return QSqlTableModel::data(dbidx);
        }
    }
    return QVariant();
}

void WantedModel::addItem(const QString &text)
{
    LOG_DEBUG("Add item " << text);
    if (existsInWantedItems(text)) {
        LOG_TRACE("Item already in things, increase quantity");
        beginInsertRows(QModelIndex(), rowCount(), rowCount());
        incrementQuantityForItem(text);
        select();
        endInsertRows();
    } else {
        LOG_TRACE("New item, insert");
        addToWantedItems(text);
    }
}
