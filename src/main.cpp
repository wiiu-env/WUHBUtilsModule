#include "export.h"
#include "utils/logger.h"
#include "version.h"
#include <wums.h>

WUMS_MODULE_EXPORT_NAME("homebrew_wuhb_utils");

WUMS_USE_WUT_DEVOPTAB();

#define MODULE_VERSION      "v0.1.2"
#define MODULE_VERSION_FULL MODULE_VERSION MODULE_VERSION_EXTRA

WUMS_APPLICATION_STARTS() {
    OSReport("Running WUHBUtilsModule " MODULE_VERSION_FULL "\n");
    initLogging();
}

WUMS_APPLICATION_ENDS() {
    WUHBUtils_CleanUp();
    deinitLogging();
}
