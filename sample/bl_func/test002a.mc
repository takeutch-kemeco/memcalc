bl_openWin(256,256);

y = 0;
LY:
	x = 0;
	LX:
		c = bl_rgb(x, y, 0);
		bl_setPix(x, y, c);

		if (x < 256) {
			x = x + 1;
			goto LX;
		}

	if (y < 256) {
		y = y + 1;
		goto LY;
	}

bl_wait(-1);

