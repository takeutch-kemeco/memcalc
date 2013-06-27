x = 1;

gosub F;
gosub F;
gosub F;
goto END;

:F
        x = x + 1;
        return;

:END
        print(x);

