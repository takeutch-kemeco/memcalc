/* complex.c
 * Copyright (C) 2013 Takeutch Kemeco
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

#include <math.h>
#include <stdint.h>
#include "complex.h"

struct Complex complex_constructor(const double re, const double im)
{
        struct Complex a = {.re = re, .im = im};

        return a;
}

double complex_realpart(const struct Complex a)
{
        return a.re;
}

double complex_imagpart(const struct Complex a)
{
        return a.im;
}

struct Complex complex_constructor_safe(const struct Complex re, const struct Complex im)
{
        return complex_constructor(complex_realpart(re), complex_realpart(im));
}

struct Complex complex_realpart_safe(const struct Complex a)
{
        return complex_constructor(complex_realpart(a), 0);
}

struct Complex complex_imagpart_safe(const struct Complex a)
{
        return complex_constructor(complex_imagpart(a), 0);
}

struct Complex complex_mk_polar(const struct Complex a)
{
        const double z = complex_realpart(a);
        const double r = complex_imagpart(a);

        return complex_constructor(cos(r) * z, sin(r) * z);
}

struct Complex complex_cis(const struct Complex a)
{
        return complex_mk_polar(complex_constructor(1.0, complex_realpart(a)));
}

static double complex_polar_r(const struct Complex a)
{
        return atan2(complex_imagpart(a), complex_realpart(a));
}

static double complex_polar_z(const struct Complex a)
{
        const double re = complex_realpart(a);
        const double im = complex_imagpart(a);

        return sqrt(re * re + im * im);
}

struct Complex complex_polar(const struct Complex a)
{
        return complex_constructor(complex_polar_z(a), complex_polar_r(a));
}

struct Complex complex_conjugate(const struct Complex a)
{
        const double re = complex_realpart(a);
        const double im = complex_imagpart(a);

        return complex_constructor(re, -im);
}

struct Complex complex_add(const struct Complex a, const struct Complex b)
{
        const double a_re = complex_realpart(a);
        const double a_im = complex_imagpart(a);
        const double b_re = complex_realpart(b);
        const double b_im = complex_imagpart(b);

        return complex_constructor(a_re + b_re, a_im + b_im);
}

struct Complex complex_sub(const struct Complex a, const struct Complex b)
{
        const double a_re = complex_realpart(a);
        const double a_im = complex_imagpart(a);
        const double b_re = complex_realpart(b);
        const double b_im = complex_imagpart(b);

        return complex_constructor(a_re - b_re, a_im - b_im);
}

struct Complex complex_mul(const struct Complex a, const struct Complex b)
{
        const double a_re = complex_realpart(a);
        const double a_im = complex_imagpart(a);
        const double b_re = complex_realpart(b);
        const double b_im = complex_imagpart(b);

        return complex_constructor(a_re * b_re - a_im * b_im, a_re * b_im + a_im * b_re);
}

struct Complex complex_div(const struct Complex a, const struct Complex b)
{
        const double a_re = complex_realpart(a);
        const double a_im = complex_imagpart(a);
        const double b_re = complex_realpart(b);
        const double b_im = complex_imagpart(b);
        const double c = (b_re * b_re) + (b_im * b_im);

        return complex_constructor((a_re * b_re + a_im * b_im) / c, (a_im * b_re - a_re * b_im) / c);
}

struct Complex complex_mod(const struct Complex a, const struct Complex b)
{
        const double re = fmod(complex_realpart(a), complex_realpart(b));
        const double im = fmod(complex_imagpart(a), complex_realpart(b));

        return complex_constructor(re, im);
}

struct Complex complex_lshift(const struct Complex a, const struct Complex b)
{
        const double re = ((uint64_t)complex_realpart(a)) << ((uint64_t)complex_realpart(b));
        const double im = ((uint64_t)complex_imagpart(a)) << ((uint64_t)complex_imagpart(b));

        return complex_constructor(re, im);
}

struct Complex complex_rshift(const struct Complex a, const struct Complex b)
{
        const double re = ((uint64_t)complex_realpart(a)) >> ((uint64_t)complex_realpart(b));
        const double im = ((uint64_t)complex_imagpart(a)) >> ((uint64_t)complex_imagpart(b));

        return complex_constructor(re, im);
}

struct Complex complex_or(const struct Complex a, const struct Complex b)
{
        const double re = ((uint64_t)complex_realpart(a)) | ((uint64_t)complex_realpart(b));
        const double im = ((uint64_t)complex_imagpart(a)) | ((uint64_t)complex_imagpart(b));

        return complex_constructor(re, im);
}

struct Complex complex_and(const struct Complex a, const struct Complex b)
{
        const double re = ((uint64_t)complex_realpart(a)) & ((uint64_t)complex_realpart(b));
        const double im = ((uint64_t)complex_imagpart(a)) & ((uint64_t)complex_imagpart(b));

        return complex_constructor(re, im);
}

struct Complex complex_xor(const struct Complex a, const struct Complex b)
{
        const double re = ((uint64_t)complex_realpart(a)) ^ ((uint64_t)complex_realpart(b));
        const double im = ((uint64_t)complex_imagpart(a)) ^ ((uint64_t)complex_imagpart(b));

        return complex_constructor(re, im);
}

struct Complex complex_not(const struct Complex a)
{
        const double re = ~((uint64_t)complex_realpart(a));
        const double im = ~((uint64_t)complex_imagpart(a));

        return complex_constructor(re, im);
}

struct Complex complex_comparison(const struct Complex a, const struct Complex b)
{
        const double re = (complex_realpart(a) == complex_realpart(b)) ? 1 : 0;
        const double im = (complex_imagpart(a) == complex_imagpart(b)) ? 1 : 0;

        return complex_constructor(re, im);
}

struct Complex complex_not_comparison(const struct Complex a, const struct Complex b)
{
        const double re = (complex_realpart(a) != complex_realpart(b)) ? 1 : 0;
        const double im = (complex_imagpart(a) != complex_imagpart(b)) ? 1 : 0;

        return complex_constructor(re, im);
}

struct Complex complex_is_small(const struct Complex a, const struct Complex b)
{
        const double re = (complex_realpart(a) < complex_realpart(b)) ? 1 : 0;
        const double im = (complex_imagpart(a) < complex_imagpart(b)) ? 1 : 0;

        return complex_constructor(re, im);
}

struct Complex complex_is_large(const struct Complex a, const struct Complex b)
{
        const double re = (complex_realpart(a) > complex_realpart(b)) ? 1 : 0;
        const double im = (complex_imagpart(a) > complex_imagpart(b)) ? 1 : 0;

        return complex_constructor(re, im);
}

struct Complex complex_is_small_comp(const struct Complex a, const struct Complex b)
{
        const double re = (complex_realpart(a) <= complex_realpart(b)) ? 1 : 0;
        const double im = (complex_imagpart(a) <= complex_imagpart(b)) ? 1 : 0;

        return complex_constructor(re, im);
}

struct Complex complex_is_large_comp(const struct Complex a, const struct Complex b)
{
        const double re = (complex_realpart(a) >= complex_realpart(b)) ? 1 : 0;
        const double im = (complex_imagpart(a) >= complex_imagpart(b)) ? 1 : 0;

        return complex_constructor(re, im);
}

struct Complex complex_abs(const struct Complex a)
{
        const double re = complex_realpart(a);
        const double im = complex_imagpart(a);
        const double z = sqrt(re * re + im * im);

        return complex_constructor(z, 0);
}
