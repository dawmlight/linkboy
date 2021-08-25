OpenHamony# linkboy-hos

#### 介绍
本项目是对于OpenHamony操作系统的插件扩展, 可以支持linkboy对OpenHarmony进行编程

#### 相关资源

linkboy软件下载: www.linkboy.cc

linkboy&OpenHarmony简介: https://bbs.elecfans.com/jishu_2118283_1_1.html

linkboy&OpenHarmony编程（基础篇）教程: https://bbs.elecfans.com/group_1537

linkboy&OpenHarmony相关案例视频: https://space.bilibili.com/547895278

linkboy-SIG代码仓：https://gitee.com/openharmony-sig/linkboy

linkboy SIG 历次会议纪要：https://gitee.com/openharmony-sig/sig-content/tree/master/linkboy/meetings

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

#### 开发路标规划

1. 增加完善OpenHarmony L0 通信类扩展库的图形化封装;
2. 增加对OpenHarmony L1/L2 体系的VOS虚拟机移植适配;
3. 增加OpenHarmony生态合作伙伴的开发板图形化编程支持;

