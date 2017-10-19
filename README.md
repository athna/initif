# initif

## Windows版

#### 必要なもの
- windows_initif.ps1
- windows_initif_set.ps1
- windows_initif_input.ps1
- log.txt
- initif.conf
- initif_shape.exe

#### セットアップ方法
1. 上記のファイルを「C:\CyberRange\initif\」内に配置する
2. [Windowsキー]+Rを押して、ファイル名を指定して実行を立ち上げる
3. 「gpedit.msc」と入力してグループポリシーエディタを立ち上げる
4. [コンピュータの構成]→[Windowsの設定]→[スタートアップをダブルクリック]
5. 「powershellのタグ」を選択する
6. 「C:\CyberRange\initif\windows_initif.ps1」を登録する

なお、事前に実行ポリシーを変更しておく必要があります。


## Ubuntu版
現在作成中