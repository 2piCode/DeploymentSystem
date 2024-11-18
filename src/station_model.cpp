#include "station_model.h"
#include <iostream>

StationModel::StationModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

void StationModel::addStation(const QString &hostName,
                               const QString &name,
                               const QString &description,
                               Roles::Role role,
                               const QString &username,
                               const QString &password,
                               quint16 port,
                               const QString &filePath)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());

    // Create ConnectionSettings object
    ConnectionSettings settings;
    settings.username = username;
    settings.password = password;
    settings.port = port;
    if (!filePath.isEmpty()) {
        settings.path_to_private_key = std::filesystem::path(filePath.toStdString());
    }

    Station *station = new Station(hostName, name, settings, this);
    station->SetDescription(description);
    station->SetRole(role);
    m_stations << station;

    endInsertRows();
}

void StationModel::removeStation(int index)
{
    if (index < 0 || index >= m_stations.count())
        return;

    beginRemoveRows(QModelIndex(), index, index);
    Station *station = m_stations.takeAt(index);
    station->deleteLater();
    endRemoveRows();
}

bool StationModel::checkConnection(int index){
    if (index < 0 || index >= m_stations.count()){
        return false;
    }
    
    Station *station = m_stations[index];
    return station->CheckConnection();
}

void StationModel::StartSetupProccess(int index){
    if (index < 0 || index >= m_stations.count()){
        return;
    }
    Station *station = m_stations[index];
    station->StartSetupProccess();
    return;
}


int StationModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_stations.count();
}

QVariant StationModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= m_stations.count())
        return QVariant();

    Station *station = m_stations.at(index.row());
    switch (role) {
    case HostNameRole:
        return station->GetHostName();
    case NameRole:
        return station->GetName();
    case DescriptionRole:
        return station->GetDescription();
    case RoleRole:
        return static_cast<int>(station->GetRole());
    case UsernameRole:
        return station->GetUsername();
    case PasswordRole:
        return station->GetPassword();
    case PortRole:
        return station->GetConnectionPort();
    case FilePathRole:
        return station->GetUrlPath();
    case StationRole:
        return QVariant::fromValue(station);
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> StationModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[HostNameRole] = "hostName";
    roles[NameRole] = "name";
    roles[DescriptionRole] = "description";
    roles[RoleRole] = "role";
    roles[UsernameRole] = "username";
    roles[PasswordRole] = "password";
    roles[PortRole] = "port";
    roles[FilePathRole] = "filePath";
    roles[StationRole] = "station";
    return roles;
}
