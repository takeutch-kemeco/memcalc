a = 123;
b = 234;
c = a;

x = 0;
if (a == b) {x = a + b + c;}
print(x);

if (a != b) {x = a + b + c;} else {x = 0;}
print(x);

if (a == c) {x = 1;}
print(x);

if (a != c) {x = 1;} else {x = 1 + 2 + 3;}
print(x);
