[TOC]

### 什么是LookAndFeel文件
LookAndFeel文件是`WuChang.JMADF.LookAndFeelConfigs`模块获取属性数据的依据，文件格式为`json`，用于指定组件的外观。

### LookAndFeel文件位置
LookAndFeel文件应放置于模块目录下`.laf`目录内，其文件扩展名为`.json`。

### LookAndFeel文件示例
```json
{
    "parent":
    {
        "module":"Developer.Product.ModuleName",
        "class":"main"
    },
    "widget1":
    {
        "attribute1":"value",
        "attribute2":0.0,
        "attribute3":false
    },
    "widget2":
    {
        "attribute4":"value",
        "attribute5":0.0,
        "attribute6":false,
        "attribute7":"#9999FF",
        "attribute8":"#9999FF66",
    }
}
```

### 控件与属性
- LookAndFeel文件中以属性为基本单位，多个属性可以组织成为一个控件。
- 每个LookAndFeel文件可以包含一个或多个控件。
- 属性的值可以为字符串、数值或布尔型。

### LookAndFeel的继承
`"parent"`是控件名的保留字。任何控件的命名都不能为`"parent"`。\
`"parent"`字段是可选的，当某个LookAndFeel文件中没有要查询的属性时，查询不会立即失败，而是会逐级到`"parent"`中指定的文件中查找，直到找到为止或直到某个LookAndFeel文件不存在有效的`"parent"`字段返回查询失败。

`"parent"`中的`"module"`属性是可选的，如果不存在该属性，则默认为当前模块。

`"parent"`中的`"class"`属性是必需的，它指定了到该模块目录下`.laf`文件夹中的哪个LookAndFeel文件中查找属性。该属性的值应与目标LookAndFeel文件的文件名（不含扩展名）相同。

### 颜色值格式
- 颜色值是一个字符串
- 颜色值有四种格式，分别为:`"RRGGBB"`, `"RRGGBBAA"`, `"#RRGGBB"`和`"#RRGGBBAA"`。
- 当`"AA"`不存在时，默认透明度为`"FF"`，即完全不透明。
- 颜色值开头有无`"#"`在意义上没有区别，但为了提高可读性，建议使用`"#"`开头的颜色值。

### `WuChang.JMADF.LookAndFeelConfigs`模块接口说明
**该模块开发中**
