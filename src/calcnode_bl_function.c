/* calcnode_bl_function.c
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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "complex.h"
#include "node.h"
#include "calcnode.h"
#include "calcnode_bl_function.h"
#include "memcalc.bison.h"

static struct CalcNode calcnode_bl_function__FUNC_BL_PUTC(struct Node* a)
{
        struct CalcNode cn0 = calcnode(node_child(a, 0));

        const double cn0re = complex_realpart(cn0.compval);

        bl_putc((int32_t)cn0re);

        cn0.type = CNT_BOTTOM;

        return cn0;
}

static struct CalcNode calcnode_bl_function__FUNC_BL_PUTS1(struct Node* a)
{
        struct CalcNode cn0 = {.type = CNT_BOTTOM};

        /* undefined */

        return cn0;
}

static struct CalcNode calcnode_bl_function__FUNC_BL_PRINTF(struct Node* a)
{
        struct CalcNode cn0 = {.type = CNT_BOTTOM};

        /* undefined */

        return cn0;
}

static struct CalcNode calcnode_bl_function__FUNC_BL_SCANF(struct Node* a)
{
        struct CalcNode cn0 = {.type = CNT_BOTTOM};

        /* undefined */

        return cn0;
}

static struct CalcNode calcnode_bl_function__FUNC_BL_MALLOC(struct Node* a)
{
        struct CalcNode cn0 = {.type = CNT_BOTTOM};

        /* undefined */

        return cn0;
}

static struct CalcNode calcnode_bl_function__FUNC_BL_RAND(struct Node* a)
{
        bl_rand();

        struct CalcNode cn0 = {.type = CNT_BOTTOM};

        return cn0;
}

static struct CalcNode calcnode_bl_function__FUNC_BL_SRAND(struct Node* a)
{
        struct CalcNode cn0 = calcnode(node_child(a, 0));

        const double cn0re = complex_realpart(cn0.compval);

        bl_srand((int32_t)cn0re);

        cn0.type = CNT_BOTTOM;

        return cn0;
}

static struct CalcNode calcnode_bl_function__FUNC_BL_GETS(struct Node* a)
{
        struct CalcNode cn0 = {.type = CNT_BOTTOM};

        /* undefined */

        return cn0;
}

static struct CalcNode calcnode_bl_function__FUNC_BL_OPENWIN(struct Node* a)
{
        struct CalcNode cn0 = calcnode(node_child(a, 0));
        struct CalcNode cn1 = calcnode(node_child(a, 1));

        const double cn0re = complex_realpart(cn0.compval);
        const double cn1re = complex_realpart(cn1.compval);

        bl_openWin((int32_t)cn0re, (int32_t)cn1re);

        cn0.type = CNT_BOTTOM;

        return cn0;
}

static struct CalcNode calcnode_bl_function__FUNC_BL_SETCOL(struct Node* a)
{
        struct CalcNode cn0 = calcnode(node_child(a, 0));

        const double cn0re = complex_realpart(cn0.compval);

        bl_setCol((int32_t)cn0re);

        cn0.type = CNT_BOTTOM;

        return cn0;
}

static struct CalcNode calcnode_bl_function__FUNC_BL_SETBCOL(struct Node* a)
{
        struct CalcNode cn0 = calcnode(node_child(a, 0));

        const double cn0re = complex_realpart(cn0.compval);

        bl_setBCol((int32_t)cn0re);

        cn0.type = CNT_BOTTOM;

        return cn0;
}

static struct CalcNode calcnode_bl_function__FUNC_BL_RGB(struct Node* a)
{
        struct CalcNode cn0 = calcnode(node_child(a, 0));
        struct CalcNode cn1 = calcnode(node_child(a, 1));
        struct CalcNode cn2 = calcnode(node_child(a, 2));

        const double cn0re = complex_realpart(cn0.compval);
        const double cn1re = complex_realpart(cn1.compval);
        const double cn2re = complex_realpart(cn2.compval);

        const double re = bl_rgb((int32_t)cn0re, (int32_t)cn1re, (int32_t)cn2re);

        cn0.type = CNT_COMPVAL;
        cn0.compval = complex_constructor(re, 0);

        return cn0;
}

static struct CalcNode calcnode_bl_function__FUNC_BL_ICOL(struct Node* a)
{
        struct CalcNode cn0 = calcnode(node_child(a, 0));

        const double cn0re = complex_realpart(cn0.compval);

        bl_iCol((int32_t)cn0re);

        cn0.type = CNT_BOTTOM;

        return cn0;
}

