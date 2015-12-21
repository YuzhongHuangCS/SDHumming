关于盛大语音创新院
领先的互动娱乐媒体企业上海盛大网络发展有限公司（以下简称“盛大网络”）旗下的盛大创新院又添新成员——“盛大语音创新院”于10月12日正式授牌成立。这是盛大创新院成立两年来成功孵化的独立团队，也是继百度和谷歌中国之后，国内又一企业正式进入语音识别领域。

盛大创新院成立两年来，一直致力于从事新产品、新技术的创新工作。而语音合成是公司看好并下决心投入的战略方向之一。其团队成员由来自清华、中科大、 Microsoft、Motorola 、Nuance、HP、讯飞等科研机构和优秀企业的语音方向骨干组成，在语音识别、语音合成和声纹识别完整的语音交互技术领域，积累了丰富的理论研究及产品开发经验。今年8月在全世界语音识别领域最知名、最权威的NIST SRE（说话人识别技术评测）比赛中一举获得5个单项指标评测数据名列第一，综合指标排名第一的好成绩。

盛大语音研究团队在正式挂牌成立之前就开始为盛大集团旗下多家子业务公司提供语音方面的技术支持。此次盛大语音创新院的正式成立将更好地保障盛大创新院在语音领域的投入和领先优势，更加专注于语音技术在互动娱乐中的应用创新，带来语音技术与互动娱乐完美结合的深层用户体验，为各业务子公司带来技术创新与支持。据悉，语音创新院正在研发的神秘项目也将于近期推出。

创新院官方网站：http://in.sdo.com/


盛大语音创新院哼唱检索开源软件

1，概述

哼唱检索是基于旋律内容的音乐检索，指用户通过唱出歌词或者哼出歌曲旋律作为输入检索数据，从旋律数据库中检索相对应音乐的检索方法；相对于传统的基于文本的音乐检索，哼唱检索给用户提供了更便捷友好的查询方式，不仅可以作为基于文本的音乐检索一种有益补充，而且还能带来更佳的用户体验。


2，用途及用法

本软件所包含的代码、动态库以及旋律库构成了一个完整的哼唱检索系统，其中包括：

A，创建旋律库模块。 
   参见src\SDHBuildModel\README.txt。文件列表：
   
   src/
   `-- SDHBuildModel
       |-- README.txt
       |-- SBuildModel.cpp
       |-- SBuildModel.h
       |-- SDHBuildModel.dsp
       |-- SDHBuildModel.dsw
       |-- midifile.c
       `-- midifile.h

B，哼唱特征提取、搜索及测试程序
   参见src\SDHumming\README.txt。文件列表：

   src/
   `-- SDHumming
       |-- README.txt
       |-- SDFuzzySearch.dll
       |-- SDFuzzySearch.h
       |-- SDFuzzySearch.lib
       |-- SDHumming.dsp
       |-- SDHumming.dsw
       |-- SDSP.cpp
       |-- SDSP.h
       |-- SMelody.cpp
       |-- SMelody.h
       |-- SModel.cpp
       |-- SModel.h
       |-- STester.cpp
       |-- STypes.h
       |-- SUtil.cpp
       `-- SUtil.h

C，盛大模糊检索引擎相关动态库
   参见lib\README.txt。文件列表：

   lib/
   |-- README.txt
   |-- win32
   |   |-- SDFuzzySearch.dll
   |   `-- SDFuzzySearch.lib
   `-- win64
       |-- SDFuzzySearch.dll
       `-- SDFuzzySearch.lib
	 
D，旋律库及旋律库描述文件

   文件列表：

   model/
   |-- 100Songs.txt
   |-- QBH.Model
   `-- QBHModel.info

E，盛大哼唱检索演示程序
   参见demo\README.doc

        demo/
        |-- 100Songs.txt
        |-- README.doc
        `-- SDHummingDemoSetup.exe

3，特点

音频信息处理领域的入门学习者提供了完整的学习案例和试验平台；本软件具有良好的扩展性，用户可根据需求制作自己的音乐旋律库；识别准确率高，响应速度快，本软件的前身LWX(LW1, LW3)在2008年的国际哼唱音乐检索评测中获得第一名，详情请参见http://www.music-ir.org/mirex/wiki/2008:Query-by-Singing/Humming_Results


4，应用前景

哼唱检索具有较为广阔的应用前景，包括但不限于：卡拉OK语音点歌系统。用户可以通过哼唱一段旋律的方式轻松点歌，免去当前卡拉OK点歌系统需要多次多层级目录选择的麻烦。移动互联网上的音乐检索。诸如手机之类的移动设备键盘较小，输入文本较为不方便，而通过自然便捷的语音输入方式搜索音乐将会是未来技术发展的趋势。


5，版权声明

本软件所包含内容遵守盛大网络哼唱检索开源软件的版权声明。

