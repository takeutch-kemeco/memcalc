## memcalc

memcalc は簡単な機能しか備えてない単純なスクリプト言語です。

元々は単なる電卓ツールでしたが、括弧やメモリー機能（変数）等を追加してるうちに今の形となりました。



memcalcでは比較演算を連鎖的に記述できます。
たとえば一般的な言語で if (0 < a && a < 1) と記述するのと同様のことを、memcalcでは if 0 < a < 1 と一息に記述できます。

memcalc では数は全て暗黙に複素数として扱われます。

たとえば a = 1; とした場合は、実際には 1 + i0 な複素数（実数１で虚数０）が a へ代入されます。

虚数も指定したい場合は演算子 :+ を用いて i = 0 :+ 1; などとします。
すなわち複素平面のx,yによる指定です。
この場合は i は虚数単位となります。

また、複素数を絶対値と偏角によって指定したい場合は :< 演算子を用いて x = 1 :< 3.14; 等とします。
これは phasor 形式による表現 r ∠ θ  を模してます。
この場合は x は -1 となります。

また、勿論これら :+ と :< 演算子に、変数を引数として a = b :+ c; や x = y :< z; とすることも可能です。
仮に x = 1 :+ 2; であれば、a = x :+ x; とした場合は a = 1 :+ 1; と同様です。（a = 1 :+ 2; とはならないので注意）

a が変数だとして、絶対値を得たい場合は |a| とします。結果は実数です。

a の複素共役(conjugate)を得たい場合は ~a とします。これは変数の上にバーを書く記法を模しています。

a の極座標における偏角(argument)を得たい場合は、arg a; とします。結果は実数です。

すなわち理想としては、a == (|a|) :< (arg a) です。しかし実際には計算誤差の為に一致しないかもしれません。

a の複素平面における実数値を得たい場合は Re a とします。（注：名前の大文字と小文字は区別します）
また、同様に虚数値を得たい場合は Im a とします。これらは数学における書式を模しています。

a の b 乗(power)を得たい場合は a ^ b; とします。

平方根を求める関数は特には用意していませんが、√はすなわち1/2乗なので a ^ (1/2) とすることでで平方根を求められます。

現時点では三角関数が未実装なのでせっかくの複素数のおいしい所を生かしきれません。
これら関数は近日中に実装したい最優先事項と考えてます。



画面描画とキー入力のバックエンドとして blike を利用しています。
その為、ビルドには別途 blike が必要となります。

blike の入手は github から以下のアドレスです:

        git clone git://github.com/takeutch-kemeco/blike.git

いくつかの bl_ 関数は未実装です。
未実装関数の状況とくに文章化はしてません。ソースコード src/memcalc.bison.y を直接読んで判断してください。

ライセンスはbison, flexの義理を感じるので GPL2 です。



### ビルド・インストール方法：
        bison, flex, autoconf, automake, libtool, blike の入ってる環境にて

        ./autogen.sh

        ./configure --prefix=/usr
        make
        sudo make install

### アンインストール方法：
        sudo make uninstall



### サンプルコード：
        sample/ 以下にテストに使ったコードの残骸がいろいろ残してあります。
        sample/bl_func 以下に bl_ 関数のテストに使ったコードの残骸がいろいろ残してあります。



