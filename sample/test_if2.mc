a = 0;



if 1 {
        if 1 {a = 123;}
}
print(a);
a = 0;



if 0 {
        if 1 {a = 0;} else {}
} else {
        if 1 {a = 123;}
}
print(a);
a = 0;



if 1 {
        if 1 {a = 123;} else {a = 0;}
} else {
        a = -1;
}
print(a);
a = 0;



if 1 {
        if 0 {a = 0;} else {a = 123;}
} else {
        a = -1;
}
print(a);
a = 0;



if 0 {
        a = -1;
} else {
        if 1 {a = 123;} else {a = 0;}
}
print(a);
a = 0;



if 0 {
        a = -1;
} else {
        if 0 {a = 0;} else {a = 123;}
}
print(a);
a = 0;
