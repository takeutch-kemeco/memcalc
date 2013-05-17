/* func_putpixel.c
 * Copyright (C) 2012 Takeutch Kemeco
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <fcntl.h>
#include <error.h>
#include <sys/user.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <linux/fb.h>

static char fb_dev_name[] = "/dev/fb0";

static int fh;
static uint32_t screen_height;
static uint32_t screen_width;
static uint32_t bytePerLine;
static uint32_t soff;
static uint32_t slen;
static void *smem;

static struct fb_var_screeninfo fb_var;
static struct fb_fix_screeninfo fb_fix;

static inline void print_fb_info(void)
{
#ifdef DEBUG
        printf("identification string eg [%s]\n"
               "Start of frame buffer mem [%u]\n"
               "Length of frame buffer mem [%u]\n"
               "see FB_TYPE_ [%u]\n"
               "Interleave for interleaved Planes [%u]\n"
               "see FB_VISUAL_ [%u]\n"
               "zero if no hardware panning [%u]\n"
               "zero if no hardware panning [%u]\n"
               "zero if no hardware ywrap [%u]\n"
               "length of a line in bytes [%u]\n"
               "Start of Memory Mapped I/O [%u]\n"
               "Length of Memory Mapped I/O [%u]\n"
               "Indicate to driver which [%u]\n",
               fb_fix.id,
               fb_fix.smem_start,
               fb_fix.smem_len,
               fb_fix.type,
               fb_fix.type_aux,
               fb_fix.visual,
               fb_fix.xpanstep,
               fb_fix.ypanstep,
               fb_fix.ywrapstep,
               fb_fix.line_length,
               fb_fix.mmio_start,
               fb_fix.mmio_len,
               fb_fix.accel);

        printf("visible resolution x[%u] y[%u]\n"
               "virtual resolution x[%u] y[%u]\n"
               "offset from virtual to visible x[%u] y[%u]\n"
               "guess what [%d]\n"
               "!= 0 Graylevels instead of colors [%u]\n"
               "bitfield in fb mem if true color, r ofs[%u] len[%u] msb_right[%u]\n"
               "bitfield in fb mem if true color, g ofs[%u] len[%u] msb_right[%u]\n"
               "bitfield in fb mem if true color, b ofs[%u] len[%u] msb_right[%u]\n"
               "bitfield in fb mem if true color, t ofs[%u] len[%u] msb_right[%u]\n"
               "!= 0 Non standard pixel format [%u]\n"
               "see FB_ACTIVATE_ [%u]\n"
               "height of picture in mm [%u]\n"
               "width of picture in mm [%u]\n"
               "(OBSOLETE) see fb_info.flags [%u]\n",
               fb_var.xres, fb_var.yres,
               fb_var.xres_virtual, fb_var.yres_virtual,
               fb_var.xoffset, fb_var.yoffset,
               fb_var.bits_per_pixel,
               fb_var.grayscale,
               fb_var.red.offset, fb_var.red.length, fb_var.red.msb_right,
               fb_var.green.offset, fb_var.green.length, fb_var.green.msb_right,
               fb_var.blue.offset, fb_var.blue.length, fb_var.blue.msb_right,
               fb_var.transp.offset, fb_var.transp.length, fb_var.transp.msb_right,
               fb_var.nonstd,
               fb_var.activate,
               fb_var.height,
               fb_var.width,
               fb_var.accel_flags);
#endif /* DEBUG*/
}