### 言語の全機能・仕様の説明：

        命令の最後には ; を付けます。
        C言語と同じです。


        全ての変数は大域変数となります。型は double による複素数型のみです。
        また、変数の使用に際して、C言語のようなインスタンス宣言は必要ありません。

        memcalcでは数は全て暗黙に複素数として扱われます。
        a = 1; とした場合でも、それは実際には 1 + i0 の複素数として認知されます。
        虚数部も指定したい場合は、演算子として :+ か :< を用います。
        :+ は複素平面による指定で、実数 :+ 虚数 として用います。
        :< は極座標による指定で、絶対値 :< 偏角 として用います。
        例：
                a = 1.4142;

                a = 1 :+ 2;

                a = 3 :< 4;

                r = 1;
                theta = 3.14;
                a = r :< theta;

                また、同じ値の代入であれば以下のように数珠つなぎで書くこともできます。
                a = b = c = 1;

        a の絶対値(absolute)を得たい場合は |a| とします。結果は実数です。
        a の複素共役(conjugate)を得たい場合は ~a とします。
        a の極座標における偏角(argument)を得たい場合は arg a とします。結果は実数です。
        a の複素平面における実数値を得たい場合は Re a とします。結果は実数です。（名前の大文字・小文字は区別します）
        a の複素平面における虚数値を得たい場合は Im a とします。結果は実数です。（名前の大文字・小文字は区別します）
        a の b 乗(power)を得たい場合は a ^ b; とします。
        例：
                a = 1 :+ 1;
                b = (|a|) :< (arg a);
                理想としては a と b は同じ値になるはずですが、
                実際には内部的に数値計算を経ているので、その際に生じる計算誤差によって完全には一致しないかもしれません。

                c = 3 :+ 4;
                d = (Re c) :+ (Im c);
                この場合は c と d は内部的には数値計算を経ていないので同じ値になるはずです。

                pi = 3.14159265358979;
                e = (1 + (1/100000)) ^ 100000;
                i = 0 :+ 1;
                e ^ (pi * i);
                理想としては　-1 ですが、実際にはπもeも計算精度も不足してる為に -1 には完全には一致しないはずです。

                平方根を求める関数は特には用意していませんが、√はすなわち 1/2乗なので、冪乗で代用できます。
                2 ^ (1/2);
                これでも 1.4142... を得られます。

        四則演算(+, -, *, /)を行えます。
        例：
                1 + 2;

                a = 1;
                a - a;

                b = a - 3;
                c = b * a;

                d = c / c

        モジュロ(%)が申し訳程度に行えますが、おそらく数学的に正しい計算ではない予感がするのでたぶん近日中に修正されるかもしれません。
        実数同士、虚数同士で、それぞれ独立に余り算を行っています。
        複素数における本来の正しいモジュロ演算のやり方を知らないので、こんなことになってます。
        例：
                a = 11 % 2;

        ビットシフト(<<, >>)を行えます。
        << が左シフト、>> が右シフトなのは一般的な言語の慣例に従ってます。
        これは内部的には実数、虚数、それぞれを符号無し64bit型へ型変換した上での論理シフトを行ってます。
        これが複素数における本来の数学的に正しい計算なのかどうかは知りません。
        例：
                a = 1 << 8;

                x = 1;
                y = 8;
                z = x << y;

        ビット演算(and, or, xor, not)を行えます。
        例：
                a = 123 or 456;
                a = 255 and 123;
                a = 1 xor 3;
                a = not 1024;

        また、これら演算の順序は、括弧を用いて調整することができます。
        例:
                a = (1 + 2) - 3 * (4 / 5);

                a = (((1 + 2) - 3) * (4 / 5));



        比較演算(==, /=, <, >, <=, >=)を行えます。
        memcalcの比較演算は、一般的な言語のそれとは少し異なっていて、より数学の記述に近いです。

        C言語等の一般的な言語の慣例では、数学でよく見かける条件式 0 < a < 1 を表現する場合に、0 < a && a < 1 のように書く必要がありました。
        しかし、この書式には前々から個人的に疑問を感じてました。
        もっと数学と同じように 0 < a < 1 とシンプルに書けた方が簡単なはず。
        そこで、memcalc ではそのように書けるようにしました。条件式を 0 < a < 1 のように書けます。
        例：
        　　　　　　　　if 0 < a < 1 {print(1);}
                これは内部的には、まず最初に a < 1　を行い、次に 0 < a を行い、それら結果の AND を採ります。
                つまり、最後側から判定されてきて、比較演算子で連結する変数同士でそれぞれ判定されてきます。

                if a < b > c == d /= e {print(1);}
                この場合も同様に、最後からペア同士で連鎖的に判定してきて、それら結果の AND を採ります。
                具体的には以下のように書くのと同じ意味です。
                        if (d /= e) and (c == d) and (b > c) and (a < b) {print(1);}

                なお、条件式の連鎖的な記述は、括弧によって清算されて真偽値と成ります。
                つまり括弧を隔てた場合は、連鎖されません。
                例：
                        0 < 1 < 2
                        これの値は真（つまり 1）となります。
                        従って、以下は偽となります。
                        0 == (0 < 1 < 2)
                        これは 0 == 1 という意味になるので、偽（つまり 0）です。

                        つまり、以下の書き方とは意味が異なります。
                        0 == 0 < 1 < 2
                        この場合であれば真です。

                        0 < (1 < 2)
                        と
                        0 < 1 < 2
                        この違いに注意してください。
                        連鎖は各個を隔てた時点で清算されて真偽値となり、以降は連鎖されない。ということです。

                        もちろん、これは連鎖できます。
                        (1 < 2 < 3) == (4 < 5 < 6) /= (7 > 8 > 9)
                        これは
                        1 == 1 /= 0
                        と連鎖による解釈がされて、真（つまり 1）を返します。

                        そういうことです。

        注意：ノットイコールの書式を最近変更したので注意してください。
        以前はC言語のような != でしたが、変更してHaskellのような /= となりました。

        真ならば非０（実際は１）を返し、偽なら０を返します。それは一般的な慣例と同様です。
        ただし、比較は実数のみで比較します。（”虚数は無視”して判断します）。




        シングルクオートで囲むと、文字リテラルとして解釈します。
        これは実数値によるアスキーコード数値として内部的に認識されます。
        例：
                a = 'A';

        なお、サポートする特殊文字は、以下のみです。
                a = '\n';

                a = '\0';

                a = '\'';

                a = '\"';

                a = '\\';



        a[100]のように、１次元配列を扱えます。
        単一の変数の場合と異なり、配列の場合は最初に明示的なインスタンス宣言が必要となります。
        最初に a[100] 等とすると、これが a[100] の配列宣言とな、以降は a[0] ～ a[99] までの配列として扱えます。
        例：
                a[100];
                a[1] = 1;
                a[2] = 10;

                abc[42] = 1;
                注意：この場合、abc[0] ～ abc[41] が確保されると同時に、abc[42] へ 1 を代入してしまいます。

                配列宣言後は、配列に代入できます。
                例：
                        a[0] = 1 :+ 3.14;

                        a[200] = 0.71;

                        注意：配列宣言が a[100] だったとしたら、a[0] ～ a[99] までの範囲ですが、
                        　　　a[200] = 0.71; などとしても、エラーは出しません。

                        　　　内部的にはスタック方式で変数領域を確保してるので、
                        　　　a[200] = 0.71; とした場合、スタックの先にあるそこに 0.71 を代入します。
                        　　　例：
                                        a[4];
                                        b[4];

                                        a[5] = 0.71;

                                        /*
                                        この場合、メモリー上での確保領域は

                                        a[0], a[1], a[2], a[3], b[0], b[1], b2[2], b[3]

                                        となります。
                                        そのため a[5] は、b[1] を指すので、b[1] = 0.71; と同じです。
                                        */

                                領域オーバー時の対応のような親切な処理は、あえて何も行いません。
                                これは、わざと、このようにしてあります。



        名前の最初に : を付けるとラベルになります。

        注意：以前は名前の”最後”に : を付けるとラベルになってましたが、書式が変わったので注意してください。
        : は最初に付けるように変わったのでので注意してください。

        ラベルは goto のジャンプ先の行として使います。
        ラベルの最後には ; は不要ですが、付けてもエラーにはなりません。
        また、一行に複数命令を詰め込む場合は、ラベルの最後に ; を付けた方が読み易くて良いかもしれません。
        例：
                :L0

                :label;

                :L1; :L2; :L3;



        goto ラベル名;
        によって、その行へジャンプできます。（この場合は最後の ; は必要です）
        例：
                goto L0;

                goto label;



        if 条件式 { 命令1; 命令2; ... } else { 命令1; 命令2; ... }
        条件式が非０の場合は then 側を実行し、０の場合は else 側を実行します。これは一般的な慣例と同様です。

        分岐先には複数命令を書けます。
        例：
                if a == 1 {
                        b = 1;
                        c = 2;
                        d = 3;
                } else {
                        b = 0;
                        c = 0;
                        d = 0;
                }

                入れ子にもできます
                例：
                        if a == 1 {
                                if b == 1 {
                                        goto label_a;
                                        b = 0;
                                } else {
                                        goto label_b;
                                        b = 0;
                                }
                        } else {
                                a = 1;
                        }

        条件式は特別な値ではなく、単なる”０か非０か”の数値なので、条件式の合成は、単なるビット演算によって行えます。
        例：
                if ((a == 1) or (b == 1)) and (c == 1) {print(1);}

        備考：if文の条件式を括弧で囲んだ方が気持ちいい人は、囲んでも構いません。書式的にも問題ありません。

        注意： { } で囲むのが必須となりました。
        　　　C言語のような if a==1 print(1); のような、{} を省略した書き方はサポートしません。

        注意２：条件判定に用いる比較演算は、現時点では複素数の実数値のみを判定します。
        　　　　その為、虚数値での比較を行いたい場合は Im 演算子によって虚数値を実数値にしてから比較する必要があります。
                例：
                        a = 1 :+ 2;
                        b = 2 :+ 2;
                        // 虚数値同士での比較を行いたい
                        if (Im a) == (Im b) {print(0)} else {print(-1);}



        /* 文字列 */

        // 文字列（改行まで）

        c スタイル、および c++ スタイルのコメントが書けます。
        例：
                a = 1; // ここにコメントを書く

                /* デバッグなどで複数行を手軽に無効にしたい場合等
                a = 1;
                b = 2;
                c = 3;
                */



        print(変数);
        変数の数字を画面に表示します。
        数字しか表示できません。そんなもんです。
        例：
                a = 1;
                print(a);



        putchar(変数);
        アスキーコード数値に対応する文字を、一文字、画面へ表示します。
        例:
                a = 'A';
                putchar(a);



        putpixel(x座標, y座標, 色相, 彩度, 明度);
        画面に１ピクセルの点を打ちます。
        色相は 0 ～ 2π 、彩度、明度はそれぞれ 0 ～ 1 の範囲で指定します。
        例：
                putpixel(100, 200, 0, 1, 1);
                （x = 100, y = 200 の座標に、RGBが #FF0000 の点を打ちます）

                putpixel(100, 200, 3.14, 1, 1);
                (x = 100, y = 200 の座標に、RGBが #00FFFF の点を打ちます）

                putpixel(100, 200, 0, 0, 1);
                (x = 100, y = 200 の座標に、RGBが #FFFFFF の点を打ちます)
                (白は、彩度が0で明度が1です。この場合は色相は何でも白です)

                putpixel(100, 200, 0, 0, 0);
                (x = 100, y = 200 の座標に、RGBが #000000 の点を打ちます)
                (黒は、明度が0です。この場合は色相と彩度は何でも黒です)


        各種 bl_ 関数;
        基本的にC言語から blike 関数を使う場合と同様の引数で利用できます。
        （ただし、まだ多くの bl_　関数が未実装なので、実際に動作するのはごく一部です）
        利用可能な bl_ 関数の詳細は、src/memcalc.bison.y のソースコードを直接読んで判断してください。



以上。
できることはこれだけです。
