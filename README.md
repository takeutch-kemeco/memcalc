## memcalc

memcalc は bison, flex の勉強しながら書いた副産物です。
簡単な機能しか備えていない、単純な言語です。

元々は単なる電卓ツールでしたが、括弧やメモリー機能等を追加してるうちに今の形になりました。

ライセンスはbison, flexの義理を感じるので GPL2 です。



### ビルド・インストール方法：
        bison, flex, autoconf, automake, libtool の入ってる環境にて

        ./autogen.sh

        ./configure
        make
        sudo make install

### アンインストール方法：
        sudo make uninstall



### サンプルコード：
        sample/ 以下にテストに使ったコードの残骸がいろいろ残してあります。



### 言語の全機能・仕様の説明：

        命令の最後には ; を付けます。
        C言語と同じです。


        全ての変数は大域変数となります。型は double 型のみです。
        C言語のようなインスタンス宣言は必要ありません。
        例：
                a = 1;

                a = 1.4142;

        演算は四則演算(+, -, *, /)と、
        モジュロ(%)と、
        ビットシフト(<<, >>)と、
        ビット演算(AND = &, OR = |, XOR = ^, NOT = ~)と、
        比較演算(==, !=, <, >, <=, >=) を行えます。
        
        また、これら演算の順序は、括弧を用いて調整することができます。
        例:
                a = (1 + 2) - 3 * (4 / 5);

                a = (((1 + 2) - 3) * (4 / 5)); 

        変数は全て内部的に倍精度浮動小数として扱いますが、
        ビットシフトの場合は小数切り捨てで整数化した値に対してシフトを行った結果が返ります。

        シングルクオートで囲むと、文字リテラルとして解釈し、アスキーコード数値を変数へ代入できます。
                a = 'A';

        なお、サポートする特殊文字は、以下のみです。
                a = '\n';

                a = '\0';

                a = '\'';

                a = '\"';

                a = '\\';



        a[100]のように、１次元配列を扱えます。
        最初に a[100] 等とすると、これが a[100] の配列宣言となります。
        これによって a[0] ～ a[99] までの配列を得られます。
        例：
                a[100];

                abc[42] = 1;
                注意：この場合、abc[42] を確保した後に、abc[42] へ 1 を代入してしまいます。

                配列宣言後は、配列に代入できます。
                例：
                        a[0] = 3.14;

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



        名前の最後に : を付けるとラベルになります。
        goto のジャンプ先の行として使います。
        （ラベルの最後には ; は不要です）
        例：
                L:

                label:



        goto ラベル名;
        によって、その行へジャンプできます。
        例：
                goto L;

                goto label;



        if 条件 { 命令1; 命令2; ... } else { 命令1; 命令2; ... }
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
        注意： { } で囲むのが必須となりました。
        　　　C言語のような if a==1 print(1); のような、{} を省略した書き方はサポートしません。



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


        画面解像度を自動取得して対応するようにしました。
        そのため 1920 * 1080 以外の画面モードでも動作します。
        しかし、色モードは 32bit カラーで決め打ちしてるので、16bitカラーや8bitカラーには未対応です。
        また、VRAMのエンディアンは ＜HSB側 #00RRGGBB LSB側＞ で決め打ちしてあります。これが異なるVRAMも未対応です。

        未対応の主な理由は、作者はそれをテストできるハードを持っていないので、テストできないので書く気が起こらないからです。
        また、持ってないので必要性も感じないからです。
        だれかそのようなハードを持ってる人がいたら、それに対応するコードをGPL2ライセンスでのパッチで提供してほしいです。
        そんな奴が居るとはまったく期待していませんが。



以上。
できることはこれだけです。

