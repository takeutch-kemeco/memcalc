a[10];

i = 0;
:L
        a[i] = i;
        if (i < 10) {i = i + 1; goto L;}

i = 0;
:L2
        print(a[i]);
        if (i < 10) {i = i + 1; goto L2;}
