bl_cls();

b = 39; c = 11;
bb = 1; cc = -1;
d = 37;

:L0
        bl_locate(b, c); bl_putc('O');

        bl_locate(d, 23); bl_putc('#'); bl_putc('#'); bl_putc('#'); bl_putc('#');

        if (c == 23) {goto L1;}

        bl_wait(100);

        a = bl_inkey1();

        bl_locate(b, c); bl_putc(' ');

        bl_locate(d, 23); bl_putc(' '); bl_putc(' '); bl_putc(' '); bl_putc(' ');

        if (b ==  0) {bb =  1;}
        if (b == 78) {bb = -1;}
        if (c ==  0) {cc =  1;}
        if (c == 22) and (d-1 <= b <= d+5) {cc = -1;}

        b = b + bb; c = c + cc;

        if ((a == 333) or (a == '6')) and (d < 73) {d = d + 2;}
        if ((a == 331) or (a == '4')) and (d >  1) {d = d - 2;}

        goto L0;

:L1
        bl_wait(-1);
