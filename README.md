# BEP_LoRaWAN　マニュアル

## 概要
2020年9月から2021年にかけて、e-kagakuアカデミーは[琵琶湖水環境プロジェクト「BEP Environment Project 2020」](https://global-science.or.jp/biwako-environment-project/)（以下BEP2020）を行った。本プロジェクトは小学生から高校生にわたる参加者が一人一つ琵琶湖の水環境などを測定する観測機を製作し、琵琶湖に放流してデータを集め、MATLABなどの分析ツールをもちいて分析するというものである。2020年から2021年にかけてのコロナウイルスの流行を受けて、月2回ほどオンラインでの作業日を設けつつ、各自が自宅で観測機の製作を進める形となった。

プロジェクトの参加者が製作を進めるのと同時並行で、当会のサポーター4名でサポーター機の製作を行った。プロジェクト参加者の観測機はArduinoにセンサーを繋げ、SDカードにセンサー値を記録するというものだが、サポーター機ではLoRaWANを用いてリアルタイムで観測したデータを送受信できるデバイスを作成した。本稿ではLoRaWANに焦点を当てて、作成したデバイスについてまとめる。

## LoRaWAN
### LoRaWANとは
LoRaWANはLPWA（Low Power Wide Area、省電力長距離通信）の一種であり、Semtech社のLoRa変調という技術に基づいた通信方式である。WiFiなどの2.4 GHz帯や5.0 GHz帯とは異なり、主に920 MHz帯を使っており、振動数が小さいことからわかる通り、通信速度は遅いがWiFiよりも回折しやすく、長距離通信が可能である。通信距離としては、建物が林立していなければ1 km程度は安定して通信可能であり、条件が良ければ数十キロメートル離れていても通信可能である。

### なぜLoRaWANか？
LoRaWANを使うメリットとしては以下の3つがある。
* 通信距離が長い
* 送受信できるデバイスがあれば、通信料は無料
* LoRaWANが使用する920 MHz帯は無線免許がいらない（使用するデバイスが技適を取得している必要はある）

WiFiと比べ通信速度が遅いというデメリットがあるが、以上のような理由によりIoTデバイスに用いる通信手段として適しており、今後ますます重要性が増すことが予想される。

以下の表は、WiFiとLoRaWANを対比させたものである。
| | WiFi(5G)| LoRaWAN|
|---|----|----|
| 通信距離|数100 m|数十 km|
|月額料金|4,000-5,000円|無料|
|一度に送信可能なデータ量| 数GB| 数kB|
|通信速度|超高速|低速|
### LoRaWANシステム
一般的なLoRaWANシステムは、以下のようになっている。
![LoRaWANシステムモデル](/image/LoRaWANsystemmodel.jpg) 
出典：[https://monoist.atmarkit.co.jp/mn/articles/1709/19/news004_3.html](https://monoist.atmarkit.co.jp/mn/articles/1709/19/news004_3.html)

図の一番左側にあるLoRaWANデバイス（エンドノード、センサーノードとも）が末端デバイスであり、データの取得をしてLoRaゲートウェイを介してLoRaWANネットワークサーバーに送信し、そのデータを解析してユーザアプリケーションに表示する。

## 使用したLoRaゲートウェイ、デバイス
今回はDragino社のLoRaモジュールを用いた。
* LoRaWANゲートウェイ：LPS8-JP (Dragino) &yen; 28,380<br>
  普及型8チャンネル LoRaWANゲートウェイ LPS8-JP【技適取得済み】 [http://sales.dragino.jp/ca1/7/p0-r-s/](http://sales.dragino.jp/ca1/7/p0-r-s/)
* LoRaWANデバイス1：LoRa Mini Dev-JP (Dragino)  &yen; 15,875<br>
　[https://jp.rs-online.com/web/p/communication-wireless-development-tools/1883151/](https://jp.rs-online.com/web/p/communication-wireless-development-tools/1883151/)
* LoRaWANデバイス2：LoRa Mini (Dragino) &yen; 10,164<br>
 [https://jp.rs-online.com/web/p/communication-wireless-development-tools/1883152/](https://jp.rs-online.com/web/p/communication-wireless-development-tools/1883152/)
 
LoraWANゲートウェイLPS8-JPの日本語版マニュアルは以下のURLから参照できる。
[https://www.slideshare.net/copstalk/8lorawan-lps8jp-v120jp](https://www.slideshare.net/copstalk/8lorawan-lps8jp-v120jp)
<img src="/image/LPS8-JP.jpg" width="400">
### LoRa Mini Dev-JPとLoRa Miniの違い
LoRa MiniはArduinoにも搭載されているAVR社のマイコン、ATmega328PとSX1276/78というLoRaWAN送受信用のモジュールを組み合わせたモジュールである。ATmega328Pには予めArduino UNO用のブートローダーが入っており、Arduino UNOとほぼ同じようにArduino IDEで開発できる。ただ、写真の通りピンやピンソケットがついていないため、プログラムのダウンロードやセンサーとの接続にははんだ付け作業が必要である。<br>
一方でLoRa Miniを開発ボード化したものがLoRa Mini Dev-JPであり、USBによるプログラムのダウンロードやジャンパーワイヤーを使ったセンサーの動作確認などができるようになっている。今回のサポーター機の作成では、まずLoRa Mini Dev-JPが使えるか確認してから、LoRa Miniを使用してみた。<br>
LoRa Mini
<img src="/image/LoRa_Mini.PNG" width="400">
LoRa Mini Dev-JP
<img src="/image/LoRa_Mini_Dev-JP.PNG" width="400">
## The Things Network
### The Things Network 概要
The Things Network（TTN）は
### The Things Networkの準備
#### アカウント作成
#### コンソール設定
#### ゲートウェイ登録
#### アプリケーションの登録
#### デバイスの登録
### ゲートウェイの準備
### エンドノードの準備
### DataStorageによるデータのストレージ
### IFTTT MakerによるWebサービス連携



## プログラム

## LoRa miniの使い方

## 放流結果

## 今後の展開
* LoRaWANゲートウェイ：LG01-JP (Dragino)  &yen; 11,142 <br>
KKHMF LG01-P オープンソースLoRaゲートウェイ 915 MHz
　[https://www.amazon.co.jp/dp/B0784RDBPT](https://www.amazon.co.jp/dp/B0784RDBPT)

## 参考文献
1．The Things Network　「LoRaWAN」をみんなでシェアして使う（工学社）
