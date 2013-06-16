bl_openWin(256, 256);

c = bl_rgb(255, 255, 0);
bl_setCol(c);

BL_PXOR = 7;

bl_setMode(BL_PXOR);

x = 0;
L0:
	bl_drawLine(0, 0, x,   255);
        bl_drawLine(x, 0, 255, 255);

 	if (x < 256) {
		x = x + 1;
		goto L0;
	}

bl_wait(-1);