void __init_func_putpixel(void)
{
        int err;

        if ((fh = open(fb_dev_name, O_RDWR)) == -1) {
                error(fh, 1,
                      "フレームバッファーデバイス %s のオープンに失敗しました\n",
                      fb_dev_name);
                exit(EXIT_FAILURE);
        }

        struct stat st;
        if ((err = fstat(fh, &st)) == -1) {
                error(err, 2,
                      "デバイス %s のファイルステータス取得に失敗しました\n",
                      fb_dev_name);
                exit(EXIT_FAILURE);
        }

        if (!S_ISCHR(st.st_mode) || major(st.st_rdev) != 29 /* FB_MAJOR */) {
                error(0, 3,
                      "デバイス %s をフレームバッファーとして利用できません\n",
                      fb_dev_name);
                exit(EXIT_FAILURE);
        }

        struct fb_var_screeninfo fb_var;
        if (err = ioctl(fh, FBIOGET_VSCREENINFO, &fb_var)) {
                error(err, 4, "fb_var_screeninfo の取得に失敗しました\n");
                exit(EXIT_FAILURE);
        }

        struct fb_fix_screeninfo fb_fix;
        if (err = ioctl(fh, FBIOGET_FSCREENINFO, &fb_fix)) {
                error(err, 5, "fb_fix_screeninfo の取得に失敗しました\n");
                exit(EXIT_FAILURE);
        }

        screen_width  = fb_var.xres;
        screen_height = fb_var.yres;
        bytePerLine = fb_fix.line_length;

        soff = (uint32_t)(fb_fix.smem_start) & (~PAGE_MASK);
        slen = (fb_fix.smem_len + soff + ~PAGE_MASK) & PAGE_MASK;

        smem = mmap(NULL, slen, PROT_READ | PROT_WRITE, MAP_SHARED,
                    fh, (off_t)0);
        if (smem == (void*)-1) {
                error(-1, 6,
                      "デバイス %s のメモリーマッピングに失敗しました\n",
                      fb_dev_name);
                exit(EXIT_FAILURE);
        }

        smem = (void*)smem + soff;

#ifdef DEBUG
        print_fb_info();
#endif /* DEBUG */
}

void __close_func_putpixel(void)
{
        if (smem != (void*)-1)
                munmap((caddr_t)((ptrdiff_t)smem & PAGE_MASK), slen);

        close(fh);
}

static inline uint32_t* fb_seek_pix_adrs(const uint32_t x, const uint32_t y)
{
        return smem + (y * bytePerLine) + (x << 2);
}

static void fb_set_pixel(const uint32_t x, const uint32_t y,
                         const uint32_t col)
{
        if ((x >= 0 && x < screen_width) && (y >= 0 && y < screen_height)) {
                uint32_t* d = fb_seek_pix_adrs(x, y);
                *d = col;
        }
}

static void hsv_to_rgb(double* colR, double* colG, double* colB,
                       double H, double S, double V)
{
        const double R60 = M_PI / 3;
        H = fmod(H, 2 * M_PI);
        double h = fmod(H, R60) / R60;
        double s = V - (S * V);
        double _iv = h * V;
        double iv = _iv + ((V - _iv) * (1 - S));
        double _dv = V - (h * V);
        double dv = _dv + ((V - _dv) * (1 - S));
        int i = (int)(H / R60);

        double r, g, b;

        switch (i) {
        case 0:
                r = V;
                g = iv;
                b = s;
                break;

        case 1:
                r = dv;
                g = V;
                b = s;
                break;

        case 2:
                r = s;
                g = V;
                b = iv;
                break;

        case 3:
                r = s;
                g = dv;
                b = V;
                break;

        case 4:
                r = iv;
                g = s;
                b = V;
                break;

        default:
                r = V;
                g = s;
                b = dv;
                break;
        }

        (*colR) = (255 + 0.5) * r;
        (*colG) = (255 + 0.5) * g;
        (*colB) = (255 + 0.5) * b;
}

static uint32_t rgb_to_color(const double R, const double G, const double B)
{
        uint32_t r = R;
        uint32_t g = G;
        uint32_t b = B;
        return (b << 0) | (g << 8) | (r << 16);
}

static u_long hsv_to_color(const double H, const double S, const double V)
{
        double R, G, B;
        hsv_to_rgb(&R, &G, &B, H, S, V);
        return rgb_to_color(R, G, B);
}

void __func_putpixel(const double x, const double y,
                     const double H, const double S, const double V)
{
        uint32_t color = hsv_to_color(H, S, V);
        fb_set_pixel((uint32_t)x, (uint32_t)y, color);
}
