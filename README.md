# SmartEnvGuard (智环卫士) 
![STM32 Badge](https://img.shields.io/badge/STM32F103C8T6-Embedded-blue) 
![License](https://img.shields.io/badge/License-MIT-green)

基于STM32F103C8T6的智能环境监控系统，集成温湿度检测、紫外线感应、红外触发与多模式控制功能，支持蓝牙远程交互。

[English Document](README_EN.md) | [项目文档](docs/) | [演示视频](#)

## 目录
- [核心功能](#核心功能)
- [硬件架构](#硬件架构)
- [快速开始](#快速开始)
- [开发指南](#开发指南)
- [通信协议](#通信协议)
- [贡献指南](#贡献指南)
- [许可证](#许可证)

## 核心功能 🚀
1. ​**环境感知**​  
   - DHT11温湿度传感器（±5%精度）
   - SD12紫外线强度检测（0-11级）
   - 红外人体感应（触发响应时间<0.5s）

2. ​**控制模式**​  
   ```c
   MANUAL（手动）: 通过4x4矩阵键盘直接控制设备
   AUTO（自动）  : 温湿度阈值触发控制（默认31℃/61%）
   CYCLE（循环） : 5秒周期性任务调度
   BT（蓝牙）    : 支持APP远程控制
