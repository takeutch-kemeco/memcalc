/* complex.h
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

#ifndef __COMPLEX_H__
#define __COMPLEX_H__

struct Complex {
        double re;
        double im;
};

struct Complex complex_constructor(const double re, const double im);
double complex_realpart(const struct Complex a);
double complex_imagpart(const struct Complex a);

struct Complex complex_constructor_safe(const struct Complex re, const struct Complex im);
struct Complex complex_realpart_safe(const struct Complex a);
struct Complex complex_imagpart_safe(const struct Complex a);
struct Complex complex_mk_polar(const struct Complex a);
struct Complex complex_cis(const struct Complex a);
struct Complex complex_polar(const struct Complex a);
struct Complex complex_conjugate(struct Complex a);
struct Complex complex_add(const struct Complex a, const struct Complex b);
struct Complex complex_sub(const struct Complex a, const struct Complex b);
struct Complex complex_mul(const struct Complex a, const struct Complex b);
struct Complex complex_div(const struct Complex a, const struct Complex b);
struct Complex complex_floor(const struct Complex a);
struct Complex complex_mod(const struct Complex a, const struct Complex b);
struct Complex complex_lshift(const struct Complex a, const struct Complex b);
struct Complex complex_rshift(const struct Complex a, const struct Complex b);
struct Complex complex_or(const struct Complex a, const struct Complex b);
struct Complex complex_and(const struct Complex a, const struct Complex b);
struct Complex complex_xor(const struct Complex a, const struct Complex b);
struct Complex complex_not(const struct Complex a);
struct Complex complex_comparison(const struct Complex a, const struct Complex b);
struct Complex complex_not_comparison(const struct Complex a, const struct Complex b);
struct Complex complex_is_small(const struct Complex a, const struct Complex b);
struct Complex complex_is_large(const struct Complex a, const struct Complex b);
struct Complex complex_is_small_comp(const struct Complex a, const struct Complex b);
struct Complex complex_is_large_comp(const struct Complex a, const struct Complex b);
struct Complex complex_abs(const struct Complex a);
struct Complex complex_pow(const struct Complex a, const struct Complex b);

#endif /* __COMPLEX_H__ */
