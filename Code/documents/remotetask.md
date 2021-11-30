## 遥控器的使用和程序编写

### 遥控器的物理档位

首先介绍遥控器上的物理档位，遥控器示意图如下图所示。在本次校内赛中，电控框架给出了如下几种控制方式的编写接口：

1. 三位开关S1
2. 三位开关S2
3. 左右遥杆
4. 云台俯仰控制拨轮

![遥控器正视图](graph\remotePic.png)

### 代码介绍

在介绍完以上几种控制按键后，下面来介绍这几种控制方式的代码，注意，该代码存在于`RM_Frame_C\userCode\tasks\Src`的`Control.cpp`文件中，通过在该程序中对已经写好的其他功能接口（如底盘运动、控制舵机）进行调用即可实现通过遥控器对车辆和其功能的控制：

#### 三位开关S1

位于遥控器右上角，该开关对应变量：

```cpp
RemoteControl::rcInfo.sRight 
```

该变量的值包括`UP_POS,MID_POS,DOWN_POS`分别对应了开关的上中下三档。

注意，该变量一般不用更改，仅仅明白这个变量的意义即可

#### 三位开关S2

位于遥控器左上角，该开关对应变量：

```cpp
RemoteControl::rcInfo.sLeft
```

该变量的值包括`UP_POS,MID_POS,DOWN_POS`分别对应了开关的上中下三档。

注意，该变量已经在框架里面给出：

```cpp
switch (RemoteControl::rcInfo.sLeft) {
    case UP_POS://左侧一档

        break;
    case MID_POS://左侧二档

        break;
    case DOWN_POS:default://左侧三档

        break;
}
```

#### 左右遥杆

左右遥杆分别位于遥控器中间部分的左右两侧。

左摇杆:

```cpp
float RemoteControl::rcInfo.left_col;//上下值，向上返回值大于0、向下小于0
float RemoteControl::rcInfo.left_rol;//左右值，向右返回值大于0、向左小于0
```

右摇杆:

```cpp
float RemoteControl::rcInfo.right_col;//上下值，向上返回值大于0、向下小于0
float RemoteControl::rcInfo.right_rol;//左右值，向右返回值大于0、向左小于0
```

以上值均返回一个-1~1的float类型数。

如以下示例：

这里的`ChassisSetVelocity()`为控制底盘速度的接口（具体实现见本教程相关内容），以下函数实现了右摇杆上下拨动控制前后移动（`RemoteControl::rcInfo.right_col`）、右摇杆左右拨动控制左右移动（`RemoteControl::rcInfo.right_rol`）、左摇杆左右拨动控制左右旋转（`RemoteControl::rcInfo.left_rol`）。（注意这里的`4.2`是一个系数，表明移动的速度，需要调一个合适的值）

```cpp
ChassisSetVelocity(RemoteControl::rcInfo.right_col*4.2,                     RemoteControl::rcInfo.right_rol*4.2,RemoteControl::rcInfo.left_rol*60);
```

#### 云台俯仰控制拨轮

控制拨轮位于遥控器左上角。

```cpp
float RemoteControl::rcInfo.dialWheel;//前后拨，往后大于0，往前小于0
```

返回一个-1~1的float类型数，注意拨轮往后拨动返回值大于0，往前拨动返回值小于0。

具体使用方法与左右遥杆类似，这里不再赘述。
