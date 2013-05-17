a = 123;
if (a == 123) {goto L1;}

print -1;

L1:
print(a);

if (a != 123) {} else {goto L2;}

print -1;

L2:
print(a);