static struct CalcNode calcnode_bl_function__FUNC_BL_FLSHWIN(struct Node* a)
{
        struct CalcNode cn0 = calcnode(node_child(a, 0));
        struct CalcNode cn1 = calcnode(node_child(a, 1));
        struct CalcNode cn2 = calcnode(node_child(a, 2));
        struct CalcNode cn3 = calcnode(node_child(a, 3));

        const double cn0re = complex_realpart(cn0.compval);
        const double cn1re = complex_realpart(cn1.compval);
        const double cn2re = complex_realpart(cn2.compval);
        const double cn3re = complex_realpart(cn3.compval);

        bl_flshWin((int32_t)cn0re, (int32_t)cn1re, (int32_t)cn2re, (int32_t)cn3re);

        cn0.type = CNT_BOTTOM;

        return cn0;
}

static struct CalcNode calcnode_bl_function__FUNC_BL_GETGRPB(struct Node* a)
{
        struct CalcNode cn0 = {.type = CNT_BOTTOM};

        /* undefined */

        return cn0;
}

static struct CalcNode calcnode_bl_function__FUNC_BL_SETPIX(struct Node* a)
{
        struct CalcNode cn0 = calcnode(node_child(a, 0));
        struct CalcNode cn1 = calcnode(node_child(a, 1));
        struct CalcNode cn2 = calcnode(node_child(a, 2));

        const double cn0re = complex_realpart(cn0.compval);
        const double cn1re = complex_realpart(cn1.compval);
        const double cn2re = complex_realpart(cn2.compval);

        bl_setPix((int32_t)cn0re, (int32_t)cn1re, (int32_t)cn2re);

        cn0.type = CNT_BOTTOM;

        return cn0;
}

static struct CalcNode calcnode_bl_function__FUNC_BL_FILLRECT(struct Node* a)
{
        struct CalcNode cn0 = calcnode(node_child(a, 0));
        struct CalcNode cn1 = calcnode(node_child(a, 1));
        struct CalcNode cn2 = calcnode(node_child(a, 2));
        struct CalcNode cn3 = calcnode(node_child(a, 3));

        const double cn0re = complex_realpart(cn0.compval);
        const double cn1re = complex_realpart(cn1.compval);
        const double cn2re = complex_realpart(cn2.compval);
        const double cn3re = complex_realpart(cn3.compval);

        bl_fillRect((int32_t)cn0re, (int32_t)cn1re, (int32_t)cn2re, (int32_t)cn3re);

        cn0.type = CNT_BOTTOM;

        return cn0;
}

static struct CalcNode calcnode_bl_function__FUNC_BL_DRAWRECT(struct Node* a)
{
        struct CalcNode cn0 = calcnode(node_child(a, 0));
        struct CalcNode cn1 = calcnode(node_child(a, 1));
        struct CalcNode cn2 = calcnode(node_child(a, 2));
        struct CalcNode cn3 = calcnode(node_child(a, 3));

        const double cn0re = complex_realpart(cn0.compval);
        const double cn1re = complex_realpart(cn1.compval);
        const double cn2re = complex_realpart(cn2.compval);
        const double cn3re = complex_realpart(cn3.compval);

        bl_drawRect((int32_t)cn0re, (int32_t)cn1re, (int32_t)cn2re, (int32_t)cn3re);

        cn0.type = CNT_BOTTOM;

        return cn0;
}

static struct CalcNode calcnode_bl_function__FUNC_BL_DRAWLINE(struct Node* a)
{
        struct CalcNode cn0 = calcnode(node_child(a, 0));
        struct CalcNode cn1 = calcnode(node_child(a, 1));
        struct CalcNode cn2 = calcnode(node_child(a, 2));
        struct CalcNode cn3 = calcnode(node_child(a, 3));

        const double cn0re = complex_realpart(cn0.compval);
        const double cn1re = complex_realpart(cn1.compval);
        const double cn2re = complex_realpart(cn2.compval);
        const double cn3re = complex_realpart(cn3.compval);

        bl_drawLine((int32_t)cn0re, (int32_t)cn1re, (int32_t)cn2re, (int32_t)cn3re);

        cn0.type = CNT_BOTTOM;

        return cn0;
}

static struct CalcNode calcnode_bl_function__FUNC_BL_RND(struct Node* a)
{
        struct CalcNode cn0 = calcnode(node_child(a, 0));

        const double cn0re = complex_realpart(cn0.compval);

        const double re = bl_rnd((int32_t)cn0re);

        cn0.type = CNT_COMPVAL;
        cn0.compval = complex_constructor(re, 0);

        return cn0;
}

static struct CalcNode calcnode_bl_function__FUNC_BL_WAIT(struct Node* a)
{
        struct CalcNode cn0 = calcnode(node_child(a, 0));

