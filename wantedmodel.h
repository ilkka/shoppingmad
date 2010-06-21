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

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index = QModelIndex(), int role = Qt::DisplayRole) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation = Qt::Horizontal, int role = Qt::DisplayRole) const;

    enum Role {
        LabelRole = Qt::UserRole,
        QuantityRole
    };

signals:

public slots:
    void addItem(const QString& text);

private:
    friend class WantedModelPrivate;
    WantedModelPrivate* const d;
};

#endif // WANTEDMODEL_H
