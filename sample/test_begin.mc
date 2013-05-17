goto MAIN;

F:
        if (i) {
                a = 5;
                print(a);
        } else {
                a = 6;
                print(a);
        }

        goto _F;

MAIN:
        i = 0;
        LOOP_START:
        goto F;
        _F:
        i = i + 1;
        if (i < 2) {goto LOOP_START;}
