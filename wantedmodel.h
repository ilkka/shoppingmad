#ifndef WANTEDMODEL_H
#define WANTEDMODEL_H

#include <QAbstractListModel>

class WantedModelPrivate;

class WantedModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit WantedModel(QObject *parent = 0);
    virtual ~WantedModel();

    virtual int rowCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    enum Role {
        LabelRole = Qt::UserRole,
        QuantityRole
    };

signals:

public slots:

private:
    friend class WantedModelPrivate;
    WantedModelPrivate* const d;
};

#endif // WANTEDMODEL_H
