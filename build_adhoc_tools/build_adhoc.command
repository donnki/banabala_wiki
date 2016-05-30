#!/bin/sh
shellpath=`dirname $0`
cd $shellpath
APP_NAME=HerosLG\ iOS
APP_PATH="/Volumes/MacintoshHD/DerivedData/HerosLG-bljaawcsazbrjwcxdibaenihgegr/Build/Products/Debug-iphoneos/"
SIGNATURE="iPhone Distribution: Huiwen Wang"
DATE_STR=`date +%Y-%m-%d`
DEST_DIR="../../../../../HerosLG_TestApp/2.ios/HerosLG iOS $DATE_STR"
PROVISION_FILE="HerosLG_lolbranch_adhoc.mobileprovision"

echo "创建临时目录."
mkdir Payload
echo "正在将app文件拷贝至临时目录..."
cp -r "$APP_PATH/$APP_NAME.app" Payload/
rm -r "Payload/$APP_NAME.app/_CodeSignature/"

echo "使用Provision文件路径：$PROVISION_FILE"
cp "$PROVISION_FILE" "Payload/$APP_NAME.app/embedded.mobileprovision"

echo "正在使用证书进行签名：$SIGNATURE"
codesign -f -s "$SIGNATURE" --entitlements entitlements.plist "Payload/$APP_NAME.app"

echo "正在打包ipa文件：$APP_NAME.ipa"
zip -qr "$APP_NAME.ipa" Payload/

echo "将生成的ipa文件移动至指定路径"
if [[ ! -d "$DEST_DIR" ]]; then
	mkdir "$DEST_DIR"
fi
mv "$APP_NAME.ipa" "$DEST_DIR"

echo "删除临时目录"
rm -rf Payload

cd "$DEST_DIR"
echo "打包完成，已在以下路径生成包文件：\n`pwd`/$APP_NAME.ipa"
