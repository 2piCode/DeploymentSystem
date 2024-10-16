#ifndef CONFIG_WRITER_H
#define CONFIG_WRITER_H

#include <filesystem>

#include "config.h"

class ConfigWriter {
   public:
    virtual ~ConfigWriter() = default;
    virtual void WriteInFile(const std::unique_ptr<Config>& config,
                             const std::filesystem::path& path,
                             bool is_save_password = false) const = 0;
    virtual std::unique_ptr<Config> ReadFromFile(
        const std::filesystem::path& path) = 0;
};

#endif
