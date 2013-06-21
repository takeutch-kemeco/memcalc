goto MAIN;

:AAA
        a = a + 1;
        print(a);
        goto MAIN;

:MAIN
        if (a == 3) {goto END;} else {goto AAA;}

:END
print(123);
