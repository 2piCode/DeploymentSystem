#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <memory>

#include "ssh_connection.h"
#include "station.h"

class StationBuilder : public QObject {
    Q_OBJECT
   public:
    explicit StationBuilder(QObject* parent = nullptr) : QObject(parent) {}

    const std::unique_ptr<MainStation>& GetMainStation() const {
        return main_station_;
    }
    std::unique_ptr<MainStation>& GetMainStation() { return main_station_; }

    MainStation* ReleaseMainStation() { return main_station_.release(); }

    Q_INVOKABLE Station* CreateStation(const QString& hostName,
                                       const QString& name,
                                       ConnectionSettings settings = {}) {
        if (main_station_ == nullptr) {
            main_station_ =
                std::make_unique<MainStation>(hostName, name, settings);
            return main_station_.get();
        }

        main_station_->AddChildStation(
            std::make_unique<Station>(hostName, name, settings));

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

    Q_INVOKABLE void RemoveStation(int index) {
        if (main_station_ == nullptr || index < 0) {
            return;
        }

        if (index == 0) {
            main_station_.reset();
            return;
        }

        if (index > main_station_->GetChildStations().size()) {
            return;
        }

        main_station_->RemoveChildStation(index - 1);
    }

   private:
    std::unique_ptr<MainStation> main_station_;
};
