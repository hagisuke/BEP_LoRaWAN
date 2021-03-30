# BEP_LoRaWAN　実証実験報告マニュアル

## 目次
1. [概要](#anchor1)<br>
2. [LoRaWAN](#anchor2)<br>
3. [使用したLoRaゲートウェイ、デバイス](#anchor3)<br>
4. [The Things NetworkとLoRaWAN](#anchor4)<br>
5. [プログラム](#anchor5)<br>
6. [LoRa Mini Dev-JPの使い方](#anchor6)<br>
7. [LoRa Miniの使い方](#anchor7)<br>
8. [放流結果](#anchor8)<br>
9. [今後の展開](#anchor9)<br>
10. [参考文献](#anchor10)<br>

<a id="anchor1"></a>
## 概要
2020年9月から2021年にかけて、e-kagakuアカデミーは[琵琶湖水環境プロジェクト「BEP Environment Project 2020」](https://global-science.or.jp/biwako-environment-project/)（以下BEP2020）を行った。本プロジェクトは小学生から高校生にわたる参加者が一人一つ琵琶湖の水環境などを測定する観測機を製作し、琵琶湖に放流してデータを集め、MATLABなどの分析ツールをもちいて分析するというものである。2020年から2021年にかけてのコロナウイルスの流行を受けて、月2回ほどオンラインでの作業日を設けつつ、各自が自宅で観測機の製作を進める形となった。 <br>
[紹介動画](https://youtu.be/qNQzHGHJ1aQ)

プロジェクトの参加者が製作を進めるのと同時並行で、当会のサポーター4名でサポーター機の製作を行った。プロジェクト参加者の観測機はArduinoにセンサーを繋げ、SDカードにセンサー値を記録するというものだが、サポーター機ではLoRaWANを用いてリアルタイムで観測したデータを送受信できるデバイスを作成した。本稿ではLoRaWANに焦点を当てて、作成したデバイスについてまとめる。

**なお、取得データの所有権および当リポジトリの著作権はe-kagakuにあるため、データ・文章・画像等の内容の無断転載及び複製等の行為はご遠慮ください。（連絡先：science.labo008@gmail.com）**

<a id="anchor2"></a>
## LoRaWAN
### LoRaWANとは
LoRaWANはLPWA（Low Power Wide Area、省電力長距離通信）の一種であり、Semtech社のLoRa変調という技術に基づいた通信方式である。WiFiなどの2.4 GHz帯や5.0 GHz帯とは異なり、主に920 MHz帯を使っており、振動数が小さいことからわかる通り、通信速度は遅いがWiFiよりも回折しやすく、長距離通信が可能である。通信距離としては、建物が林立していなければ1 km程度は安定して通信可能であり、条件が良ければ数十キロメートル離れていても通信可能である。

### なぜLoRaWANか？
LoRaWANを使うメリットとしては以下の4つがある。
* 通信距離が長い
* 消費電力が小さい
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
* アンテナ: RF ANT 920MHZ/1.8GHZ WHIP RA【ANT-GHEL2R-SMA】 &yen; 756<br>
  [https://www.marutsu.co.jp/pc/i/28031075/](https://www.marutsu.co.jp/pc/i/28031075/)
* アンテナ外付けケーブル: SMA-IPEX/UFL ケーブル 8cm SMAメス⇔U.FL/IPEXメス 内蔵アンテナ外付ケーブル &yen; 799<br>
  [https://www.amazon.co.jp/dp/B07P9PBX5M](https://www.amazon.co.jp/dp/B07P9PBX5M)
 
 
LoraWANゲートウェイLPS8-JPの日本語版マニュアルは以下のURLから参照できる。<br>
[https://www.slideshare.net/copstalk/8lorawan-lps8jp-v120jp](https://www.slideshare.net/copstalk/8lorawan-lps8jp-v120jp)

<img src="/image/LPS8-JP.jpg" width="400">

<a id="anchor11"></a>
### LoRa Mini Dev-JPとLoRa Miniの違い
LoRa MiniはArduinoにも搭載されているAVR社のマイコン、ATmega328PとSX1276/78というLoRaWAN送受信用のモジュールを組み合わせたモジュールである。ATmega328Pには予めArduino UNO用のブートローダーが入っており、Arduino UNOとほぼ同じようにArduino IDEで開発できる。ただ、写真の通りピンやピンソケットがついていないため、プログラムのダウンロードやセンサーとの接続にははんだ付け作業が必要である。<br>
一方でLoRa Miniを開発ボード化したものがLoRa Mini Dev-JPであり、USBによるプログラムのダウンロードやジャンパーワイヤーを使ったセンサーの動作確認などができるようになっている。今回のサポーター機の作成では、まずLoRa Mini Dev-JPが使えるか確認してから、LoRa Miniを使用してみた。

LoRa Mini
<img src="/image/LoRa_Mini.PNG" width="400">

LoRa Mini Dev-JP
<img src="/image/LoRa_Mini_Dev-JP.PNG" width="400">

<a id="anchor4"></a>
## The Things NetworkとLoRaWAN
### The Things Network 概要
[The Things Network（TTN）](https://www.thethingsnetwork.org/country/japan/)はLoRaWANのサービスを提供している情報共有プラットホームサービスである。LoRaWANシステムモデルの図の、ネットワークサーバーやユーザーアプリケーションを提供していると考えればよい。<br>
今回のサポーター機の開発にあたり、このThe Things Networkを利用した。以下に登録手順や使い方を述べる。<br>
[LPS8-JPのマニュアル](https://www.slideshare.net/copstalk/8lorawan-lps8jp-v120jp)も参考になる。

### The Things Networkの準備

<details><summary>詳細を表示</summary><div>

#### アカウント作成
1．[https://www.thethingsnetwork.org/country/japan/](https://www.thethingsnetwork.org/country/japan/)にアクセス<br>
2．「Sign Up」からユーザー名とメールアドレス、パスワードを入れて「Create account」 <br>
3．認証メールが来るので、「Activate Account」をクリック、ログインできる <br>

#### コンソール設定
1．「CONSOLE」をクリックし、右上のユーザー名をクリックして「Settings」＞「Language」から言語を日本語にして「Save」で日本語化！<br> 
2．同じく「設定」から「ハンドラー」＞「ttn-handler-asia-se」を選択し「保存」をしておく<br>

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
|情報 |ブランドにDragino、モデルにLPS8-JP、アンテナにN/Aと入れる |
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
|一般 |「記述」にデバイスの名前を付ける、他にもいろいろ設定可能 <br>※アクティベーション方式はOTAA、ABPどちらか選択（プログラムがABP版とOTAA版で異なるので、どちらを選択したか覚えておく。詳細は「プログラム」で後述） |
|場所|地図上でのデバイスの位置に加え、高度もメートル単位で設定可能 |<br>

LoRaWanの通信を研究開発を行っている際は「設定」から「フレームカウンターチェック」を無効にすることをお勧めする。<br>有効のままだと、一回エンドノードの電源を切る度にデバイス登録を再度し直さなければならず、かなり面倒である。<br>また、設定で「ABP」と「OTAA」を切り替えられる。

</div></details>

### ゲートウェイの準備
<details><summary>詳細を表示</summary><div>

Wi-FiでLPS8-JPに接続する方法<br>
LPS8-JPの電源を入れた際、PCにWi-Fiアクセスポイント「dragino-xxxxxx(xxxはゲートウェイによって異なる)」が表示されるので、パスワード「dargino+daragino」を入力して接続する。<br>
「dragino-xxxxxx」が表示されない場合は、トグルボタン（側面にある小さい穴）を何度も押してみる。<br>

※WiFiかLANでパソコンと通信できることを前提とする <br>
1. LPS8-JPのIPアドレス（デフォルトは10.130.1.1）をブラウザに入力し、設定画面へ<br>
2. Username（デフォルトは「root」）とPassword（デフォルトは「dragino」）を入力し、Login<br>
  ※Login後、Passwordは変更すべき（上部メニューの「System」＞「Administration」から変更できる ）<br>
3. 上部メニューの「Internet」＞「Internet Access」をクリックし、ネット接続設定をする<br>
4. TeraTermなどのSSHクライアントでLPS8-JPに接続、「ping [www.openwave.co.jp](www.openwave.co.jp) 」などと打ち込み通信できることを確認する（FQDN指定でpingを実行している）<br>
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

<details><summary>詳細を表示</summary><div>

LoRaWANではデバイスとゲートウェイ間で通信を行うが、単に通信するだけでは暗号化がされていないので、盗聴されてしまう。これは、個人情報と密接にかかわっているIoTのデータを送受信するうえで重大な問題である。<br>
LoRaWANでは暗号化について規定があり、デバイス・ゲートウェイ間の暗号化通信の開始時にアクティベーションが必要となる。このアクティベーションの方式に2種類あり、ABP(Activation by Personalization)とOTAA(Over-The-Air Activation)である。ABPとOTAAの違いについては、以下で概説する。

#### ABP(Activation by Personalization)方式
暗号化には、セッションキーとデバイスアドレスが必要になるが、ABPではLoRaWANサーバーで予め発行したアプリケーションセッションキー（AppSKey）、ネットワークセッションキー（NwkSKey）デバイスアドレス（DevAddr）を、プログラムを通じてデバイスに書き込んでおく。NwkSKeyがデバイスとゲートウェイ間で通信する際のデバイスの照合に用いられ、こちらは公開される。一方でAppSKeyは秘密鍵であり、ペイロードの暗号化と復号に用いられる。概念図は以下のようになる。

<img src="/image/activation/ABP_Activation.png" width="400">

出典: [https://www.rs-online.com/designspark/what-is-the-activation-method-on-lorawan-abp-and-otaa-jp](https://www.rs-online.com/designspark/what-is-the-activation-method-on-lorawan-abp-and-otaa-jp)

#### OTAA(Over-The-Air Activation)方式
OTAAでは、デバイスごとにことなるセッションキーやデバイスアドレスを取得してデバイスに書き込んでおく必要がなく、プログラム開始時にサーバーとの暗号化通信に必要な情報をやりとりする。具体的には、以下の順序に従って行われる。<br>
まず、デバイスが参加要求をLoRaWANサーバーに送信する。参加要求では、アプリケーションEUI、デバイスEUI、アプリケーションキーなどが送信される。これをもとにサーバー側がデバイスアドレスや各種セッションキーを含む要求承認を行う。これをもとに、暗号化通信が開始される。

<img src="/image/activation/OTAA_Activation.png" width="400">

出典: [ https://www.rs-online.com/designspark/what-is-the-activation-method-on-lorawan-abp-and-otaa-jp ](https://www.rs-online.com/designspark/what-is-the-activation-method-on-lorawan-abp-and-otaa-jp )

TTNでのアクティベーション方式や暗号化の詳細については、[https://www.thethingsnetwork.org/docs/lorawan/security.html](https://www.thethingsnetwork.org/docs/lorawan/security.html) を参照。

今回のサポーター機ではABP方式を用いた。

</div></details>

### プログラムの概略

#### 使用したライブラリ
* SPI library（標準ライブラリ）
* SoftwareSerial library（標準ライブラリ）
* [Arduino-LMIC library](https://github.com/matthijskooijman/arduino-lmic)
* [CayenneLPP library](https://github.com/ElectronicCats/CayenneLPP)
* [TinyGPSPlus library](https://github.com/mikalhart/TinyGPSPlus)
  
なお、[librariesフォルダー](/libraries)に今回使ったプログラムに必要なライブラリーを全て載せた。

#### プログラムの説明

<a id="anchor6"></a>
## LoRa Mini Dev-JPの使い方
LoRa Mini Dev-JPはLoRaモジュール用の開発ボードであり、Arduino UNOと互換性があるため、Arduino IDEでプログラミングができる。USBシリアル変換モジュールがついているので、プログラムのダウンロード方法もArduino UNOと同様である。（IDEのボードの設定も"Arduino Uno"のままで良い）また、アンテナも予めついている。

Arduino UNOとの相違点はピンの対応のみである。以下にデータシートに記載されているピンの対応図を載せる。

<image src="/image/LoRaMini/LoRaMini_pin.PNG" width="400">
  
出典: [https://docs.rs-online.com/7023/A700000006486751.pdf](https://docs.rs-online.com/7023/A700000006486751.pdf)

<a id="anchor7"></a>
## LoRa Miniの使い方
[LoRa Mini Dev-JPとLoRa Miniの違い](#anchor11)で述べたように、LoRa Miniは開発用ボードではないので、USBシリアル変換モジュールや電源ポートなどもついていない。今回はLoRa通信を行えるArduino UNO用のシールドを作成するという方針でLoRa Miniを使用することにした。<br>
LoRa MiniはAVR社のマイコン、ATmega328PとSX1276/78というLoRaWAN送受信用のモジュールを組み合わせたモジュールを組み合わせたモジュールであり、それ自身がArduino UNOと等価なマイコンを積んでいるため、Arduino UNOの上にLoRa Miniをシールドとして取り付ける方法は冗長である。本来はSX1276/78のみをシールド化できれば最善だが、市販でSX1276/78が単体で販売されていなかったので、本方法を採用した。

回路図の設計を行ううえで、Dragino社のGithubに掲載されているLoRa Miniの回路図などを参考にした。<br>
[https://github.com/dragino/Lora/tree/master/LoRa%20mini/v1.3](https://github.com/dragino/Lora/tree/master/LoRa%20mini/v1.3)

### ATmega328Pについて

<details><summary>詳細を表示</summary><div>

ATmega328PはAVR社のマイコンであり、Arduinoにも搭載されている。<br>
同じ電子部品でもパッケージの種類が複数あり、例えばArduino UNOではDIP-28やTQFP-32などのパッケージのATmega328Pを用いている。

<image src="/image/LoRaMini/ArduinoUNO_DIP.png" width="400">

DIP-28パッケージのATmega328Pを搭載したArduino UNO

<image src="/image/LoRaMini/ArduinoUNO_TQFP.png" width="400">

TQFP-32パッケージのATmega328Pを搭載したArduino UNO

画像の赤枠で囲まれた部分がATmega328Pである。ここで、DIP-28パッケージのものはATmega328Pの取り外しが可能であるが、TQFP-32パッケージのものは取り外せない。Arduino UNOにLoRa Miniシールドを載せるとき、LoRa Mini内部のATmega328Pを使わなければならないため、Arduino UNOのATmega328Pを取り外さなくてはいけない。よって、**DIP-28パッケージのATmega328Pを搭載したArduino UNO**を用いる必要がある。

ATmega328Pにプログラムをダウンロードするには、あらかじめブートローダーが入っている必要がある。ブートローダにも何種類かあるが、LoRa MiniのATmega328PにはあらかじめArduino UNOと同じブートローダーが入れられている。またArduino UNOとして使うにはクロック周波数は16 MHzでなければならない。ATmega328Pの内部には8 MHzの水晶振動子しかないが、LoRa Miniの回路には16 MHzの水晶振動子が組み込まれているので、こちらも気にする必要はない。

参考までに、LoRa Mini内のATmega328PのパッケージはTQFP-32である。TQFP-32パッケージの場合のピンの配置は以下の図のようになっている。LoRa Mini内部の回路図を見る際に参考になるかもしれない。

<img src="/image/LoRaMini/ATmega328P_TQFP_pinout.jpg" width="400">

</div></details>

### 回路図

<details><summary>詳細を表示</summary><div>

まず、ユニバーサル基板にArduino UNO用のピンソケットをはんだ付けした。その後LoRa Miniに[ピンヘッダー](https://akizukidenshi.com/catalog/g/gC-00167/)をはんだ付けして、下の回路図のようにユニバーサル基板とはんだ付けをした。

<img src="/image/LoRaMini/LoRaMini_circuit.png" width="400">

図の配線の色分けは、重なり部分を見やすくするための便宜上のもので、特に意味はない。<br>
また、実際の配線はユニバーサル基板の裏側で行っている。

ピンの対応は下の表のとおりである。
|LoRa Mini|Arduino UNO|
|---|---|
|Reset|Reset|
|RXI|D0|
|TXO|D1|
|LORA_NSS|D10|
|D3|D3|
|D4|D4|
|GND|GND|
|D5|D5|
|3.3 V|3.3 V|
|MISO|D12|
|MOSI|D11|
|SCK|D13|
|GND|GND|
|LORA_DIO3|-|
|LORA_DIO4|-|
|A7|-|
|A6|-|
|A5|A5|
|A4|A4|
|A3|A3|
|A2|A2|
|A1|A1|
|A0|A0|
|AREF|AREF|

また今回はGPSデータを取得するため、シールドにGPSセンサー用の回路を追加した。

<image src="/image/LoRaMini/LoRaMini_GPScircuit.png" width="400">

</div></details>

### アンテナ
LoRa Miniはアンテナが別売りなので、以下の物品を購入した。
* アンテナ: RF ANT 920MHZ/1.8GHZ WHIP RA【ANT-GHEL2R-SMA】<br>
  [https://www.marutsu.co.jp/pc/i/28031075/](https://www.marutsu.co.jp/pc/i/28031075/)
* アンテナ外付けケーブル: SMA-IPEX/UFL ケーブル 8cm SMAメス⇔U.FL/IPEXメス 内蔵アンテナ外付ケーブル<br>
  [https://www.amazon.co.jp/dp/B07P9PBX5M](https://www.amazon.co.jp/dp/B07P9PBX5M)
  
### プログラムのダウンロード
1. Arduino UNOのATmega328Pを取り外す
2. Arduino UNOの上にシールドをかぶせ、Arduino UNOとPCをUSBで接続する。
3. Arduino IDEの「ツール」＞「書き込み装置」から、"Arduino as ISP"を選択
4. Arduino IDEの「ツール」＞「ボード」から、"Arduino UNO"を選択
5. プログラムの書き込みを行う

<a id="anchor8"></a>

## 放流結果（放流日 2021年1月16日（土））
なぜかはんだ付けして作成した送信機は受信できなかったが、既存品の送信機は受信に成功した。使ったセンサーはGPSのみである。<br> <br>
取得したデータ：[取得データ.xlsx](/取得データ.xlsx) <br>
当日使ったプログラム：[GPS送信プログラム.ino](/GPS送信プログラム.ino)

### ブイ放流前写真
はんだ付けして作成した送信機 <br>
<image src="/image/Analysis/fig_3.jpg" width="400">
<image src="/image/Analysis/fig_4.jpg" width="400"> <br><br>
既存品を用いた送信機 <br>
<image src="/image/Analysis/fig_5.jpg" width="400">
<image src="/image/Analysis/fig_6.jpg" width="400">

### 放流・回収の様子
滋賀県大津市和邇川河口 <br>
<image src="/image/Analysis/fig_7.png" height="200">
<image src="/image/Analysis/fig_8.png" height="200"> <br>

[Facebook投稿](https://www.facebook.com/186352151439515/posts/4887357574672259/) <br>
LIVE配信のアーカイブ
* [準備](https://youtu.be/dgwvzyrcYEE)
* [開始～グループ2](https://youtu.be/ArVwdoS0HKU)
* [グループ3の報告～グループ5](https://youtu.be/Upn2pfBHjsM)
* [グループ6の報告～追加放流準備](https://youtu.be/XsILmHTfFZw)

### ゲートウェイ設置点
<image src="/image/Analysis/fig_2.jpg" width="400">

### 受信成功点
<image src="/image/Analysis/fig_1.png" width="400">

### 受信成功点アニメーション

<image src="/image/Analysis/受信成功点アニメーション.gif" width="400">


### Altitude（高度）
和邇川河口の高度は約80mであった

### RSSI（電波強度）
RSSIが大きいほど電波強度が高いことを示す。どこも-70前後であった。

### SNR（電波品質）
SNRが大きいほど電波品質が高いことを示す。<br> SNR = RSSI – ノイズ強度 <br> Wi-Fiの場合、+30以上でないと通信が悪いと言われているので、LoRaWanは電波品質が悪くてもデータを送れるといえる。どこも+10前後であった。


<a id="anchor9"></a>
## 今後の展開

### 生徒へLoRaWAN通信を還元するまでのハードル
* LoRaWANの端末がそこそこのお値段する。
* LoRa Mini（基板のみの方）をシールド化するためのはんだ付けは、初心者には困難で時間もかかる。（はんだ付けに不慣れなサポータ―で3時間）また、Arduino UNOがATmega328Pを取り外しできるタイプでなければいけない。よってLoRa Mini Dev-JP（開発用ボード）を使うのかLoRa Miniを使うか再考する必要がある。
* LoRa Miniを用いて作成した送信機がうまく動作しなかった原因の検証
* ゲートウェイをどうするか。<br>
 合宿で使用する分には既製品が1台あれば十分だが、今回のBEPのように各自1台製作するのであればRasberry PiとLoRa通信モジュールで安価に制作できないか検討する。<br>
 [https://www.rs-online.com/designspark/building-a-raspberry-pi-powered-lorawan-gateway-jp](https://www.rs-online.com/designspark/building-a-raspberry-pi-powered-lorawan-gateway-jp)<br>
 LoRa Miniを2台使用して送受信を行う方法もある。<br>
 [https://wiki.dragino.com/index.php?title=LoRa_Mini](https://wiki.dragino.com/index.php?title=LoRa_Mini)
* 生徒向けマニュアルを作成
* 様々な環境での通信距離の検証

### 今後の計画
1. はんだ付けして作成した送信機がうまく動作しなかった原因の検証 (2月6日) 
2. 最長受信距離の検証（2月中）
3. 5月再放流の準備（3月～4月）
4. 再放流（5月）
5. （5月までに余裕があれば）ゲートウェイをRaspberry PiとLoRa通信用のモジュール等を載せたシールドで自作できないか試す
6. （5月までに余裕があれば）Dragino社以外の送信機を用いてデータの送受信ができるか試す
7. 子供向けのテキスト作成（5月以降）


### LoRa通信の応用案
* 農業への応用
* 河川の水位や水質のリアルタイムでの監視
* 老朽化建築物などの点検の自動化
* 水中ロボットへの搭載
* SBPの機体に搭載<br>
などなど



<a id="anchor10"></a>
## 参考文献
1. The Things Network　「LoRaWAN」をみんなでシェアして使う（工学社）<br>
2. The Things Network ホームページ<br>
  [https://www.thethingsnetwork.org/](https://www.thethingsnetwork.org/)<br>
3. Dragino Github Lora<br>
  [https://github.com/dragino/Lora](https://github.com/dragino/Lora)<br>
4. Datasheet_Lora_Mini<br>
  [https://docs.rs-online.com/7023/A700000006486751.pdf](https://docs.rs-online.com/7023/A700000006486751.pdf)<br>
5. myDevices ホームページ<br>
  [https://mydevices.com/](https://mydevices.com/)<br>
6. From Arduino to a Microcontroller on a Breadboard<br>
  [https://www.arduino.cc/en/Tutorial/BuiltInExamples/ArduinoToBreadboard](https://www.arduino.cc/en/Tutorial/BuiltInExamples/ArduinoToBreadboard)<br>
7. 株式会社オープンウェーブ　「ダウンロード」<br>
  [http://www.openwave.co.jp/download/](http://www.openwave.co.jp/download/)<br>
8. DESIGNSPARK 「LoRaWANの2つのアクティベーション方式とは？ABPとOTAAについて解説します」<br>
  [ https://www.rs-online.com/designspark/what-is-the-activation-method-on-lorawan-abp-and-otaa-jp ](https://www.rs-online.com/designspark/what-is-the-activation-method-on-lorawan-abp-and-otaa-jp )<br>
9. SlideSHare 「普及型 8チャンネルLoRaWANゲートウェイ LPS8-JP日本語マニュアル v1.2.0-JP」<br>
  [https://www.slideshare.net/copstalk/8lorawan-lps8jp-v120jp](https://www.slideshare.net/copstalk/8lorawan-lps8jp-v120jp)<br>
10. 「いまさら聞けないLoRaWAN入門」<br>
  [https://monoist.atmarkit.co.jp/mn/articles/1709/19/news004.html](https://monoist.atmarkit.co.jp/mn/articles/1709/19/news004.html)
11. DESIGN SPARK 「Raspberry Piを使ってLoRaWANゲートウェイを製作する」<br>
  [https://www.rs-online.com/designspark/building-a-raspberry-pi-powered-lorawan-gateway-jp](https://www.rs-online.com/designspark/building-a-raspberry-pi-powered-lorawan-gateway-jp)
12. Dragino Wiki 「Lora Mini」<br>
  [https://wiki.dragino.com/index.php?title=LoRa_Mini](https://wiki.dragino.com/index.php?title=LoRa_Mini)


e-kagakuアカデミー©e-kagaku Academy All Rights Reserved.
