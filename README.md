#### Cydia Hook使用
1. Root手机安装[基础框架应用](http://www.cydiasubstrate.com/)
2. 下载[Cydia SDK](http://www.cydiasubstrate.com/id/73e45fe5-4525-4de7-ac14-6016652cc1b8/)并集成到工程中
3. 编写hook代码，ndk部分需要单独编译，将生成的so文件打包并安装
4. 安装时Cydia插件提示软重启，重启后可以查看log信息

编译命令：
> ndk-build  APP_ABI="armeabi"

> ndk-build  APP_ABI=“x86”

在使用Cydia Hook时需要知道so中不同平台中对应的函数名称，可以通过如下`nm`命令获取：

`nm`命令位置：`/usr/local/Cellar/android-ndk/android-ndk-r10e/toolchains/arm-linux-androideabi-4.9/prebuilt/darwin-x86_64/arm-linux-androideabi/bin`

`nm`命令使用：
```
NetEasedeiMac-9:bin netease$ ./nm -a ~/Desktop/libdvm.so | grep dvmUseJNIBridge
0005bd30 T _Z15dvmUseJNIBridgeP6MethodPv
```

可以使用`objdump`查找so中方法：
```
NetEasedeiMac-9:bin netease$ ./objdump -TC ~/Desktop/libdvm.so |grep dvmUseJNIBridge
0005bd30 g    DF .text	000000d7 dvmUseJNIBridge(Method*, void*)
```
对应的`nm`方法：
```
NetEasedeiMac-9:bin netease$ ./nm -gC ~/Desktop/libdvm.so | grep dvmUseJNIBridge
0005bd30 T dvmUseJNIBridge(Method*, void*)
```
添加hook函数参考 [参考1](http://www.csdn.net/article/2015-08-07/2825405) [参考2](http://blog.csdn.net/qq_18870023/article/details/52247483) [参考3](http://www.cnblogs.com/goodhacker/p/4912904.html)
