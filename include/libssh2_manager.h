#ifndef LIBSSH2_MANAGER_H
#define LIBSSH2_MANAGER_H

#include <libssh2.h>

#include <QObject>

class LibSSH2Manager final : public QObject {
    Q_OBJECT
   public:
    static LibSSH2Manager& instance() {
        static LibSSH2Manager instance;
        return instance;
    }

    bool initialize() {
        if (!is_initialize) {
            if (libssh2_init(0) != 0) {
                emit errorOccurred("libssh2 initialization failed");
                return false;
            }
            is_initialize = true;
        }
        return true;
    }

   signals:
    void errorOccurred(const QString& errorMessage);

   private:
    LibSSH2Manager() = default;
    ~LibSSH2Manager() final { cleanup(); }

    LibSSH2Manager(const LibSSH2Manager&) = delete;
    LibSSH2Manager& operator=(const LibSSH2Manager&) = delete;
    LibSSH2Manager(LibSSH2Manager&&) = delete;
    LibSSH2Manager& operator=(LibSSH2Manager&&) = delete;

    bool is_initialize = false;

    void cleanup() {
        if (is_initialize) {
            libssh2_exit();
            is_initialize = false;
        }
    }
};

#endif
