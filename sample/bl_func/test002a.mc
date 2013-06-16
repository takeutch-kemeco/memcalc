y = 0;
LY:
	x = 0;
LX:
		c = (x << 16) | (y << 8) | 0;
		bl_setPix(x, y, c);

		if (x < 250) {
			x = x + 1;
			goto LX;
		}

	if (y < 250) {
		y = y + 1;
		goto LY;
	}

bl_wait(-1);

