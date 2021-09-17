#include <nusys.h>
#include "inc/config.h"
#include "inc/stages.h"

static void vsyncCallback(int pendingGfx);
static void callback_prenmi();

void mainproc(void) {
    if(TV_TYPE == PAL)
    {
        osViSetMode(&osViModeTable[OS_VI_FPAL_LAN1]);
        osViSetYScale(0.833);
        nuPreNMIFuncSet((NUScPreNMIFunc)callback_prenmi);
    }else if(TV_TYPE == MPAL){
        osViSetMode(&osViModeTable[OS_VI_MPAL_LAN1]);
    }

    nuGfxInit();
    nuGfxFuncSet((NUGfxFunc)vsyncCallback);
    nuGfxDisplayOn();
    while(1)
      ;
}

void callback_prenmi()
{
    nuGfxDisplayOff();
    osViSetYScale(1);
}

void vsyncCallback(int pendingGfx) {
    stage00_update();
    if (pendingGfx < 1)
      stage00_draw();
}
