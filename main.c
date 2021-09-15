#include <nusys.h>
#include "inc/stages.h"

static void vsyncCallback(int pendingGfx);

void mainproc(void * dummy)
{
    nuGfxInit();
    nuGfxFuncSet((NUGfxFunc)vsyncCallback);
    nuGfxDisplayOn();
    while(1)
      ;
}

void vsyncCallback(int pendingGfx)
{
    stage00_update();
    if (pendingGfx < 1)
      stage00_draw();
}
