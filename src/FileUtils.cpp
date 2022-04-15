#include "FileUtils.h"
#include "export.h"
#include "utils/logger.h"
#include <dirent.h>
#include <sys/stat.h>
#include <wuhb_utils/utils.h>

int32_t CheckFile(const char *filepath) {
    if (!filepath) {
        return 0;
    }

    struct stat filestat {};

    char dirnoslash[strlen(filepath) + 2];
    snprintf(dirnoslash, sizeof(dirnoslash), "%s", filepath);

    while (dirnoslash[strlen(dirnoslash) - 1] == '/') {
        dirnoslash[strlen(dirnoslash) - 1] = '\0';
    }

    char *notRoot = strrchr(dirnoslash, '/');
    if (!notRoot) {
        strcat(dirnoslash, "/");
    }

    if (stat(dirnoslash, &filestat) == 0) {
        return 1;
    }

    return 0;
}

WUHBRPXInfoResultCode getRPXInfoForPath(const std::string &path, BundleSource source, WUHBRPXInfo *outFileInfo) {
    auto realSource = RomfsSource_FileDescriptor;
    switch (source) {
        case BundleSource_FileDescriptor:
            realSource = RomfsSource_FileDescriptor;
            break;
        case BundleSource_FileDescriptor_CafeOS:
            realSource = RomfsSource_FileDescriptor_CafeOS;
            break;
    }
    if (romfsMount("wuu_rpxinfo", path.c_str(), realSource) < 0) {
        return WUHB_UTILS_RPX_INFO_MOUNT_FAILED;
    }
    DIR *dir;
    struct dirent *entry;

    if (!(dir = opendir("wuu_rpxinfo:/code/"))) {
        romfsUnmount("wuu_rpxinfo");
        return WUHB_UTILS_RPX_INFO_OPENDIR_FAILED;
    }
    bool found = false;
    WUHBRPXInfoResultCode res;
    while ((entry = readdir(dir)) != nullptr) {
        if (std::string_view(entry->d_name).ends_with(".rpx")) {
            romfs_fileInfo info;
            if (romfsGetFileInfoPerPath("wuu_rpxinfo", (std::string("code/") + entry->d_name).c_str(), &info) >= 0) {
                found               = true;
                res                 = WUHB_UTILS_RPX_INFO_SUCCESS;
                outFileInfo->length = info.length;
                outFileInfo->offset = info.offset;
            } else {
                DEBUG_FUNCTION_LINE_ERR("Failed to get info for %s", entry->d_name);
            }
            break;
        }
    }

    closedir(dir);

    romfsUnmount("wuu_rpxinfo");

    if (!found) {
        return WUHB_UTILS_RPX_INFO_NO_RPX_FOUND;
    }
    return res;
}