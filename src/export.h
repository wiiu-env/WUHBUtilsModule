#pragma once

typedef enum WUHBRPXInfoResultCode {
    WUHB_UTILS_RPX_INFO_SUCCESS        = 0,
    WUHB_UTILS_RPX_INFO_MOUNT_FAILED   = -1,
    WUHB_UTILS_RPX_INFO_OPENDIR_FAILED = -2,
    WUHB_UTILS_RPX_INFO_NO_RPX_FOUND   = -3,
} WUHBRPXInfoResultCode;

void WUHBUtils_CleanUp();
