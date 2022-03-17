#### 前言

> 注意：flv是不支持h.265封装的。目前解封装功能正常，所得到的H.264文件与AAC文件均可正常播放。



### 1、demo使用

```bash
$ make clean && make DEBUG=1
$ 
$ $ ./flv_demux_h264_aac 
Usage: 
   ./flv_demux_h264_aac avfile/test1.flv
   ./flv_demux_h264_aac avfile/test2.flv
```



### 2、参考链接

**【参考文章】**

 - [FLV格式详解\_JT同学的博客-CSDN博客\_flv格式**（推荐！！！）**](https://blog.csdn.net/weixin_42462202/article/details/88661883)

 - [FLV封装格式介绍及解析 - 简书](https://www.jianshu.com/p/419f99be3989)

 - [音视频封装：FLV格式详解和打包H264、AAC方案（上） - 云+社区 - 腾讯云](https://cloud.tencent.com/developer/article/1747043)

 - [音视频封装：FLV格式详解和打包H264、AAC方案（下） - 云+社区 - 腾讯云](https://cloud.tencent.com/developer/article/1746991)

**【参考源码】**

 - [https://github.com/ImSjt/H.264toFLV.git](https://github.com/ImSjt/H.264toFLV.git)

 - [https://github.com/rainfly123/flvmuxer](https://github.com/rainfly123/flvmuxer)

**【工具下载】**

 - SpecialAAAC.exe：[https://sourceforge.net/projects/aacstreamanalysis/](https://sourceforge.net/projects/aacstreamanalysis/)

 - H264BSAnalyzer.exe：[https://github.com/latelee/H264BSAnalyzer/tree/master/release](https://github.com/latelee/H264BSAnalyzer/tree/master/release)

 - FlvParse.exe：[https://github.com/ty6815/AvStackDocs/tree/master/media%20format/flv](https://github.com/ty6815/AvStackDocs/tree/master/media%20format/flv)



### 附录（demo目录架构）

```bash
$ tree
.
├── aac_adts.c
├── aac_adts.h
├── avfile
│   ├── test1.aac
│   ├── test1.flv
│   ├── test1.h264
│   ├── test2.aac
│   ├── test2.flv
│   └── test2.h264
├── docs
│   ├── FLV封装格式介绍及解析 - 简书.mhtml
│   ├── FLV格式详解_JT同学的博客-CSDN博客_flv格式.mhtml
│   ├── 笔记---H.264里的SEI - 简书.mhtml
│   ├── 音视频封装：FLV格式详解和打包H264、AAC方案（上） - 云+社区 - 腾讯云.mhtml
│   └── 音视频封装：FLV格式详解和打包H264、AAC方案（下） - 云+社区 - 腾讯云.mhtml
├── flv.c
├── flv_format.h
├── flv.h
├── h264_nalu.h
├── main.c
├── Makefile
├── README.md
├── reference_code
│   └── Flv_to_h264_AAC_Demux.zip
└── tools
    ├── FlvParse.exe
    ├── H264BSAnalyzer.exe
    └── SpecialAAAC.exe
```

