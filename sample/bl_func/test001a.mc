s[10];
s[0] = 'c';
s[1] = 'o';
s[2] = 'l';
s[3] = 'o';
s[4] = 'r';
s[5] = ' ';
s[6] = ':';
s[7] = ' ';
s[8] = '\n';

bl_color(7,0);

i = 0;
L0:
	bl_color(i, 0);
	bl_locate(0, i);

	j = 0;
	L1:
		bl_putc(s[j]);

		if (j < 9) {
			j = j + 1;
			goto L1;
		}

	if (i < 15) {
		i = i + 1;
		goto L0;
	}

bl_wait(-1);

