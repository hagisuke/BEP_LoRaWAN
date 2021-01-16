# BEP_LoRaWAN　マニュアル

## 目次
1. [概要](#anchor1)<br>
2. [LoRaWAN](#anchor2)<br>
3. [使用したLoRaゲートウェイ、デバイス](#anchor3)<br>
4. [The Things NetworkとLoRa](#anchor4)<br>
5. [プログラム](#anchor5)<br>
6. [LoRa Miniの使い方](#anchor6)<br>
7. [放流結果](#anchor7)<br>
8. [今後の展開](#anchor8)<br>
9. [参考文献](#anchor9)<br>

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
 
LoraWANゲートウェイLPS8-JPの日本語版マニュアルは以下のURLから参照できる。<br>
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
## The Things NetworkとLoRa
### The Things Network 概要
[The Things Network（TTN）](https://www.thethingsnetwork.org/country/japan/)はLoRaWANのサービスを提供している情報共有プラットホームサービスである。LoRaWANシステムモデルの図の、ネットワークサーバーやユーザーアプリケーションを提供していると考えればよい。<br>
今回のサポーター機の開発にあたり、このThe Things Networkを利用した。以下に登録手順や使い方を述べる。
### The Things Networkの準備

<details><summary>詳細を表示</summary><div>

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
1. 「CONSOLE」＞「アプリケーション一覧」＞「アプリケーションを追加」で移動<br>
2. 以下の各項目を入力して、「アプリケーション追加」をクリック

|項目|入力内容|
|---|----|
|アプリケーションID |任意のIDを設定 |
|記述 |アプリケーションの名前 |
|アプリケーションEUI |アプリケーション追加時に自動で設定される |
|ハンドラー登録 |ttn-handler-asia-se |

#### デバイスの登録
1. 「アプリケーション一覧」から登録したアプリケーションを選択 <br>
  ※右上の「設定」をクリックすると「EUI」「コラボレータ」「アクセスキー」の設定が可能 <br>
2. 上のメニューの「デバイス」をクリックしてデバイス欄のデバイス登録に移動<br>
3. 以下の各項目を入力して、「登録」をクリック

|項目|入力内容|
|---|----|
|デバイスID |任意のIDを設定 |
|デバイスEUI |8バイトのキー、マークをクリックすると自動生成されるはず|
|App Key |16バイトのキー、入力しなければ自動生成されるのでOK |
|App EUI |アプリケーションに登録されている「APP EUI」を選択 |

4. 右上の設定をクリックして詳細を確認しよう<br>
  ※下の方のサンプルコードは後で使う 

|項目|内容|
|---|---|
|一般 |「記述」にデバイスの名前を付ける、他にもいろいろ設定可能 <br>※アクティベーション方式はOTAA、ABPどちらか選択（プログラムがABP版とOTAA版で異なるので、どちらを選択したか覚えておく。詳細ｈは「プログラム」で後述） |
|場所|地図上でのデバイスの位置に加え、高度もメートル単位で設定可能 |

</div></details>

### ゲートウェイの準備
<details><summary>詳細を表示</summary><div>

※WiFiかLANでパソコンと通信できることを前提とする <br>
1. LG01-JPのIPアドレス（デフォルトは「10.130.1.1」）をブラウザに入力し、設定画面へ<br>
2. Username（デフォルトは「root」）とPassword（デフォルトは「dragino」）を入力し、Login<br>
  ※Login後、Passwordは変更すべき（上部メニューの「System」＞「Administration」から変更できる ）<br>
3. 上部メニューの「Internet」＞「Internet Access」をクリックし、ネット接続設定をする<br>
4. TeraTermなどのSSHクライアントでLG01-JPに接続、「ping [www.openwave.co.jp](www.openwave.co.jp) 」などと打ち込み通信できることを確認する（FQDN指定でpingを実行している）<br>
5. Web設定画面に戻り、最初の画面の「Firmware Version」が「IoT-4.3.0」であるか確認 <br>
  ※違う場合、以下の手順に従う<br>
  5-1. オープンウェーブHPの下の方の「ダウンロード」をクリック<br>
  5-2. 「ファームウェア/ドライバー」の中の、 「LG01-JP/OLG01-JPファームウェア（バージョン：IoT-4.3.0）」をダウンロード <br>
  5-3. Web設定画面上部メニューの「System」＞「Backup/Flash Firmware」を開く<br>
  5-4. 「Flash new firmware image」の「ファイルを選択」から先ほどダウンロードしたものを選択、「Flash Image」をクリック<br>
6. 上部メニューの「Sensor」＞「IOT Server」を開く<br>
7. IoT Server欄でLoRaWANを選択する<br>
  ※Debuggerのオプションをチェックすると、上部メニューの「Status」＞「System Log」に詳細なログが記録されるので、開発中はチェックがおススメ <br>
8. [オープンウェーブHPのダウンロード](http://www.openwave.co.jp/download/)から「LG01-JP/OLG01-JP用LoRaWAN受信プログラム（single_pkt_fwd_v002.hex）」をダウンロード<br>
9. 上部メニューの「Sensor」＞「Flash MCU」に移動<br>
10. ファイルを選択で先ほどのものを選び、「Flash Image」をクリック<br>
11. 上部メニューの「Sensor」＞「LoRa/LoRaWAN」を開き、以下を入力

**LoRaWAN Server Settings**

|設定項目|設定値|
|---|---|
|Server Address |router.as2.thethings.network |
|Server Port |1700 |
|Gateway ID |The Things Networkに登録したゲートウェイID |
|Mail Address |The Things Networkアカウントのメールアドレス |
|Latitude |The Things Networkに登録したゲートウェイの緯度 |
|Longtitude |The Things Networkに登録したゲートウェイの経度 |

**Radio Settings**

|設定項目|設定値|
|---|---|
|TX Frequency |923400000 |
|RX Frequency |923200000 |
|Encryption Key |設定なし |
|Spreading Factor |SF7 |
|Coding Rate |4/5 |
|Signal Bandwidth |125kHz |
|Preamble Length |8 |

</div></details>

### エンドノードの準備

<details><summary>詳細を表示</summary><div>
  
※ここでは温度湿度センサDHT11を使用した例を示す <br>
1. パソコンとLoRa Mini Dev-JPをUSBケーブルで接続<br>
2. Arduino IDEを起動し、「ツール」＞「ボード」で「Arduino/Genuino Uno」を選択 <br>
3. 「ツール」＞「シリアルポート」でUSBのポートを選択 <br>
4. 「スケッチ」＞「ライブラリをインクルード」＞「ライブラリを管理」をクリック <br>
5. ライブラリマネージャが起動するので、以下の3つを検索し、インストールする <br>
  * DHT sersor library<br>
  * Adafruit Unified Sensor<br>
  * CayenneLPP <br>
6. [オープンウェーブHPのダウンロード](http://www.openwave.co.jp/download/)から「LoRaWAN in C library(AS923対応版)」をダウンロード <br>
7. Arduino IDEの「スケッチ」＞「ライブラリをインクルード」＞「ZIP形式のライブラリをインクルード」をクリック、先ほどのZIPファイルを選択するとインストールされる <br>
8. LoRa Mini Dev-JPとDHT11をジャンパ線でつなぐ（データ線はA0につないでおく<span style="color : red">DHT11はデジタルピンでは？</span>） <br>
9. [オープンウェーブHPのダウンロード](http://www.openwave.co.jp/download/)から「温度湿度センサThe Things Networkサンプル（ABP版or OTAA版）」をダウンロード <br>
10. デバイス登録時のサンプルコードをもとに、ダウンロードしたプログラムの以下を修正 <br>
  * ABP：DEVADDRとNWKSKEYとAPPSKEY<br>
  * OTAA：DEVEUIとAPPEUIとAPPKEY <br>
11. Arduino IDEでプログラムを書き込む <br>
12. The Things Networkの「CONSOLE」＞「アプリケーション一覧」でアプリケーションを選択し、「データ」をクリックすると、受信が確認できる！ <br>
13. 今はデータが16進なので、フォーマットするために「Payload Formats」をクリック <br>
14. 真ん中あたりのコーディングスペースに以下のコードを入力すれば、データの表示が変わる <br>
```
function Decoder(bytes, port){
  var humidity=bytes[0];
  var temperature=bytes[1];
  return{
    humidity:humidity
    temperature:temperature
  }
}
```

</div></details>

### アプリケーションとの連携

<details><summary>詳細を表示</summary><div>

#### Cayenneによるデータの可視化（ダッシュボード）
1. [https://mydevices.com](https://mydevices.com )にアクセスし、右上のLog inをクリック<br>
2. Cayenneを選択し、Log in画面の下にあるRegisterをクリックし、新規アカウントを作成<br>
3. 「LoRa」を選択<br>
4. 左のリストから「The Things Network」を選び、右のリストから「Cayenne LPP」を選択する<br>
5. 以下の項目を設定すると、ダッシュボードが開く<br>

|設定項目|設定内容|
|---|---|
|Name|表示される際の名前|
|DevEUI|The Things Networkで取得したDevEUI|
|Activation Mode|Already registered|
|Location|This device doesn't move|

6. ブラウザのURLの最後の「/」以降がプロセスのIDなのでメモしておく<br>
7. The Things Networkの「CONSOLE」＞「アプリケーション一覧」から連携する<br>
  * アプリケーションを選び、「Payload Formats」をクリック<br>
  * 一番上の欄で「Cayenne LPP」を選び、Save<br>
  * 「インテグレーション」＞「インテグレーションの追加」＞「MyDevices」<br>
  * プロセスIDに先ほどメモしたものを、AccesKeyのDefault Keyに「devices」「messages」<br>
  * 「インテグレーションの追加」をクリック<br>
8. [オープンウェーブHPのダウンロード](http://www.openwave.co.jp/download/)から「温度湿度センサThe Things Network-Cayenneサンプル（ABP版or OTAA版）」をダウンロード<br>
9. デバイス登録時のサンプルコードをもとに、ダウンロードしたプログラムの以下の部分を修正<br>
  ABP：DEVADDRとNWKSKEYとAPPSKEY<br>
  OTAA：DEVEUIとAPPEUIとAPPKEY<br>
10. Arduino IDEでプログラムを書き込む<br>
11. CayenneのWeb画面を開くと、データが表示される<br>
  ※AndroidのCayenneアプリでも見れる

#### DataStorageによるデータのストレージ
1. The Things Networkで連携するアプリケーションを選択し、インテグレーションの追加<br>
2. 「DataStorage」を選択＞「インテグレーションの追加」をクリック（特に設定なし）<br>
3. 概要が表示されるので、「プラットフォームに移動」をクリック<br>
4. 表示されたページ右上の「Authorize」をクリック<br>
5. アクセスキーはThe Things Networkでアプリケーションを選択してdefault keyを使う<br>
6. 「Authorize」をクリックして認証完了<br>
7. 「GET」の「Try it out!」をクリックすると、「REST」のレスポンス内容が表示される<br>

<!-- 
#### IFTTT MakerによるWebサービス連携
※今回はTwitter連携を例にする
1. [https://ifttt.com](https://ifttt.com )にアクセスし、アカウント作成
2. The Things Networkの「インテグレーションの追加」で「IFTTT Maker」を選択
3. 設定画面が開くのでそのまま開いておく
4. IFTTTのHPの下部にある「IFTTT Platform」をクリック
・「Get started」＞「Become a Maker」＞「Start building」で「Customize your profile」
・別タブで連携するTwitterアカウントにログインしてから、「Twitter」を選択
・「連携アプリを認証」すると、「Applets」の画面が開く
・左のメニューから「Private」を選択、右上の「New Applet」をクリック
・各項目を入力して「Save」
-->
</div></details>

<a id="anchor5"></a>
## プログラム

### アクティベーション方式について

<a id="anchor6"></a>
## LoRa Miniの使い方

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
