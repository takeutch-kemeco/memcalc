a[1000000];	// 8MB

i = 0;
L1:
if i < 1000000 {
	a[i] = i;
	i = i + 10;
	goto L1;
}

i = 0;
L2:
if i < 1000000 {
	print(a[i]);
	i = i + 10;
	goto L2;
}

