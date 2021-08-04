OpenHamony# linkboy-hos

#### 介绍
本项目是对于OpenHamony操作系统的插件扩展, 可以支持linkboy对OpenHarmony进行编程

#### 软件架构
本项目包含: 
1.  OpenHarmony功能的封装 (HAL: GPIO, UART, TIMER, ...)
2.  提供一个精简的虚拟机实现, 可以运行linkoby编程语言的后端字节码序列
3.  包含一些特殊类的驱动程序, 如: 红外解码, 步进电机驱动, 温湿度DS18B20, DHT系列...

#### 使用说明

1.  将vos文件夹放置于 OpenHarmony 源码的 Application\sample\wifi-iot\app 文件夹;
2.  将父文件夹的 BUILD.gn 文件内容改为:

import("//build/lite/config/component/lite_component.gni")
lite_component("app") {
    features = [
        "vos:main"
    ]
}


#### 参与贡献