        const double cn0re = complex_realpart(cn0.compval);

        bl_wait((int32_t)cn0re);

        cn0.type = CNT_BOTTOM;

        return cn0;
}

static struct CalcNode calcnode_bl_function__FUNC_BL_COLOR(struct Node* a)
{
        struct CalcNode cn0 = calcnode(node_child(a, 0));
        struct CalcNode cn1 = calcnode(node_child(a, 1));

        const double cn0re = complex_realpart(cn0.compval);
        const double cn1re = complex_realpart(cn1.compval);

        bl_drawLine((int32_t)cn0re, (int32_t)cn1re);

        cn0.type = CNT_BOTTOM;

        return cn0;
}

static struct CalcNode calcnode_bl_function__FUNC_BL_LOCATE(struct Node* a)
{
        struct CalcNode cn0 = calcnode(node_child(a, 0));
        struct CalcNode cn1 = calcnode(node_child(a, 1));

        const double cn0re = complex_realpart(cn0.compval);
        const double cn1re = complex_realpart(cn1.compval);

        bl_locate((int32_t)cn0re, (int32_t)cn1re);

        cn0.type = CNT_BOTTOM;

        return cn0;
}

static struct CalcNode calcnode_bl_function__FUNC_BL_GETPIX(struct Node* a)
{
        struct CalcNode cn0 = calcnode(node_child(a, 0));
        struct CalcNode cn1 = calcnode(node_child(a, 1));

        const double cn0re = complex_realpart(cn0.compval);
        const double cn1re = complex_realpart(cn1.compval);

        const double re = bl_getPix((int32_t)cn0re, (int32_t)cn1re);

        cn0.type = CNT_COMPVAL;
        cn0.compval = complex_constructor(re, 0);

        return cn0;
}

static struct CalcNode calcnode_bl_function__FUNC_BL_WAITNF(struct Node* a)
{
        struct CalcNode cn0 = calcnode(node_child(a, 0));

        const double cn0re = complex_realpart(cn0.compval);

        bl_waitNF((int32_t)cn0re);

        cn0.type = CNT_BOTTOM;

        return cn0;
}

static struct CalcNode calcnode_bl_function__FUNC_BL_INKEY1(struct Node* a)
{
        struct CalcNode cn0;

        const double re = bl_inkey1();

        cn0.type = CNT_COMPVAL;
        cn0.compval = complex_constructor(re, 0);

        return cn0;
}

static struct CalcNode calcnode_bl_function__FUNC_BL_CLS(struct Node* a)
{
        struct CalcNode cn0;

        bl_cls();

        cn0.type = CNT_BOTTOM;

        return cn0;
}

static struct CalcNode calcnode_bl_function__FUNC_BL_INPTINT(struct Node* a)
{
        struct CalcNode cn0 = {.type = CNT_BOTTOM};

        /* undefined */

        return cn0;
}

static struct CalcNode calcnode_bl_function__FUNC_BL_INPTFLOT(struct Node* a)
{
        struct CalcNode cn0 = {.type = CNT_BOTTOM};

        /* undefined */

        return cn0;
}

static struct CalcNode calcnode_bl_function__FUNC_BL_SETMODE(struct Node* a)
{
        struct CalcNode cn0 = calcnode(node_child(a, 0));

        const double cn0re = complex_realpart(cn0.compval);

        bl_setMode((int32_t)cn0re);

        cn0.type = CNT_BOTTOM;

        return cn0;
}

static struct CalcNode calcnode_bl_function__FUNC_BL_FILLOVAL(struct Node* a)
{
        struct CalcNode cn0 = calcnode(node_child(a, 0));
        struct CalcNode cn1 = calcnode(node_child(a, 1));
        struct CalcNode cn2 = calcnode(node_child(a, 2));
        struct CalcNode cn3 = calcnode(node_child(a, 3));

        const double cn0re = complex_realpart(cn0.compval);
        const double cn1re = complex_realpart(cn1.compval);
        const double cn2re = complex_realpart(cn2.compval);
        const double cn3re = complex_realpart(cn3.compval);

        bl_fillOval((int32_t)cn0re, (int32_t)cn1re, (int32_t)cn2re, (int32_t)cn3re);

        cn0.type = CNT_BOTTOM;

        return cn0;
}

static struct CalcNode calcnode_bl_function__FUNC_BL_DRAWSTR(struct Node* a)
{
        struct CalcNode cn0 = {.type = CNT_BOTTOM};

        /* undefined */

        return cn0;
}

