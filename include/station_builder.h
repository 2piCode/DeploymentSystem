#ifndef STATION_BUILDER_H
#define STATION_BUILDER_H

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <memory>

#include "ssh_connection.h"
#include "station.h"

class StationBuilder : public QObject {
    Q_OBJECT
    Q_PROPERTY(QList<Station*> childStations READ GetChildStations NOTIFY childStationsChanged)
   public:
    explicit StationBuilder(std::shared_ptr<MainStation> main_station,
                            QObject* parent = nullptr)
        : QObject(parent), main_station_(main_station) {}

    Q_INVOKABLE const std::shared_ptr<MainStation>& GetMainStation() const {
        return main_station_;
    }
    Q_INVOKABLE std::shared_ptr<MainStation>& GetMainStation() { return main_station_; }

    Q_INVOKABLE Station* CreateStation(const QString& hostName,
                                       const QString& name,
                                       ConnectionSettings settings = {}) {
        main_station_->AddChildStation(
            std::make_unique<Station>(hostName, name, settings));
        emit childStationsChanged();
        return main_station_->GetChildStations().back().get();
    }

    Q_INVOKABLE Station* GetStation(int index = 0) {
        if (main_station_ == nullptr || index < 0) {
            return nullptr;
        }

        if (index == 0) {
            return main_station_.get();
        }

        if (index > main_station_->GetChildStations().size()) {
            return nullptr;
        }

        return main_station_->GetChildStations().at(index - 1).get();
    }

    Q_INVOKABLE QList<Station*> GetChildStations() {
        QList<Station*> child_stations_;

        for (auto& station : main_station_->GetChildStations())
            child_stations_.push_back(station.get());

        return child_stations_;
    }

    Q_INVOKABLE void RemoveStation(int index) {
        if (main_station_ == nullptr || index < 0) {
            return;
        }

        if (index > main_station_->GetChildStations().size()) {
            return;
        }

        main_station_->RemoveChildStation(index);
        emit childStationsChanged();
    }

   signals:
    void childStationsChanged();
   private:
    std::shared_ptr<MainStation> main_station_;
};

#endif
