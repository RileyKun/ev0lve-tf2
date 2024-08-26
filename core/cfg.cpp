//
// Created by panzerfaust on 5/10/2020.
//

#include <core/cfg.h>

/*
 * WARNING! DO NOT USE COMPLEX TYPES WITH THIS CONFIG SYSTEM!
 *
 * CHEAT SHEET
 * ================================
 * HEADER:
 * magic - Magic number, identifying that THIS IS valid (hopefully) config file.
 * cheat_id - Hash of cheat name. Might be used to identify user that config is meant for another cheat
 * version - Version of config file. Might be used if some config_values did change type but have the same name
 *
 * DATA:
 * hash - Hash of config_value name
 * size - Size of data
 * data - Raw value
 *
 * IN FILE:
 * [HEADER][DATA 1][DATA 2][DATA 3][DATA N]
 */

cfg::config_result cfg::config_system::save(const std::string &filename) {
    auto file = path + "/" + filename;

    size_t total_size = 10;                     // [magic=2][cheat_id=4][version=4]
    for (auto& entry : storage)
        total_size += 8 + entry.second.size;    // [hash=4][size=4][data=size]

    auto buffer = (char*)malloc(total_size);
    memcpy(buffer, &magic, 2);
    memcpy(buffer + 2, &cheat_id, 4);
    memcpy(buffer + 6, &version, 4);

    auto offset = 10;
    for (auto& entry : storage) {
        memcpy(buffer + offset, &entry.first, 4);
        memcpy(buffer + offset + 4, &entry.second.size, 4);
        memcpy(buffer + offset + 8, entry.second.mem, entry.second.size);

        offset += 8 + entry.second.size;
    }

    std::ofstream writer(file, std::ios::binary);
    if (!writer.is_open())
        return config_result::ERROR_BAD_FILE;

    writer.write(buffer, total_size);
    writer.close();

    return config_result::OK;
}

cfg::config_result cfg::config_system::load(const std::string &filename, bool should_reset) {
    if (should_reset)
        reset();

    auto file = path + "/" + filename;

    std::ifstream reader(file, std::ios::binary);
    if (!reader.is_open())
        return config_result::ERROR_BAD_FILE;

    reader.seekg(0, std::ifstream::end);
    auto size = reader.tellg();
    reader.seekg(0);

    // Config file must have at least header
    if (size < 10) {
        reader.close();
        return config_result::ERROR_BAD_FILE;
    }

    char cfg_magic[2]{};
    reader.read(cfg_magic, 2);

    // Definitely not config file
    if (memcmp(cfg_magic, &magic, 2) != 0) {
        reader.close();
        return config_result::ERROR_BAD_MAGIC;
    }

    char cfg_cheat_id[4]{};
    reader.read(cfg_cheat_id, 4);

    // Wrong cheat ID
    if (memcmp(cfg_cheat_id, &cheat_id, 4) != 0) {
        reader.close();
        return config_result::ERROR_BAD_CHEAT_ID;
    }

    char cfg_version[4]{};
    reader.read(cfg_version, 4);

    // Wrong version
    if (memcmp(cfg_version, &version, 4) != 0) {
        reader.close();
        return config_result::ERROR_BAD_VERSION;
    }

    auto read = 10;
    while (read < size) {
        rage::joaat_t hash{};
        size_t data_size{};

        reader.read(reinterpret_cast<char*>(&hash), 4);
        reader.read(reinterpret_cast<char*>(&data_size), 4);

        char data[64]{};
        reader.read(data, data_size);
        storage[hash].set(data, data_size);

        read += 8 + data_size;
    }

    reader.close();
    return config_result::OK;
}

void cfg::config_system::reset() {
    for (auto& entry : storage)
        entry.second.reset();
}

std::vector<std::string> &cfg::config_system::refresh(const std::string& ext) {
    files.clear();

    for (auto& d : std::filesystem::directory_iterator(path)) {
        auto& f = d.path();
        if (f.extension().string() == ext)
            files.push_back(f.filename().string());
    }

    return files;
}

void cfg::config_system::destroy() {
    for (auto& entry : storage)
        entry.second.invalidate();
    storage.clear();
}