static struct CalcNode calcnode_bl_function__FUNC_BL_OPENVWIN(struct Node* a)
{
        struct CalcNode cn0 = calcnode(node_child(a, 0));
        struct CalcNode cn1 = calcnode(node_child(a, 1));
        struct CalcNode cn2 = calcnode(node_child(a, 2));

        const double cn0re = complex_realpart(cn0.compval);
        const double cn1re = complex_realpart(cn1.compval);
        const double cn2re = complex_realpart(cn2.compval);

        bl_openVWin((int32_t)cn0re, (int32_t)cn1re, (int32_t)cn2re);

        cn0.type = CNT_BOTTOM;

        return cn0;
}

static struct CalcNode calcnode_bl_function__FUNC_BL_SLCTWIN(struct Node* a)
{
        struct CalcNode cn0 = calcnode(node_child(a, 0));

        const double cn0re = complex_realpart(cn0.compval);

        bl_slctWin((int32_t)cn0re);

        cn0.type = CNT_BOTTOM;

        return cn0;
}

static struct CalcNode calcnode_bl_function__FUNC_BL_COPYRCT0(struct Node* a)
{
        struct CalcNode cn0 = calcnode(node_child(a, 0));
        struct CalcNode cn1 = calcnode(node_child(a, 1));
        struct CalcNode cn2 = calcnode(node_child(a, 2));
        struct CalcNode cn3 = calcnode(node_child(a, 3));
        struct CalcNode cn4 = calcnode(node_child(a, 4));
        struct CalcNode cn5 = calcnode(node_child(a, 5));
        struct CalcNode cn6 = calcnode(node_child(a, 6));
        struct CalcNode cn7 = calcnode(node_child(a, 7));

        const double cn0re = complex_realpart(cn0.compval);
        const double cn1re = complex_realpart(cn1.compval);
        const double cn2re = complex_realpart(cn2.compval);
        const double cn3re = complex_realpart(cn3.compval);
        const double cn4re = complex_realpart(cn4.compval);
        const double cn5re = complex_realpart(cn5.compval);
        const double cn6re = complex_realpart(cn6.compval);
        const double cn7re = complex_realpart(cn7.compval);

        bl_copyRct0((int32_t)cn0re, (int32_t)cn1re, (int32_t)cn2re, (int32_t)cn3re,
                    (int32_t)cn4re, (int32_t)cn5re, (int32_t)cn6re, (int32_t)cn7re);

        cn0.type = CNT_BOTTOM;

        return cn0;
}

static struct CalcNode calcnode_bl_function__FUNC_BL_COPYRCT1(struct Node* a)
{
        struct CalcNode cn0 = calcnode(node_child(a, 0));
        struct CalcNode cn1 = calcnode(node_child(a, 1));
        struct CalcNode cn2 = calcnode(node_child(a, 2));
        struct CalcNode cn3 = calcnode(node_child(a, 3));
        struct CalcNode cn4 = calcnode(node_child(a, 4));
        struct CalcNode cn5 = calcnode(node_child(a, 5));
        struct CalcNode cn6 = calcnode(node_child(a, 6));
        struct CalcNode cn7 = calcnode(node_child(a, 7));
        struct CalcNode cn8 = calcnode(node_child(a, 8));

        const double cn0re = complex_realpart(cn0.compval);
        const double cn1re = complex_realpart(cn1.compval);
        const double cn2re = complex_realpart(cn2.compval);
        const double cn3re = complex_realpart(cn3.compval);
        const double cn4re = complex_realpart(cn4.compval);
        const double cn5re = complex_realpart(cn5.compval);
        const double cn6re = complex_realpart(cn6.compval);
        const double cn7re = complex_realpart(cn7.compval);
        const double cn8re = complex_realpart(cn8.compval);

        bl_copyRct1((int32_t)cn0re, (int32_t)cn1re, (int32_t)cn2re, (int32_t)cn3re,
                    (int32_t)cn4re, (int32_t)cn5re, (int32_t)cn6re, (int32_t)cn7re,
                    (int32_t)cn8re);

        cn0.type = CNT_BOTTOM;

        return cn0;
}

static struct CalcNode calcnode_bl_function__FUNC_BL_DRAWPTRN_D(struct Node* a)
{
        struct CalcNode cn0 = {.type = CNT_BOTTOM};

        /* undefined */

        return cn0;
}

static struct CalcNode calcnode_bl_function__FUNC_BL_DRAWPTRN_R(struct Node* a)
{
        struct CalcNode cn0 = {.type = CNT_BOTTOM};

        /* undefined */

        return cn0;
}

