使用说明

本目录所包含源文件是盛大网络哼唱检索系统的源文件，以及示例文件（STester.cpp）。哼唱检索源文件的说明如下：

    SDFuzzySearch.h 为盛大网络模糊检索引擎的头文件，提供模糊检索引擎接口的调用方法。
    
    SDSP.h 和 SDSP.cpp为盛大网络哼唱检索系统的数字信号处理函数库，供提取哼唱旋律特征（SMelody.cpp）使用
    
    SMelody.h 和 SMelody.cpp为盛大网络哼唱检索系统的旋律特征提取函数库，目的是从输入哼唱语音中提取旋律特征
    
    SModel.h 和 SModel.cpp为盛大网络哼唱检索系统的旋律库操作函数库，目的是从旋律库模型文件中读取旋律数据
    
    STypes.h 为盛大网络哼唱检索系统的常量以及结构体定义文件
    
    SUtil.h 和 SUtil.cpp 为盛大网络哼唱检索系统的常用函数库

示例文件（STester.cpp）的目的是为了介绍哼唱检索系统各接口的调用方法。本目录还附带了一个示例工程（Visual C++ 6）用于编译这些源代码以生成控制台测试程序。当编译生成控制台程序之后，用户通过命令行调用来进行哼唱检索。假如工具的名字叫做SDHumming，那么运行控制台测试程序的命令行是这样的：

    命令行> SDHumming.exe QBHModel.dat QBHModel.info test.wav result.txt

其中，QBHModel.dat和QBHModel.info是旋律库生成工具生成的模型文件。test.wav是输入的测试语音文件，该文件采用MicroSoft Wav格式，采样率为8kHz，采样精度为16bit。result.txt为识别结果输出文件，当程序运行成功之后，将会在该文件中输出识别结果。


版权声明

本目录所包含内容遵守盛大网络哼唱检索开源软件的版权声明。
