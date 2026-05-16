#include "popcorn.h"
#include "stb_truetype.h"

char buffer[24<<20]; // font file buffer

pop_Services* svc;

// Return the largest integer <= x
int floor(float x);

int pop_API pop_main(pop_Services* asvc, int argc, CHAR16** argv) {
    svc = asvc;
    
    stbtt_fontinfo font;
    int ascent, baseline, ch = 0;
    float scale, xpos = 2.0f;
    char *text = "Hello Popcorn!";

    // Load font file into buffer
    popf_FileMode mode = { TRUE, FALSE, FALSE, TRUE };
    popf_File* f = svc->fileopen(svc, L"/binstuff/stb_truetype_test/notosans.ttf", mode);
    if (!f) {
        svc->println(svc, L"Font load failed.");
        return 1;
    }
    void* fontdata = f->read(f);
    f->close(f);

    stbtt_InitFont(&font, fontdata, 0);

    scale = stbtt_ScaleForPixelHeight(&font, 100);
    stbtt_GetFontVMetrics(&font, &ascent, 0, 0);
    baseline = (int)(ascent * scale);

    // Initialize graphics
    if (svc->sgfx->init(svc->sgfx) != pop_SUCCESS) {
        svc->println(svc, L"Graphics init failed.");
        return 1;
    }

    while (text[ch]) {
        int advance, lsb, x0, y0, x1, y1;
        float x_shift = xpos - (float)floor(xpos);
        stbtt_GetCodepointHMetrics(&font, text[ch], &advance, &lsb);
        stbtt_GetCodepointBitmapBoxSubpixel(&font, text[ch], scale, scale,
                                            x_shift, 0, &x0, &y0, &x1, &y1);

        int w = x1 - x0;
        int h = y1 - y0;
        unsigned char* bitmap = (unsigned char*)svc->memalloc(svc, w * h);

        stbtt_MakeCodepointBitmapSubpixel(&font, bitmap, w, h, w,
                                          scale, scale, x_shift, 0, text[ch]);

        // Blit bitmap into framebuffer
        for (int yy = 0; yy < h; yy++) {
            for (int xx = 0; xx < w; xx++) {
                unsigned char val = bitmap[yy * w + xx];
                if (val) {
                    popg_PUTPIXEL(svc->sgfx,
                        (int)xpos + x0 + xx,
                        baseline + y0 + yy,
                        val, val, val); // grayscale
                }
            }
        }

        svc->memfree(svc, bitmap);

        xpos += advance * scale;
        if (text[ch+1])
            xpos += scale * stbtt_GetCodepointKernAdvance(&font, text[ch], text[ch+1]);
        ++ch;
    }

    // Push framebuffer to screen
    svc->sgfx->blit(svc->sgfx);

    // Wait for a keypress before exit
    svc->curmove(svc, 100, 100);
    svc->println(svc, L"Press Enter to exit.");
    CHAR16* line = svc->readline(svc);
    svc->memfree(svc, line);

    svc->sgfx->deinit(svc->sgfx);
    return 0;
}

int floor(float x) {
    int i = (int)x;          // truncate toward zero
    if (x < 0.0f && x != (float)i) {
        return i - 1;        // adjust for negatives
    }
    return i;
}

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype_shim.h"