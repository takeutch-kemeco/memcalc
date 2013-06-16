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

#include <blike.h>
#include <stdint.h>

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

static uint32_t hsv_to_color(const double H, const double S, const double V)
{
        double R, G, B;
        hsv_to_rgb(&R, &G, &B, H, S, V);
        return rgb_to_color(R, G, B);
}

void __func_putpixel(const double x, const double y,
                     const double H, const double S, const double V)
{
        uint32_t color = hsv_to_color(H, S, V);
        bl_setPix((uint32_t)x, (uint32_t)y, color);
}
