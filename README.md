# banabala_wiki
只是把放在VPS上的个人wiki搬到github上，记得很零散，还未来及整理，已经停更了改成evernote了……


| 日期         | 作者   | 版本   | 说明    |
| ---------- | ---- | ---- | ----- |
| 2018-01-15 | 赵青青  | 0.1  | 初始化笔记 |
|            |      |      |       |

## 功能开启接口

### 判断功能是否开启

示例：

```lua
local isOpen = DataCenter.functionOpen:IsOpenByUIName("Backpack")
```



附：如果某功能没有UI面板，或者说是与后端制定的某项逻辑，要怎样判断功能是否开启？

传入与后端通信的Id使用判断条件（查看**功能开启配置表** 找到配置id）

```lua
local isOpen = DataCenter.functionOpen:IsOpenById(1001)
```



### 功能开启事件

**事件注册**

```lua
---监听新功能开启
local isSkillOpen = DataCenter.functionOpen:IsOpenByUIName("Weapon")
if not isSkillOpen then
	EventMgr:on("NewFunctionOpen",self.OnFunctionOpen,self)
end
```

**事件监听和移除**

```lua
function UIBattle:OnFunctionOpen(funcName,id)
    --print("==========UIBattle:OnFunctionOpen=========",funcName,id)
    if funcName == "Weapon" then
		---TODO 开启技能的逻辑
        EventMgr:clear("NewFunctionOpen",self)
    end
end
```



## 策划配置

以下内容建议由程序提供给策划

必填

function_name 界面名称：UI面板的名字

battle_btn_name 飞到主界面图的图标名字 ：Battle中按钮的变量名（代码用到的变量名）



选填

ui_groups：

如果一个功能在主界面有两个以上的入口，比如装备收集，在主界面有装备收集图标，也可以从背包按钮中打开。配置方法如下：battle_btn_name 配置**equipCollectBtn**，在ui_groups配置**backpackBtn**



## 实现机制

### 全量开启

在第一次登录时，收到 已开启功能列表（30001），它对应的主界面图标就会亮起。

### 增量开启

核心：根据玩家等级 开启新功能

当玩家升级时，服务器推送 **新功能开启协议（30002）**，客户端显示主界面图标并进行动画表现

### 功能关闭

如果不幸的是某个模块收到 **功能关闭协议（30003）** ，目前也会被认定为 **功能未开启**

具体功能关闭还未实现，因为未排期。

### 组的概念

在同一个tabbar中的多个UI属于同个组，组名使用在主界面图标的按钮名（代码名）

**由来：**因为tabbar中某个标签页开了，主界面的入口就会显示（不显示就没有入口可以进入）

```lua
local isOpen = DataCenter.functionOpen:IsOpenByGroupName(主界面图标变量名)
```



## 做了那些事？

### 控制主界面图标显示隐藏

只控制了主界面的图标，以下未控制：

左侧组队，左侧任务

左上角人物头像，人物血条

右上角提示按钮，小地图

聊天消息框



### 控制Tabbar左侧图标显示隐藏

如果界面未使用统一的tabbar，不受控制。

判断是否为tabbar，看父类是否继承自**UITabBarController**



### 给Tabbar左侧图标加红点

当tabbar里的某个功能是新开启的，且按钮未点击，按钮上会出现红点，点击一次后消失。



### 给主界面图标加红点/特效

两种标识：红点，特效，可独立或多个存在。

根据策划配置，当有新功能开启时，给主界面图标加标识

且：如果里面的子页（tabbar图标）有红点未消失时，主界面标识不会消失

## 存在问题

目前不支持关闭tabbar的第一个tabbar按钮。

如果打开界面前未检查此功能是否开启，那么被关闭的功能也会被打开（tabbar除外）



