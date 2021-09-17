#include <nusys.h>
#include "inc/config.h"

void ClearBackground(u8 r, u8 g, u8 b);
void DrawRect(int x, int y);

static u8 b;

void stage00_init(void) {
    b = 255;    
}

void stage00_update(void) {
    b -= 5;
}

void stage00_draw(void) {
    glistp = glist;
    RCPInit(glistp);
    ClearBackground(32,32,b);
    DrawRect(SCREEN_WD/2 - 32, SCREEN_HT/2 - 32);

    gDPFullSync(glistp++);
    gSPEndDisplayList(glistp++);
    nuGfxTaskStart(glist, (s32)(glistp - glist) * sizeof(Gfx),
                   NU_GFX_UCODE_F3DEX, NU_SC_SWAPBUFFER);
}

void DrawRect(int x, int y) {
    gDPSetFillColor(glistp++, (GPACK_RGBA5551(255,0,0,1) << 16 | GPACK_RGBA5551(255,0,0,1)));  
    gDPFillRectangle(glistp++, x,y,x+64,y+64);
    gDPPipeSync(glistp++);
}

void ClearBackground(u8 r, u8 g, u8 b) {
    gDPSetCycleType(glistp++, G_CYC_FILL);
    gDPSetColorImage(glistp++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WD, osVirtualToPhysical(nuGfxCfb_ptr));
    gDPSetFillColor(glistp++, (GPACK_RGBA5551(r, g, b, 1) << 16 | GPACK_RGBA5551(r, g, b, 1)));
    gDPFillRectangle(glistp++, 0, 0, SCREEN_WD - 1, SCREEN_HT - 1);
    gDPPipeSync(glistp++);
}
