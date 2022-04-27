#include "FileUtils.h"
#include "utils/FileReader.h"
#include "utils/FileReaderCompressed.h"
#include "utils/utils.h"
#include <map>
#include <wuhb_utils/utils.h>
#include <wums/exports.h>

std::forward_list<std::unique_ptr<FileReader>> openFiles;
std::map<std::string, std::string> mountedWUHB;
std::mutex mutex;

void WUHBUtils_CleanUp() {
    std::lock_guard<std::mutex> lock(mutex);
    openFiles.clear();

    for (const auto &[name, path] : mountedWUHB) {
        romfsUnmount(name.c_str());
    }
    mountedWUHB.clear();
}

WUHBUtilsApiErrorType WUU_MountBundle(const char *name, const char *path, BundleSource source, int32_t *outRes) {
    if (!name || !path || (source != BundleSource_FileDescriptor && source != BundleSource_FileDescriptor_CafeOS)) {
        return WUHB_UTILS_API_ERROR_INVALID_ARG;
    }
    std::lock_guard<std::mutex> lock(mutex);
    for (const auto &[key, value] : mountedWUHB) {
        if (key == name) {
            if (value == path) {
                *outRes = 0;
                return WUHB_UTILS_API_ERROR_NONE;
            }
            return WUHB_UTILS_API_ERROR_MOUNT_NAME_TAKEN;
        }
    }

    auto res = romfsMount(name, path, (RomfsSource) source);
    if (res == 0) {
        mountedWUHB[name] = path;
    }

    *outRes = res;

    return WUHB_UTILS_API_ERROR_NONE;
}

WUHBUtilsApiErrorType WUU_UnmountBundle(const char *name, int32_t *outRes) {
    if (!name) {
        return WUHB_UTILS_API_ERROR_INVALID_ARG;
    }
    std::lock_guard<std::mutex> lock(mutex);
    if (mountedWUHB.count(name) > 0) {
        auto res = romfsUnmount(name);
        if (outRes) {
            *outRes = res;
        }
        mountedWUHB.erase(name);
        return WUHB_UTILS_API_ERROR_NONE;
    }
    return WUHB_UTILS_API_ERROR_MOUNT_NOT_FOUND;
}

WUHBUtilsApiErrorType WUU_FileOpen(const char *name, uint32_t *outHandle) {
    if (!outHandle || !name) {
        return WUHB_UTILS_API_ERROR_INVALID_ARG;
    }
    std::lock_guard<std::mutex> lock(mutex);
    std::unique_ptr<FileReader> reader;
    std::string path   = std::string(name);
    std::string pathGZ = path + ".gz";

    if (CheckFile(path.c_str())) {
        reader = make_unique_nothrow<FileReader>(path);
    } else if (CheckFile(pathGZ.c_str())) {
        reader = make_unique_nothrow<FileReaderCompressed>(pathGZ);
    } else {
        return WUHB_UTILS_API_ERROR_FILE_NOT_FOUND;
    }

    if (!reader || !reader->isReady()) {
        return WUHB_UTILS_API_ERROR_NO_MEMORY;
    }
    *outHandle = reader->getHandle();
    openFiles.push_front(std::move(reader));

    return WUHB_UTILS_API_ERROR_NONE;
}

WUHBUtilsApiErrorType WUU_FileRead(uint32_t handle, uint8_t *buffer, uint32_t size, int32_t *outRes) {
    if (!buffer | !outRes) {
        return WUHB_UTILS_API_ERROR_INVALID_ARG;
    }
    std::lock_guard<std::mutex> lock(mutex);
    for (auto &reader : openFiles) {
        if ((uint32_t) reader.get() == (uint32_t) handle) {
            *outRes = (int32_t) reader->read(buffer, size);
            return WUHB_UTILS_API_ERROR_NONE;
        }
    }

    return WUHB_UTILS_API_ERROR_FILE_HANDLE_NOT_FOUND;
}

WUHBUtilsApiErrorType WUU_FileClose(uint32_t handle) {
    if (remove_locked_first_if(mutex, openFiles, [handle](auto &cur) { return cur->getHandle() == handle; })) {
        return WUHB_UTILS_API_ERROR_NONE;
    }

    return WUHB_UTILS_API_ERROR_FILE_HANDLE_NOT_FOUND;
}

WUHBUtilsApiErrorType WUU_FileExists(const char *name, int32_t *outRes) {
    if (!outRes || !name) {
        return WUHB_UTILS_API_ERROR_INVALID_ARG;
    }
    std::lock_guard<std::mutex> lock(mutex);
    std::string checkgz = std::string(name) + ".gz";
    *outRes             = CheckFile(name) || CheckFile(checkgz.c_str());
    return WUHB_UTILS_API_ERROR_NONE;
}

WUHBUtilsApiErrorType WUU_GetRPXInfo(const char *path, BundleSource source, WUHBRPXInfo *outFileInfo) {
    if (!outFileInfo || !path || (source != BundleSource_FileDescriptor && source != BundleSource_FileDescriptor_CafeOS)) {
        return WUHB_UTILS_API_ERROR_INVALID_ARG;
    }
    std::lock_guard<std::mutex> lock(mutex);

    auto res = getRPXInfoForPath(path, source, outFileInfo);

    if (res == WUHB_UTILS_RPX_INFO_MOUNT_FAILED) {
        return WUHB_UTILS_API_ERROR_MOUNT_FAILED;
    } else if (res == WUHB_UTILS_RPX_INFO_NO_RPX_FOUND || res == WUHB_UTILS_RPX_INFO_OPENDIR_FAILED) {
        return WUHB_UTILS_API_ERROR_FILE_NOT_FOUND;
    }

    return WUHB_UTILS_API_ERROR_NONE;
}

uint32_t WUU_GetVersion() {
    return WUHB_UTILS_MODULE_VERSION;
}

WUMS_EXPORT_FUNCTION(WUU_MountBundle);
WUMS_EXPORT_FUNCTION(WUU_UnmountBundle);
WUMS_EXPORT_FUNCTION(WUU_FileOpen);
WUMS_EXPORT_FUNCTION(WUU_FileRead);
WUMS_EXPORT_FUNCTION(WUU_FileClose);
WUMS_EXPORT_FUNCTION(WUU_FileExists);
WUMS_EXPORT_FUNCTION(WUU_GetRPXInfo);
WUMS_EXPORT_FUNCTION(WUU_GetVersion);