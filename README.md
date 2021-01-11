# BEP_LoRaWAN　マニュアル

## 目次
1. [概要](#anchor1)<br>
2. [LoRaWAN](#anchor2)<br>
3. [使用したLoRaゲートウェイ、デバイス](#anchor3)<br>
4. [The Things Network](#anchor4)<br>
5. [プログラム](#anchor5)<br>
6. [LoRa Miniの使い方](#anchor6)<br>
7. [放流結果](#anchor7)<br>
8. [今後の展開](#anchor8)<br>
9．[参考文献](#anchor9)<br>

<a id="anchor1"></a>
## 概要
2020年9月から2021年にかけて、e-kagakuアカデミーは[琵琶湖水環境プロジェクト「BEP Environment Project 2020」](https://global-science.or.jp/biwako-environment-project/)（以下BEP2020）を行った。本プロジェクトは小学生から高校生にわたる参加者が一人一つ琵琶湖の水環境などを測定する観測機を製作し、琵琶湖に放流してデータを集め、MATLABなどの分析ツールをもちいて分析するというものである。2020年から2021年にかけてのコロナウイルスの流行を受けて、月2回ほどオンラインでの作業日を設けつつ、各自が自宅で観測機の製作を進める形となった。

プロジェクトの参加者が製作を進めるのと同時並行で、当会のサポーター4名でサポーター機の製作を行った。プロジェクト参加者の観測機はArduinoにセンサーを繋げ、SDカードにセンサー値を記録するというものだが、サポーター機ではLoRaWANを用いてリアルタイムで観測したデータを送受信できるデバイスを作成した。本稿ではLoRaWANに焦点を当てて、作成したデバイスについてまとめる。


<a id="anchor2"></a>
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

<a id="anchor3"></a>
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

<a id="anchor4"></a>
## The Things Network
### The Things Network 概要
[The Things Network（TTN）](https://www.thethingsnetwork.org/country/japan/)はLoRaWANのサービスを提供している情報共有プラットホームサービスである。LoRaWANシステムモデルの図の、ネットワークサーバーやユーザーアプリケーションを提供していると考えればよい。<br>
今回のサポーター機の開発にあたり、このThe Things Networkを利用した。以下に登録手順や使い方を述べる。
### The Things Networkの準備
#### アカウント作成
1．[https://www.thethingsnetwork.org/country/japan/](https://www.thethingsnetwork.org/country/japan/)にアクセス<br>
2．「Sign Up」からユーザー名とメールアドレス、パスワードを入れて「Create account」 <br>
3．認証メールが来るので、「Activate Account」をクリック、ログインできる <br>
#### コンソール設定
1．「CONSOLE」をクリックし、右上のユーザー名をクロックして「Settings」＞「Language」から言語を日本語にして「Save」で日本語化！<br> 
2．同じく「設定」から「ハンドラー」＞「ttn-handler-asia-se」を選択し「保存」をしておきましょう<br>
#### ゲートウェイ登録
1．「CONSOLE」＞「ゲートウェイ一覧」＞「ゲートウェイを登録」で登録画面へ <br>
2．「I’ｍ using the legacy packet forwarder」にチェックを入れる <br>
　　すると、「ゲートウェイID」が「ゲートウェイEUI」に変わるはず<br>
3. 以下の各項目を入力して、「ゲートウェイ登録」をクリック<br>
<span style="color : red">要確認</span>
|項目|入力内容|
|----|-------|
|ゲートウェイEUI |LPS8-JPのMACアドレス（裏側のシールに記載）|
|記述|ゲートウェイの名前 |
|周波数計画 |Asia 923-925MHz |
|Router |ttn-router-asia-se |
|場所 |地図上で設置場所をダブルクリック |
|アンテナ代替え|アンテナ設置場所が屋内ならインドア、屋外ならアウトドア |
4. 右上の「設定」をクリックして詳細を確認しよう<br>

|項目|内容|
|---|----|
|プライバシー |公開範囲の設定、「公共ステータスに変更」でよい |
|情報 |ブランドにDragino、モデルにLG01-JP、アンテナにN/Aと入れる |
|コラボレータ |他のユーザーを各権限で追加できる（知らない人に注意）、今は無視 |

#### アプリケーションの登録
#### デバイスの登録
### ゲートウェイの準備
### エンドノードの準備
### DataStorageによるデータのストレージ
### IFTTT MakerによるWebサービス連携


<a id="anchor5"></a>
## プログラム

<a id="anchor6"></a>
## LoRa miniの使い方

<a id="anchor7"></a>
## 放流結果

<a id="anchor8"></a>
## 今後の展開
* LoRaWANゲートウェイ：LG01-JP (Dragino)  &yen; 11,142 <br>
KKHMF LG01-P オープンソースLoRaゲートウェイ 915 MHz
　[https://www.amazon.co.jp/dp/B0784RDBPT](https://www.amazon.co.jp/dp/B0784RDBPT)

<a id="anchor9"></a>
## 参考文献
1．The Things Network　「LoRaWAN」をみんなでシェアして使う（工学社）
