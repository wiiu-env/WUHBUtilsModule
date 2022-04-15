#pragma once
#include "export.h"
#include <coreinit/filesystem.h>
#include <functional>
#include <romfs_dev.h>
#include <string>
#include <wuhb_utils/utils.h>

WUHBRPXInfoResultCode getRPXInfoForPath(const std::string &path, BundleSource source, WUHBRPXInfo *outFileInfo);

int32_t CheckFile(const char *filepath);