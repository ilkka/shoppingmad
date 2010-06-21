#ifndef WANTEDMODEL_H
#define WANTEDMODEL_H

#include <QSqlTableModel>

class WantedModelPrivate;

class WantedModel : public QSqlTableModel
{
    Q_OBJECT
public:
    /*! Create a wantedmodel instance
      */
    static WantedModel* create(QObject* parent = 0);
    virtual ~WantedModel();

    virtual QVariant data(const QModelIndex &index = QModelIndex(), int role = Qt::DisplayRole) const;

    enum Role {
        LabelRole = Qt::UserRole,
        QuantityRole
    };

signals:

public slots:
    void addItem(const QString& text);

private:
    explicit WantedModel(QObject *parent = 0);

    friend class WantedModelPrivate;
    WantedModelPrivate* const d;
};

#endif // WANTEDMODEL_H
