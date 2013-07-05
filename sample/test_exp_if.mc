x = 5;
a = x;
:L0
	x = x - 1;
	a = (x <= 1) -> a :: a * x;
	if (x > 0) {goto L0;}

print(a);