struct CalcNode calcnode_bl_function(struct Node* a)
{
        switch (a->ope) {
        case __FUNC_BL_PUTC:            return calcnode_bl_function__FUNC_BL_PUTC(a);
        case __FUNC_BL_PUTS1:           return calcnode_bl_function__FUNC_BL_PUTS1(a);
        case __FUNC_BL_PRINTF:          return calcnode_bl_function__FUNC_BL_PRINTF(a);
        case __FUNC_BL_SCANF:           return calcnode_bl_function__FUNC_BL_SCANF(a);
        case __FUNC_BL_MALLOC:          return calcnode_bl_function__FUNC_BL_MALLOC(a);
        case __FUNC_BL_RAND:            return calcnode_bl_function__FUNC_BL_RAND(a);
        case __FUNC_BL_SRAND:           return calcnode_bl_function__FUNC_BL_SRAND(a);
        case __FUNC_BL_GETS:            return calcnode_bl_function__FUNC_BL_GETS(a);
        case __FUNC_BL_OPENWIN:         return calcnode_bl_function__FUNC_BL_OPENWIN(a);
        case __FUNC_BL_SETCOL:          return calcnode_bl_function__FUNC_BL_SETCOL(a);
        case __FUNC_BL_SETBCOL:         return calcnode_bl_function__FUNC_BL_SETBCOL(a);
        case __FUNC_BL_RGB:             return calcnode_bl_function__FUNC_BL_RGB(a);
        case __FUNC_BL_ICOL:            return calcnode_bl_function__FUNC_BL_ICOL(a);
        case __FUNC_BL_FLSHWIN:         return calcnode_bl_function__FUNC_BL_FLSHWIN(a);
        case __FUNC_BL_GETGRPB:         return calcnode_bl_function__FUNC_BL_GETGRPB(a);
        case __FUNC_BL_SETPIX:          return calcnode_bl_function__FUNC_BL_SETPIX(a);
        case __FUNC_BL_FILLRECT:        return calcnode_bl_function__FUNC_BL_FILLRECT(a);
        case __FUNC_BL_DRAWRECT:        return calcnode_bl_function__FUNC_BL_DRAWRECT(a);
        case __FUNC_BL_DRAWLINE:        return calcnode_bl_function__FUNC_BL_DRAWLINE(a);
        case __FUNC_BL_RND:             return calcnode_bl_function__FUNC_BL_RND(a);
        case __FUNC_BL_WAIT:            return calcnode_bl_function__FUNC_BL_WAIT(a);
        case __FUNC_BL_COLOR:           return calcnode_bl_function__FUNC_BL_COLOR(a);
        case __FUNC_BL_LOCATE:          return calcnode_bl_function__FUNC_BL_LOCATE(a);
        case __FUNC_BL_GETPIX:          return calcnode_bl_function__FUNC_BL_GETPIX(a);
        case __FUNC_BL_WAITNF:          return calcnode_bl_function__FUNC_BL_WAITNF(a);
        case __FUNC_BL_INKEY1:          return calcnode_bl_function__FUNC_BL_INKEY1(a);
        case __FUNC_BL_CLS:             return calcnode_bl_function__FUNC_BL_CLS(a);
        case __FUNC_BL_INPTINT:         return calcnode_bl_function__FUNC_BL_INPTINT(a);
        case __FUNC_BL_INPTFLOT:        return calcnode_bl_function__FUNC_BL_INPTFLOT(a);
        case __FUNC_BL_SETMODE:         return calcnode_bl_function__FUNC_BL_SETMODE(a);
        case __FUNC_BL_FILLOVAL:        return calcnode_bl_function__FUNC_BL_FILLOVAL(a);
        case __FUNC_BL_DRAWSTR:         return calcnode_bl_function__FUNC_BL_DRAWSTR(a);
        case __FUNC_BL_OPENVWIN:        return calcnode_bl_function__FUNC_BL_OPENVWIN(a);
        case __FUNC_BL_SLCTWIN:         return calcnode_bl_function__FUNC_BL_SLCTWIN(a);
        case __FUNC_BL_COPYRCT0:        return calcnode_bl_function__FUNC_BL_COPYRCT0(a);
        case __FUNC_BL_COPYRCT1:        return calcnode_bl_function__FUNC_BL_COPYRCT1(a);
        case __FUNC_BL_DRAWPTRN_D:      return calcnode_bl_function__FUNC_BL_DRAWPTRN_D(a);
        case __FUNC_BL_DRAWPTRN_R:      return calcnode_bl_function__FUNC_BL_DRAWPTRN_R(a);
        }

        struct CalcNode cn = {.type = CNT_NOT_FOUND};
        return cn;
}
