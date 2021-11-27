//
// Created by LEGION on 2021/10/4.
//
#include "ControlTask.h"

bool ChassisCenterFlag = true;	/* 代表底盘旋转舵机是否可用 */
bool ChassisDoorFlag = true; 		/* 代表底盘关门舵机是否可用 */
bool ClawSpinFlag = true; 			/* 代表夹爪翻转舵机是否可用 */
bool ClawPanningFlag = true; 		/* 代表夹爪位移舵机是否可用 */
bool ClawCenterFlag = true; 		/* 代表夹爪中心舵机是否可用 */
bool AutoInOut = true;					/* 代表自动化装矿是否可用 */
bool AutoSpin = true;						/* 代表自动化旋转矿是否可用 */
bool AutoUpDown = true; 				/* 从地上夹着举到头顶 */
bool AutoPush = true;						/* 向前135°抛出 */

int PushCount = 0;							/* 记录向前抛出时间 */ 
int PrevPushCount = -1;					/* 记录向前抛出前一次时间*/
void CtrlHandle(){
    if (RemoteControl::rcInfo.sRight == DOWN_POS){//右侧三档，急停模式
        ChassisStop();
        UserStop();
		}
		
		else if (RemoteControl::rcInfo.sRight == MID_POS) { //右侧二挡，舵机调试控制模式
			
			int ch = 1; 
			switch (ch) {
				case 0:	/* 控制底盘转动舵机：测试程序 */
				{
					if (-0.5 < RemoteControl::rcInfo.left_col && RemoteControl::rcInfo.left_col < 0.5) ChassisCenterFlag = true;
					else if (RemoteControl::rcInfo.left_col > 0.5 && ChassisCenterFlag) {
						ChassisCenterFlag = false;
						SpinTo(ChassisCenter, 0);
					} else if (RemoteControl::rcInfo.left_col < -0.5 && ChassisCenterFlag) {
						ChassisCenterFlag = false;
						SpinTo(ChassisCenter, 180);
					};
					break;
				}
				case 1: /* 控制底盘关门舵机：测试程序 */
				{
					if (-0.5 < RemoteControl::rcInfo.left_col && RemoteControl::rcInfo.left_col < 0.5) ChassisDoorFlag = true;
					else if (RemoteControl::rcInfo.left_col > 0.5 && ChassisDoorFlag) {
						ChassisDoorFlag = false;
						SpinAdd(ChassisDoor, 90);
					} else if (RemoteControl::rcInfo.left_col < -0.5 && ChassisDoorFlag) {
						ChassisDoorFlag = false;
						SpinAdd(ChassisDoor, -90);
					};
					break;
				}
				case 2: /* 控制机械臂爪子转动：测试程序 */
				{
					if (-0.5 < RemoteControl::rcInfo.left_col && RemoteControl::rcInfo.left_col < 0.5) ClawSpinFlag = true;
					else if (RemoteControl::rcInfo.left_col > 0.5 && ClawSpinFlag) {
						ClawSpinFlag = false;
						SpinAdd(ClawSpinLeft, 90);
						SpinAdd(ClawSpinRight, -90);
					} else if (RemoteControl::rcInfo.left_col < -0.5 && ClawSpinFlag) {
						ClawSpinFlag = false;
						SpinAdd(ClawSpinLeft, -90);
						SpinAdd(ClawSpinRight, 90);
					};
					break;
				}
				case 3: /* 控制机械臂上下位移：测试程序 */
				{	
					if (-0.5 < RemoteControl::rcInfo.left_col && RemoteControl::rcInfo.left_col < 0.5) ClawPanningFlag = true;
					else if (RemoteControl::rcInfo.left_col > 0.5 && ClawPanningFlag) {
						ClawPanningFlag = false;
						SpinAdd(ClawPanningLeft, 90);
						SpinAdd(ClawPanningRight, -90);
					} else if (RemoteControl::rcInfo.left_col < -0.5 && ClawPanningFlag) {
						ClawPanningFlag = false;
						SpinAdd(ClawPanningLeft, -90);
						SpinAdd(ClawPanningRight, 90);
					};
					break;
				}
				case 4: /* 控制机械臂中心位移：测试程序 */
				{
					if (-0.5 < RemoteControl::rcInfo.left_col && RemoteControl::rcInfo.left_col < 0.5) ClawCenterFlag = true;
					else if (RemoteControl::rcInfo.left_col > 0.5 && ClawCenterFlag) {
						ClawCenterFlag = false;
						SpinAdd(ClawCenter, 90);
					} else if (RemoteControl::rcInfo.left_col < -0.5 && ClawCenterFlag) {
						ClawCenterFlag = false;
						SpinAdd(ClawCenter, -90);
					};
					break;
				}
				
			};
		} 
		
		else { // 右侧一档，正常模式
        switch (RemoteControl::rcInfo.sLeft) {
            case UP_POS:	//左侧一档，控制底盘运动
								/* 右摇杆前后拨动控制灵敏性，右摇杆左右拨动控制灵敏性，左摇杆左右拨动控制灵敏性 */
								/* * 后的是一个系数，可以调节到最舒适的手感 */
                ChassisSetVelocity(RemoteControl::rcInfo.right_col*3.8,
                                   RemoteControl::rcInfo.right_rol*3.8,RemoteControl::rcInfo.left_rol*50);
                break;
            case MID_POS:	//左侧二档，自动化控制 + 微调舵机模式
						{
							/* 控制机械臂完成抓取并放入车内 */
							//if (-0.9 < RemoteControl::rcInfo.left_col && RemoteControl::rcInfo.left_col < 0.9)
								//AutoInOut = true;
							//else if (RemoteControl::rcInfo.left_col > 0.9 && AutoInOut)	{ /* 装矿 */
								//AutoInOut = false;
								///* 松开爪子 */
								//SpinTo(ClawCenter, 140);
								///* 将爪子放下 */
								//SpinTo(ClawPanningLeft, 0);
								//SpinTo(ClawPanningRight, 0);
								///* 夹紧爪子 */
								//SpinTo(ClawCenter, 50);
								///* 再开门 */
								//SpinTo(ChassisDoor, 180);
								///* 平移舵机位移，使得爪子在框上方 */
								//SpinTo(ClawPanningLeft, 180);
								//SpinTo(ClawPanningRight, 180);
								/* 松开爪子 */
								//SpinTo(ClawCenter, 140);
								/* 爪子归位 */
								//SpinTo(ClawPanningLeft, 90);
								//SpinTo(ClawPanningRight, 90);
								/* 再关门 */
								//SpinTo(ChassisDoor, 0);
								/* 旋转底盘使得空闲盒子在等待位置 */
								//SpinAdd(ChassisCenter, 90);
								//AutoInOut = true;
							//} else if (RemoteControl::rcInfo.left_col < -0.9 && AutoInOut) { /* 将框取出放到地上 */
								//AutoInOut = false;
								/* 松开爪子 */
								//SpinTo(ClawCenter, 140);
								/* 平移舵机位移，使得爪子在框上方 */
								//SpinTo(ClawPanningLeft, 180);
								//SpinTo(ClawPanningRight, 180);
								/* 夹紧爪子 */
								//SpinTo(ClawCenter, 50);
								/* 平移舵机位移，使得爪子放在地上 */
								//SpinTo(ClawPanningLeft, 0);
								//SpinTo(ClawPanningRight, 0);
								/* 松开爪子 */
								//SpinTo(ClawCenter, 140);
								/* 爪子归位 */
								//SpinTo(ClawPanningLeft, 90);
								//SpinTo(ClawPanningRight, 90);
								/* 旋转底盘使得有球盒子在等待位置 */
								//SpinAdd(ChassisCenter, -90);
								//AutoInOut = true;
							//} else {};
							
							
							/* 从地上夹上 & 放到地上 */
							//if (-0.5 < RemoteControl::rcInfo.right_col && RemoteControl::rcInfo.right_col < 0.5) AutoUpDown = true;
							//else if (RemoteControl::rcInfo.right_col < -0.5 && AutoUpDown) {
								//AutoUpDown = false;
								/* 松开爪子 */
								//SpinTo(ClawCenter, 140);
								/* 将爪子放下 */
								//SpinTo(ClawPanningLeft, 0);
								//SpinTo(ClawPanningRight, 0);
								/* 夹紧爪子 */
								//SpinTo(ClawCenter, 50);
								/* 举起爪子 */
								//SpinTo(ClawPanningLeft, 90);
								//SpinTo(ClawPanningRight, 90);
							//} else if (RemoteControl::rcInfo.right_col > 0.5 && AutoUpDown) {
								//AutoUpDown = false;
								/* 将爪子放下 */
								//SpinTo(ClawPanningLeft, 0);
								//SpinTo(ClawPanningRight, 0);
								/* 松开爪子 */
								//SpinTo(ClawCenter, 140);
								/* 举起爪子 */
								//SpinTo(ClawPanningLeft, 90);
								//SpinTo(ClawPanningRight, 90);
							//} else {};
								
							/* 旋转方块 */
							//if (-0.5 < RemoteControl::rcInfo.right_rol && RemoteControl::rcInfo.right_rol < 0.5) AutoSpin = true;
							//else if (RemoteControl::rcInfo.right_rol > 0.5 && AutoSpin) {
								//AutoSpin = false;
								//SpinAdd(ClawSpinLeft, -90.0);
								//SpinAdd(ClawSpinRight, 90.0);
							//} else if (RemoteControl::rcInfo.right_rol < -0.5 && AutoSpin) {
								//AutoSpin = false;
								//SpinAdd(ClawSpinLeft,90.0);
								//SpinAdd(ClawSpinRight, -90.0);
							//}
							
							/* 左右移动盒子 */
							//if (-0.5 < RemoteControl::rcInfo.left_rol && RemoteControl::rcInfo.left_rol < 0.5) ChassisCenterFlag = true;
							//else if (RemoteControl::rcInfo.left_rol > 0.5 && ChassisCenterFlag) {
								//ChassisCenterFlag = false;
								//SpinTo(ChassisCenter, 240);
							//} else if (RemoteControl::rcInfo.left_rol < -0.5 && ChassisCenterFlag) {
								//ChassisCenterFlag = false;
								//SpinTo(ChassisCenter, 0);
							//} else {};
							
							break;
						}
						case DOWN_POS: //左侧三挡，手动舵机模式
						{
							/* 控制底盘旋转 */
							/* 可正常转动，注意调节参数让其每次能够旋转 90 ° */
							//if (-0.5 < RemoteControl::rcInfo.left_col && RemoteControl::rcInfo.left_col < 0.5) ChassisCenterFlag = true;
							//else if (RemoteControl::rcInfo.left_col > 0.5 && ChassisCenterFlag) {
								//ChassisCenterFlag = false;
								//SpinTo(ChassisCenter, 240);
							//} else if (RemoteControl::rcInfo.left_col < -0.5 && ChassisCenterFlag) {
								//ChassisCenterFlag = false;
								//SpinTo(ChassisCenter, 0);
							//} else {};
							
							/* 控制向前抛出 */
							PushCount++;
							if (PrevPushCount != -1 && PushCount - PrevPushCount > 400) {
								PrevPushCount = -1;
								SpinTo(ClawCenter, 140); /* 松爪子 */
							} else if (-0.5 < RemoteControl::rcInfo.left_col && RemoteControl::rcInfo.left_col < 0.5) ChassisCenterFlag = true;
							else if (RemoteControl::rcInfo.left_col > 0.5 && ChassisCenterFlag) {
								ChassisCenterFlag = false;
								SpinTo(ClawPanningLeft, 75);
								SpinTo(ClawPanningRight, 75);
								PrevPushCount = PushCount;
							} else if (RemoteControl::rcInfo.left_col < -0.5 && ChassisCenterFlag) {
								ChassisCenterFlag = false;
								SpinTo(ClawPanningLeft, 180);
								SpinTo(ClawPanningRight, 180);
								SpinTo(ClawCenter, 140); /* 松爪子 */
							} else {};
								
							/* 控制机械臂夹紧 */
							/* 可正常转动，向外松，向内紧，收紧需要调松，OKOK */
							if (-0.5 < RemoteControl::rcInfo.right_rol && RemoteControl::rcInfo.right_rol < 0.5) ClawCenterFlag = true;
							if (RemoteControl::rcInfo.right_rol > 0.5 && ClawCenterFlag) {
								ClawCenterFlag = false;
								SpinTo(ClawCenter, 140.0); // 松
							} else if (RemoteControl::rcInfo.right_rol < -0.5 && ClawCenterFlag) {
								ClawCenterFlag = false;
								SpinTo(ClawCenter, 55.0); // 紧
							} else {};

							/* 控制机械臂平移舵机移动，注意两个舵机方向相反, OKOK */
							if (-0.5 < RemoteControl::rcInfo.right_col && RemoteControl::rcInfo.right_col < 0.5) ClawPanningFlag = true;
							else if (RemoteControl::rcInfo.right_col > 0.5 && ClawPanningFlag) {
								ClawPanningFlag = false;
								SpinAdd(ClawPanningLeft, -90.0);
								SpinAdd(ClawPanningRight, -90.0);
							} else if (RemoteControl::rcInfo.right_col < -0.5 && ClawPanningFlag) {
								ClawPanningFlag = false;
								SpinAdd(ClawPanningLeft, 90.0);
								SpinAdd(ClawPanningRight, 90.0);
							} else {};
								
							/* 控制机械臂爪子舵机翻转，注意两个舵机方向相反, OKOK */
							/* 可正常转动，已经成功每次 90 度 */
							//if (-0.5 < RemoteControl::rcInfo.right_rol && RemoteControl::rcInfo.right_rol < 0.5) ClawSpinFlag = true;
							//else if (RemoteControl::rcInfo.right_rol > 0.5 && ClawSpinFlag) {
								//ClawSpinFlag = false;
								//SpinAdd(ClawSpinLeft, -90.0);
								//SpinAdd(ClawSpinRight, 90.0);
							//} else if (RemoteControl::rcInfo.right_rol < -0.5 && ClawSpinFlag) {
								//ClawSpinFlag = false;
								//SpinAdd(ClawSpinLeft,90.0);
								//SpinAdd(ClawSpinRight, -90.0);
							//} else {};
						}
						default:
								break;
        }

    }

}