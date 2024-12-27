# 大作业
蒋徐祺 2024计算概论A大作业

## 整体定位

### 软件定位
一个前后端分离的，自由的，轻量化，可拓展，可部署在一般嵌入式设备的五子棋应用

### 开发定位：
试一试Linux，试一试各种开发工具，试一试搭个服务器，试一试训练神经网络，GUI之类的可能有点丑哇👉👈

## 环境配置
- 总体上，开发环境使用了Linux，前端和判断规则的后端由C++编写，AI由python编写，利用pytorch框架实现神经网络。

- 在前端框架的选择上，以轻量化为导向，主流的Qt体积太大了，于是选择了一个更加轻量级的框架fltk，更加适合嵌入式设备

- 前后端通过http request交互，C++的部分使用了httplib库，python的部分使用了flask。为兼容botzone，http中数据以json格式传输，解析json使用了jsoncpp库

- 音频处理使用了SDL2

- 训练神经网络使用了pytorch，训练代码参考了基于github上利用AlphaZero的[开源代码](https://github.com/junxiaosong/AlphaZero_Gomoku.git)

- 具体的依赖关系可以在Makefile和requirement.txt中看到

## 开发特色

- 学习使用了git进行版本控制，实现了版本管理，实现了AI和GUI的平行开发，并有效规范了进度
- 学习并编写了简单的Makefile，方便地处理了多个文件的依赖，同时PHONY也能方便地运行程序
- 学习了Markdown，编写了简单的README文件
- 学习并使用了python虚拟环境对框架进行部署
- 中途使用过云服务器对服务器进行部署，学了简单的ssh（但是没钱了所以最终部署在localhost QwQ）
- 看了Google关于alphazero的论文，初步了解了强化学习的一点东西
- 考虑到数据兼容性，大部分数据用json传输和存储
- 项目最终开源到了GitHub，方便后续与他人进行协作

## 功能介绍

- 人人对战与人机对战，可选先手
- GUI界面，多页面应用
- 悔棋功能
- 存储功能，与加载存档功能
- 可以挑选bgm并调节音量

## 进一步开发

### 功能上

- 可以通过神经网络的训练程度分设不同的AI能力梯度，使AI对战功能可以选择难度
- 在服务器端，可以利用socket处理并行的请求，可以满足多个棋局同时在线（**启动！**）
- 可以加入注册功能或利用微信的api进行身份认证，利用cookie实现对应的个人内容存储

### 优化上

- 可以加入更加详尽的注释
- AI太蠢了，会面向botzone再写个基于规则或者αβ剪枝搜索的算法
- 数据存储可以使用数据库部署在云端，会更加高效安全

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.