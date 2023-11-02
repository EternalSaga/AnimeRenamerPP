﻿# 动画番剧重命名器

开发目的是方便你更加简便地对各种你收藏下载的动画进行重命名，以方便jellyfin等影音系统进行刮削。

我感觉大家推荐的各种重命名工具都不太好用，就自己开发了一个

不太会写GUI，很丑，大家将就用，希望踊跃提交合并请求

## 编译方式

强烈建议使用vcpkg进行依赖管理，也请使用cmake进行项目构建

使用第三方库有：

* boost：filesystem，locale，regex
* libfmt
* wxWidgets
* nolhmann json

## 当前进度

开发中，bug很多，但是如果你是开发者，可以尝试使用，最基础功能已经达成

* [X] 对文件夹下的番剧进行重命名(可能会失败)
* [X] 对重命名前后的文件名进行备份
* [X] 从备份文件恢复文件名
* [ ] 完全不防呆
* [ ] 甚至不能连续打开多个文件夹进行重命名
