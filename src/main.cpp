#include "export.h"
#include "utils/logger.h"
#include <wums.h>

WUMS_MODULE_EXPORT_NAME("homebrew_wuhb_utils");

WUMS_USE_WUT_DEVOPTAB();

WUMS_APPLICATION_STARTS() {
    initLogging();
}

WUMS_APPLICATION_ENDS() {
    WUHBUtils_CleanUp();
    deinitLogging();
}
