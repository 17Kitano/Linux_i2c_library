このライブラリ群はラズパ等のLinuxベースのボードで
I2Cを使う為の物です。

今の所ラズパイ3・4で動作確認済みです。


ファイル内訳

i2c_library.c --- C言語用

i2c_library_class.cpp --- C++用にclassに書き換えた物

i2c_library_class_py.cpp --- Pythonから使えるようにboostライブラリで書いた物

i2c_fancs.so --- ラズパイでi2c_library_class_py.cppをコンパイルした物。pythonにimportして使える

i2c_library_test.py --- i2c_fancs.soのテストプログラム