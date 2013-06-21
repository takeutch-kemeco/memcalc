i = 0;

:L1
        print(i);
        i = i + 1;
        if (i < 10) {goto L1;} else {i = 234;}

print(i);
