a = 1;

i = 10;

:L0
        if (i > 0) {goto L1;} else {goto L2;}
        :L1
                a = a * i;
                i = i - 1;
        goto L0;

        :L2
        print(a);
