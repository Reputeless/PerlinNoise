# Perlin Noise C++11

Ken Perlin の ImprovedNoise (<http://mrl.nyu.edu/~perlin/noise/>) を C++11 で実装しました。  
Visual Studio 2012 で動作を確認しています。

リファレンス実装に加えて
* 乱数シード設定
* 1D, 2D ノイズ
* オクターブノイズ

の機能を追加しました。  
  
Sample.cpp を実行すると、入力したパラメータで雲模様の画像を作成します。  
入力例
* frequency = 8.0
* octaves = 8
* seed = 12345

![f8o8_12345](https://lh4.googleusercontent.com/-YGjRmYg8E3I/UTM4CTD_KyI/AAAAAAAACDI/RPKF_r_XOfA/s800/f8o8_12345.png)
