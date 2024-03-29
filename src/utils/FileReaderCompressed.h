#pragma once

#include "FileReader.h"
#include "logger.h"
#include <zlib.h>

#define BUFFER_SIZE 0x20000

class FileReaderCompressed : public FileReader {
public:
    [[maybe_unused]] FileReaderCompressed(uint8_t *buffer, uint32_t size);

    explicit FileReaderCompressed(std::string &file);

    ~FileReaderCompressed() override = default;

    int64_t read(uint8_t *buffer, uint32_t size) override;

private:
    bool initDone = false;
    alignas(0x40) uint8_t zlib_in_buf[BUFFER_SIZE]{};
    z_stream strm{};

    void initCompressedData();
};
