[TOC]

### 准备libJModule库
libJModule库是JMADF框架中用于模块开发的部分。  
你可以从 [https://github.com/FangCunWuChang/libJModule.git](https://github.com/FangCunWuChang/libJModule.git) 处克隆libJModule库源码或将其以Git子项目的形式添加。  
在使用libJModule库时，你需要注意以下几点：
- 在你的项目中添加 [libJModule许可证](https://github.com/FangCunWuChang/libJModule/blob/develop/LICENSE)。  
- 确保你拥有有效的 [JUCE许可证](https://juce.com/get-juce) 或使用Juce框架的 [开源版本](https://github.com/juce-framework/JUCE) ，需要注意的是，Juce框架的开源版本使用 [GPLv3许可协议](https://www.gnu.org/licenses/gpl-3.0.en.html) ，这意味着你的模块也需要在GPLv3协议下发布。

### 动态编译Juce框架
为了减少模块的体积，你需要对Juce框架进行动态编译并链接至你的模块中。

1. 你需要使用Projucer建立项目并选择所有模块后生成如下的一组文件，并将其中的`.cpp`文件添加至你的Juce动态编译项目中（Mac下添加`.mm`文件）:
```
include_juce_analytics.cpp
include_juce_audio_basics.cpp
include_juce_audio_basics.mm
include_juce_audio_devices.cpp
include_juce_audio_devices.mm
include_juce_audio_formats.cpp
include_juce_audio_formats.mm
include_juce_audio_processors.cpp
include_juce_audio_processors.mm
include_juce_audio_utils.cpp
include_juce_audio_utils.mm
include_juce_box2d.cpp
include_juce_core.cpp
include_juce_core.mm
include_juce_cryptography.cpp
include_juce_cryptography.mm
include_juce_data_structures.cpp
include_juce_data_structures.mm
include_juce_dsp.cpp
include_juce_dsp.mm
include_juce_events.cpp
include_juce_events.mm
include_juce_graphics.cpp
include_juce_graphics.mm
include_juce_gui_basics.cpp
include_juce_gui_basics.mm
include_juce_gui_extra.cpp
include_juce_gui_extra.mm
include_juce_opengl.cpp
include_juce_opengl.mm
include_juce_osc.cpp
include_juce_product_unlocking.cpp
include_juce_product_unlocking.mm
include_juce_video.cpp
include_juce_video.mm
JuceHeader.h
```
**需要特别注意的是，以下的文件不要添加到编译列表中：**
```
include_juce_audio_plugin_client_AAX.cpp
include_juce_audio_plugin_client_AAX.mm
include_juce_audio_plugin_client_AU.r
include_juce_audio_plugin_client_AUv3.mm
include_juce_audio_plugin_client_AU_1.mm
include_juce_audio_plugin_client_AU_2.mm
include_juce_audio_plugin_client_RTAS.r
include_juce_audio_plugin_client_RTAS_1.cpp
include_juce_audio_plugin_client_RTAS_2.cpp
include_juce_audio_plugin_client_RTAS_3.cpp
include_juce_audio_plugin_client_RTAS_4.cpp
include_juce_audio_plugin_client_RTAS_utils.cpp
include_juce_audio_plugin_client_RTAS_utils.mm
include_juce_audio_plugin_client_Standalone.cpp
include_juce_audio_plugin_client_Unity.cpp
include_juce_audio_plugin_client_utils.cpp
include_juce_audio_plugin_client_VST2.cpp
include_juce_audio_plugin_client_VST3.cpp
include_juce_audio_plugin_client_VST_utils.mm
```

2. 在你的Juce动态编译项目中添加如下的预处理器定义：
```
JUCE_DISPLAY_SPLASH_SCREEN=0
JUCE_USE_DARK_SPLASH_SCREEN=1
JUCE_MODULE_AVAILABLE_juce_audio_basics=1
JUCE_MODULE_AVAILABLE_juce_audio_devices=1
JUCE_MODULE_AVAILABLE_juce_audio_formats=1
JUCE_MODULE_AVAILABLE_juce_audio_processors=1
JUCE_MODULE_AVAILABLE_juce_audio_utils=1
JUCE_MODULE_AVAILABLE_juce_core=1
JUCE_MODULE_AVAILABLE_juce_cryptography=1
JUCE_MODULE_AVAILABLE_juce_data_structures=1
JUCE_MODULE_AVAILABLE_juce_events=1
JUCE_MODULE_AVAILABLE_juce_graphics=1
JUCE_MODULE_AVAILABLE_juce_gui_basics=1
JUCE_MODULE_AVAILABLE_juce_gui_extra=1
JUCE_MODULE_AVAILABLE_juce_opengl=1
JUCE_MODULE_AVAILABLE_juce_video=1
JUCE_GLOBAL_MODULE_SETTINGS_INCLUDED=1
JUCE_STANDALONE_APPLICATION=1
JUCE_DLL_BUILD=1
JucePlugin_Build_VST=0
JucePlugin_Build_VST3=0
JucePlugin_Build_AU=0
JucePlugin_Build_AUv3=0
JucePlugin_Build_RTAS=0
JucePlugin_Build_AAX=0
JucePlugin_Build_Standalone=0
JucePlugin_Build_Unity=0
JUCE_DISABLE_DLL_ALLOCATORS=1
JUCE_MODAL_LOOPS_PERMITTED=1
```
**另外，你需要为Debug编译配置添加预处理器定义`_DEBUG`，为Release编译配置添加预处理器定义`NDEBUG`**

3. 将Juce源码文件夹中`modules`目录作为包含文件路径添加到你的Juce动态编译项目中

4. 如果你使用MSVC编译器，则视情况使用`/MD`与`/MDd`编译选项之一，同时使用`/bigobj` `/Gd`编译选项与如下链接选项：`/DYNAMICBASE` `/LARGEADDRESSAWARE` `/SUBSYSTEM:WINDOWS`；如果你使用GCC或Clang编译器，则使用`-fPIC`编译选项和`-mbig-obj`链接选项。无论你使用何种编译器，你都需要将C语言版本设置为`C17`，将C++语言版本设置为`C++20`

5. 编译Juce框架，生成动态链接库

### 开始编写模块
在编写模块前，你需要进行如下准备工作：

1. 添加在上一步中生成的`JuceHeader.h`文件，并将其中`#include <juce_audio_plugin_client/juce_audio_plugin_client.h>`一行代码注释

2. 将Juce源码文件夹中`modules`目录作为包含文件路径添加到你的项目中

3. 将克隆的`libJModule`目录作为包含文件路径添加到你的项目中，并将其中全部源文件添加进项目

4. 将上一步中生成的Juce动态库链接到你的项目中

5. 在你的项目中添加如下的预处理器定义：
```
JUCE_GLOBAL_MODULE_SETTINGS_INCLUDED=1
JUCE_DLL=1
JUCE_DISABLE_DLL_ALLOCATORS=1
JUCE_MODAL_LOOPS_PERMITTED=1
```
**另外，你需要为Debug编译配置添加预处理器定义`_DEBUG`，为Release编译配置添加预处理器定义`NDEBUG`**

6. 如果你使用MSVC编译器，则视情况使用`/MD`与`/MDd`编译选项之一，同时使用`/bigobj` `/Gd`编译选项与如下链接选项：`/DYNAMICBASE` `/LARGEADDRESSAWARE` `/SUBSYSTEM:WINDOWS`；如果你使用GCC或Clang编译器，则使用`-fPIC`编译选项和`-mbig-obj`链接选项。无论你使用何种编译器，你都需要将C语言版本设置为`C17`，将C++语言版本设置为`C++20`

### 模块类编写
包含`libJModule.h`头文件，建立继承自`jmadf::Module`类的模块类，**注意该模块类的类名须有足够的辨识度**，JMADF框架使用类名为不同模块分配各自的静态空间。  
在模块类中，你需要重写以下方法：
```cpp
public:
	virtual bool init() = 0;
	virtual void destory() = 0;

	virtual const juce::String getModuleName() = 0;
	virtual const juce::String getModuleVersion() = 0;
```

`bool init()`方法将在模块加载时被自动调用，你可以在其中初始化环境，申请资源，注册接口或加载其他模块，其返回值为模块加载成功或失败状态。

`void destory()`方法将在模块卸载时自动调用，你可以在其中销毁环境，释放资源或卸载其他模块。

`const juce::String getModuleName()`方法应返回模块名称，模块名称以`开发者.项目名.模块名`或`开发者.项目名.模块名.次级模块名....`的形式组织。

`const juce::String getModuleVersion()`方法应返回模块版本，模块版本以`MAJOR.MINOR.PATCH`的形式组织。

**需要特别注意的是，上述方法中的模块名称与模块版本不用于获取模块信息，其唯一作用为与模块配置信息比对，确认模块身份，故上述方法返回信息务必与模块配置文件中的信息保持一致，否则模块加载将会被框架拒绝！**

### 注册模块类
使用`JModuleInstance(className)`宏注册上一步中编写的模块类。

**注意，`JModuleInstance(className)`宏在整个模块项目中只可以使用一次！**

### 模块接口注册与调用
接口是模块间通信的必要方式，模块注册的接口将会被暴露给其他模块，其他模块可以使用注册接口的模块名与接口的key调用接口，在调用接口的同时可以向接口传递调用者的模块名与注册接口时指定数量类型的参数，libJModule库提供了如下方法注册与调用接口：
```cpp
namespace jmadf
{
	void RegisterInterface<typename ...T>(const juce::String& key, const std::function<const juce::String&, T...>& func)
	void CallInterface<typename ...T>(const juce::String& moduleId, const juce::String& key, T... args);
}
```

使用示例：
```cpp
jmadf::RegisterInterface<void>("test1", [](const juce::String& caller) {});
jmadf::RegisterInterface<size_t&, void*&>("test2", [](const juce::String& caller, size_t& a1, void*& a2) {a1 = 3; a2 = 0x1C; });

//==========================================================

jmadf::CallInterface<void>("Developer.Project.Test", "test1");

size_t p1 = 0;
void* p2 = nullptr;
jmadf::CallInterface<size_t&, void*&>("Developer.Project.Test", "test2", p1, p2);
//p1==3,p2==0x1C
```

**需要特别注意的是：**
- **接口的调用有一定的时间成本，请不要注册频繁调用的接口，这将会影响应用性能。必要时，可以利用接口传递函数或对象指针以应对频繁的模块间通信**
- **调用接口时应保证模板参数T与注册接口时的模板参数T类型与数量相同，JMADF会对参数类型进行检查，如参数类型不匹配，接口的调用将会失败**

### 在模块中加载模块
libJModule库提供了如下方法获取模块信息并对模块进行操作：
```cpp
namespace jmadf
{
	bool LoadModule(const juce::String& moduleId);
	void UnloadModule(const juce::String& moduleId);

	bool ModuleIsLoaded(const juce::String& moduleId);
	bool ModuleIsExists(const juce::String& moduleId);

	const juce::StringArray GetAllModules();
	const juce::StringArray GetAllModulesInGroup(const juce::String& groupName);

	const jmadf::ModuleInfo* FindModule(const juce::String& moduleId);
}
```

`bool LoadModule(const juce::String& moduleId)`通过模块名称加载模块，返回模块加载成功/失败，如果此前模块已经被加载，则会返回`true`。

`void UnloadModule(const juce::String& moduleId)`通过模块名称卸载模块，这可能不会立即使JMADF卸载该模块，模块将会根据加载/卸载次数动态调整引用计数，当引用计数为0时模块才会卸载。

**需要特别注意的是：**
- **模块不可以试图加载/卸载其本身，任何对模块本身的加载/卸载操作都将被屏蔽**
- **为了保证模块的依赖顺序，建议在模块类的`init`方法中加载其他模块，在模块类`destory`方法中逆序卸载其他模块**

`bool ModuleIsLoaded(const juce::String& moduleId)`查询模块是否已被加载。

`bool ModuleIsExists(const juce::String& moduleId)`查询模块是否存在于模块列表中。

`const juce::StringArray GetAllModules()`返回模块列表中的全部模块名称。

`const juce::StringArray GetAllModulesInGroup(const juce::String& groupName)`根据组名查找模块列表中属于该组的模块名称，组名以`产品.组名`的形式组织。

`const jmadf::ModuleInfo* FindModule(const juce::String& moduleId)`返回对应模块的模块配置信息。

### 错误信息
libJModule维护了一个错误信息队列，模块可以向队列中提交错误信息，并可对错误信息进行查询，相关方法如下：
```cpp
namespace jmadf
{
	void RaiseException(const juce::String& exception);
	const juce::String GetException();
	void ClearException();
}
```

`void RaiseException(const juce::String& exception)`向队列中提交一条错误信息，错误信息将自动在头部添加以`"[]"`包裹的模块名。不以模块名开头的错误信息为JMADF框架内部提交的错误信息。

`const juce::String GetException()`获取全部错误信息，每条错误信息间将以`'\n'`分隔。

`void ClearException()`清空错误信息队列。

### 合理放置你的模块
在编译之后，你将会获得一个动态链接库文件（`*.dll`或`*.so`或`*.dylib`），你需要进行如下操作：

1. 在VocalShaper安装目录下`modules`文件夹中建立新目录，目录名与模块名相同。

2. 将动态链接库放入新目录。

3. 在该目录中建立一个json文件，文件名与模块名保持一致，该文件为模块配置信息文件，其内容格式为：
```json
{
    "id":"Developer.Project.Module",
    "version":"major.minor.patch",
    "group":"Product.Group",
    "productId":"{xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx}",
    "entry":"Dynamic_Link_Library_File_Name",
    "description":"Some information about the module",
    "dependencies":
        [
            "xxx.xxx.xxx",
            "yyy.yyy.yyy",
            "zzz.zzz.zzz"
        ]
}
```

`id`模块名，注意与模块类`getModuleName`方法返回内容一致。

`version`模块版本，注意与模块类`getModuleVersion`方法返回内容一致。

`group`模块组名，为一个字符串或一个包含字符串的数组，`GetAllModulesInGroup`方法将以此为依据查找模块。

`productId`产品ID，格式为UUID格式，JMADF框架以此为依据识别模块所属产品。模块开发时应向产品开发者询问产品的UUID，否则JMADF将不会识别该模块。

**VocalShaper目前的产品ID为`"{08338208-B752-4A90-9C73-4163152D6818}"`**

`entry`JMADF框架将由此文件加载模块，故此处应设置为放入该目录的动态链接库文件名（不含扩展名）。

`description`一些与模块相关的信息，用于提示模块的功能。开发者可以视情况自行设置。

`dependencies`模块的依赖列表，列表中的成员必须是同产品下其它模块的模块名。此设置在应用运行时不会起作用，其唯一作用是在模块安装时检查依赖模块是否安装。

### 模块打包
**相关功能正在开发中**
