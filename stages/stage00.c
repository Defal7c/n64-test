#include <nusys.h>
#include "inc/config.h"
#include "inc/Chief_Blue.h"

static int x;
static int y;
static float ang;
static int scalex;
static int scaley;

void ClearBackground(u8 r, u8 g, u8 b);
void DrawSpr(int x, int y, float ang);

static Gfx newimage_C_dummy_aligner1[] = { gsSPEndDisplayList() };
static unsigned short newimage[16*16];

void stage00_init(void) {
    x = SCREEN_WD/2;
    y = SCREEN_HT/2;
}

void stage00_update(void) {
    ang += 1;
}

void stage00_draw(void) {
    glistp = glist;
    RCPInit(glistp);
    ClearBackground(32,255,32);
    DrawSpr(x,y,ang);

    gDPFullSync(glistp++);
    gSPEndDisplayList(glistp++);
    nuGfxTaskStart(glist, (s32)(glistp - glist) * sizeof(Gfx),
                   NU_GFX_UCODE_F3DEX, NU_SC_SWAPBUFFER);
}

void DrawSpr(int x, int y, float ang) {
    //rotation stuff
    int i,j;
    float rad = ang*0.017453;
    float co = cosf(rad);
    float si = sinf(rad);
    unsigned short* image = Chief_Blue;

    for(i=0;i<16*16;i++)
        newimage[i] = 0;

    for(i=0;i<16;i++)
        for(j=0;j<16;j++)
        {
            int x = (float)(i-8)*co - (float)(j-8)*si + 8;
            int y = (float)(i-8)*si + (float)(j-8)*co + 8;
            if(x < 0 || x >= 16 || y < 0 || y >= 16)
              continue;
            newimage[i+j*16] = image[y*16+x];
        }
    gDPSetCycleType(glistp++, G_CYC_1CYCLE);
    gDPSetCombineMode(glistp++, G_CC_DECALRGBA,G_CC_DECALRGBA);
    gDPSetRenderMode(glistp++, G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE);
    gDPSetDepthSource(glistp++, G_ZS_PRIM);
    gDPSetPrimDepth(glistp++,0,0);
    gDPSetTexturePersp(glistp++,G_TP_NONE);

    gDPLoadTextureBlock(
        glistp++,
        newimage,
        G_IM_FMT_RGBA, G_IM_SIZ_16b,
        16,16,
        0,
        G_TX_MIRROR,G_TX_CLAMP, //cms, cmt
        G_TX_NOMASK,G_TX_NOMASK,
        G_TX_NOLOD,G_TX_NOLOD
    );

    gSPTextureRectangle(
        glistp++,
        x-8<<2, y-8<<2,
        x+8<<2, y+8<<2,
        G_TX_RENDERTILE,
        0<<5,0<<5,
        1<<10,1<<10 
    );

    gDPPipeSync(glistp++);
}

void ClearBackground(u8 r, u8 g, u8 b) {
    gDPSetCycleType(glistp++, G_CYC_FILL);
    gDPSetDepthImage(glistp++, nuGfxZBuffer);
    gDPSetColorImage(glistp++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WD, osVirtualToPhysical(nuGfxCfb_ptr));
    gDPSetFillColor(glistp++, (GPACK_RGBA5551(r, g, b, 1) << 16 | GPACK_RGBA5551(r, g, b, 1)));
    gDPFillRectangle(glistp++, 0, 0, SCREEN_WD - 1, SCREEN_HT - 1);
    gDPPipeSync(glistp++);
}
