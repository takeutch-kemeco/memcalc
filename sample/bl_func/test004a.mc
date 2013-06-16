bl_openWin(320, 200);

c[8];
c[0] = bl_rgb(0, 0, 0);
c[1] = bl_rgb(255, 0, 0);
c[2] = bl_rgb(0, 255, 0);
c[3] = bl_rgb(255, 255, 0);
c[4] = bl_rgb(0, 0, 255);
c[5] = bl_rgb(255, 0, 255);
c[6] = bl_rgb(0, 255, 255);
c[7] = bl_rgb(255, 255, 255);

i = 1;
L0:
	bl_setCol(c[i]);
	x = 0;
	L1:
		bl_fillRect(1, 200, x, 0);
		bl_fillRect(1, 200, 319 - x, 0);
		bl_wait(30);

		if (x < 320) {
			x = x + 2;
			goto L1;
		}

	i = (i + 1) & 7;
	goto L0;

