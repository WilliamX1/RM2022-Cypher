# RM2022 - Cypher

## reference
* [2022RM校内赛规则](./2022RM校内赛规则V1.1)
* [RoboMaster 2022云汉杯校内赛电控框架说明文档 (1)](./Resource/RoboMaster 2022云汉杯校内赛电控框架说明文档 (1).pdf)
* [校内赛电控第二次培训](./Resource/校内赛电控第二次培训.pdf)
* [第一次宣讲视频](https://www.bilibili.com/video/BV1JL411s76t?spm_id_from=333.999.0.0)
* [底盘运动拆解分析](https://zhuanlan.zhihu.com/p/20282234)
## working log
* 2021/10/2            创建 git 仓库，下载并查看校内赛规则
* 2021/10/10          看完校内赛第一次宣讲视频
* 2021/10/11          配置电控环境
* 2021/10/13          学创碰头，编译成功校赛官方代码
* 2021/10/16          参加学创电控第一次培训
* 2021/11/3            学创调官方车
* 2021/11/4            总结第一次调车文档之底盘控制
* 2021/11/6            熟悉框架
* 2021/11/10          学创调自己车底盘，连接电调和中心板。
* 2021/11/23          学创调整舵机。
* 2021/11/24          学创调试舵机，失败，完成大部分代码，PID 未调节好。

## task
### 底盘运动控制
* 4 个麦轮(45 度角) + 4 个 3508 电机
* 参考[这篇文章](https://zhuanlan.zhihu.com/p/20282234)

$$
\vec{v} = \vec{v_t} + \vec\omega \times \vec{r}
$$
![1.png](./resource/1.png)

推导得出如下公式
$$
v_{FR} = v_y - v_x + \omega(a + b)
$$
$$
v_{FL} = v_y + v_x - \omega(a + b)
$$
$$
v_{BL} = v_y - v_x - \omega(a + b)
$$
$$
v_{BR} = v_y + v_x - \omega(a + b)
$$

* **a, b 相等于是控制速度的参数！**

```C++
@param fbVelocity /* 前后平移速度 */
@param lrVelocity /* 左右平移速度 */
@param rtVelocity /* 自旋速度 */
/* 设定 a + b 是一个可调参数*/
define a+b aPLUSb
/* 四个轮子线速度，单位：m / s */
CMFRSpeed = fbVelocity - lrVelocity + rtVelocity * aPLUSb; /* 左前轮线速度 */
CMFLSpeed = fbVelocity + lrVelocity - rtVelocity * aPLUSb; /* 右前轮线速度 */
CMBLSpeed = fbVelocity - lrVelocity - rtVelocity * aPLUSb; /* 左后轮线速度 */
CMBRSpeed = fbVelocity + lrVelocity + rtVelocity * aPLUSb; /* 右后轮线速度 */
```

#### 调车方式
* 先上电，短按开关一次（有亮灯），长按开关一次（有顺序亮灯），再打开开关上面的开关，即可开机，关机顺序相反。
* 检查四个电桥所连接 **马达 ID **，检查方式是按旁边的按钮并观察闪烁次数，更改代码与之匹配。

```C++
/* ChassisTask 文件中 */
/* MOTOR_ID_1 即与闪烁次数匹配即可 */
Motor CMFL(MOTOR_ID_1,&chassisMotorInit);//定义左前轮电机
Motor CMFR(MOTOR_ID_2,&chassisMotorInit);//定义右前轮电机
Motor CMBL(MOTOR_ID_3,&chassisMotorInit);//定义左后轮电机
Motor CMBR(MOTOR_ID_4,&chassisMotorInit);//定义右后轮电机
```

* 根据轮子转动方向来分别调节 `CMFLSpeed` ，`CMFRSpeed` ，`CMBLSpeed` ，`CMBRSpeed` 。现在所用是目前官方车的正确方向。

先根据**前进**方向是否正确，分别确定每个轮子的 `fbVelocity` 的正负方向。

根据**左右平移**方向是否正确，分别确定每个轮子的 `lrVelocity` 的正负方向。

根据**顺逆旋转**方向是否正确，分别确定每个轮子的 `rtVelocity` 的正负方向。

**_目前 `aPLUSb` 还没有调过。_**

```C++
/* ChassisTask 文件中 */
CMFLSpeed = fbVelocity + lrVelocity + rtVelocity * aPLUSb; /* 左前轮线速度 */
CMFRSpeed = -fbVelocity + lrVelocity + rtVelocity * aPLUSb; /* 右前轮线速度 */
CMBLSpeed = fbVelocity - lrVelocity + rtVelocity * aPLUSb; /* 左后轮线速度 */
CMBRSpeed = -fbVelocity - lrVelocity + rtVelocity * aPLUSb; /* 右后轮线速度 */
```

### 底盘电机 PID 调节
底盘电机使用 pid 单环控制，直接修改 pid 对应参数及控制量上限数值即可，建议逐步缓慢调大，尽量避免超调。

* P control
主要调节作用，可以**加快调节**，减小误差，存在稳态误差。**P 控制刹车快慢。**

* PI control
添加与误差对时间积分成比例的项。**减少静态误差**，但会**增加调节时间**、**增大超调量**。

* PID control
PI 控制器加上与误差对时间的导数成正比的一项。**减小调节时间**，具有**预见性**，但**容易受到噪声干扰**。

```C++
PID_Regulator_t pidRegulator = {//此为储存pid参数的结构体，四个底盘电机共用
        .kp = 60,
        .ki = 0,
        .kd = 0,
        .componentKpMax = 16384,
        .componentKiMax = 0,
        .componentKdMax = 0,
        .outputMax = 16384 //3508电机输出电流上限 16384 ，可以调小，勿调大
};
```

#### 调车方式
$$
componentKpMax + componentKiMax + componentKdMax = outputMax
$$

### 底盘接线
#### 烧写器
用于烧写程序代码，一端为 USB 接口连接电脑，一端连接_主控板_上 SWD 接口。

![烧写器](./Resource/烧写器.png)

烧写器线连接时**注意顺序**，烧写器接口如下，目前本车已经接好无需重新接线。

![SWD线序](./Resource/SWD线序.png)

#### 电调
我们组用 3508 电机和 C620 电调配套，可以使用杜邦线来调整 ID。戳 **SET** 口几下就设置成几号 ID，**SET** 口连续亮几下就代表几号 ID。

1. 电调正常工作状态下，短按 1 次 **SET** 按键，进入独立设置 ID 模式，此时指示灯熄灭。
2. 在独立设置 ID 模式下，短按 **SET** 按键的次数（不超过 8 次）即为设置的 ID 号。每次有效短按，指示灯将橙灯闪烁 1 次。
3. 若 3 秒未对 **SET** 按键进行操作，电调将自动保存当前设置 ID 号。设置完 ID 的电调需要重新上电才能进入正常工作状态。

![电调](./Resource/电调.png)

[1] 号口是 **7-Pin 电机数据端口**，连接 **M3508 直流无刷减速电机** 进行数据交互。
[2] 号口是 **三相动力线接头**，连接 **M3508 直流无刷减速电机** 的**三相输入接头**相连接。
[5] 号口是 **电源线**，连接 **中心板**。
[7] 号口试 **CAN 信号端口**，连接 **中心板**。

#### 中心板
用于连接 4 个电调的 **电源线** 和 **CAN 线** 和 1 个主控板。用于供电。

且所有插头在固定后不应该受力，中心板侧面不要和铝材或其他导电体接触防止破损后短路。

![中心板](./Resource/中心板.png)

#### 电池座和电池
电池开关：短按后长按。
电池架开关：用于导出电池的电流。

![电池座和电池](./Resource/电池座和电池.png)

#### 遥控器和接收机
接收机使用附带的先连接至 **中心板** 的 **Dbus** 接口，用于配对遥控器。

#### 主控板
PWM 用于连接控制舵机。

![主控板](./Resource/主控板.png)

