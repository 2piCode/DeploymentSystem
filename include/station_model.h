#ifndef STATION_MODEL_H
#define STATION_MODEL_H

#include <QAbstractListModel>
#include "station.h"


class StationModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum StationRoles {
        HostNameRole = Qt::UserRole + 1,
        NameRole,
        DescriptionRole,
        RoleRole,
        UsernameRole,
        PasswordRole,
        PortRole,
        FilePathRole,
        StationRole
    };
    Q_ENUM(StationRoles)

    explicit StationModel(QObject *parent = nullptr);

    Q_INVOKABLE void addStation(const QString &hostName,
                            const QString &name,
                            const QString &description,
                            Roles::Role role,
                            const QString &username = QString(),
                            const QString &password = QString(),
                            quint16 port = 22,
                            const QString &filePath = QString());

    Q_INVOKABLE void removeStation(int index);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    QList<Station*> m_stations;
};

#endif // STATION_MODEL_H
