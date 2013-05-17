MAIN:
        a = 1;

        i = 0;

        LOOP_TOP:
        goto F;
        _F:
        i = i + 1;
        print(i);
        if (i < 10) {goto LOOP_TOP;}

        print(123);
        goto END;

F:
        a = a * 2;
        print(a);
        goto _F;

END:
