//
// Created by LEGION on 2021/10/4.
//
#include "ControlTask.h"

bool ChassisCenterFlag = true;	/* 代表底盘旋转舵机是否可用 */
bool ChassisDoorFlag = true; 		/* 代表底盘关门舵机是否可用 */
bool ClawSpinFlag = true; 			/* 代表夹爪翻转舵机是否可用 */
bool ClawPanningFlag = true; 		/* 代表夹爪位移舵机是否可用 */
bool ClawCenterFlag = true; 		/* 代表夹爪中心舵机是否可用 */
bool AutoIn = true;							/* 代表自动化装矿是否可用 */
bool AutoOut = true; 						/* 代表自动化丢出矿是否可用 */
bool AutoSpin = true;						/* 代表自动化旋转矿是否可用 */

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
                ChassisSetVelocity(RemoteControl::rcInfo.left_col*4.2,
                                   RemoteControl::rcInfo.left_rol*4.2,RemoteControl::rcInfo.right_rol*60);
                break;
            case MID_POS:	//左侧二档，自动化控制 + 微调舵机模式
						{
							/* 控制机械臂完成抓取并放入车内 */
							if (-0.9 < RemoteControl::rcInfo.left_col && RemoteControl::rcInfo.left_col < 0.9) {}
								// AutoIn = AutoOut = true;
							else if (RemoteControl::rcInfo.left_col > 0.9 && AutoIn)	{ /* 装矿 */
								AutoIn = false;
								/* 夹紧爪子 */
								SpinTo(ClawCenter, 90);
								/* 再开门 */
								SpinTo(ChassisDoor, 180);
								/* 平移舵机位移，使得爪子在框上方 */
								SpinTo(ClawPanningLeft, 180);
								SpinTo(ClawPanningRight, 0);
								/* 松开爪子 */
								SpinTo(ClawCenter, 0);
								/* 平移舵机位移，使得爪子归位 */
								SpinTo(ClawPanningLeft, 0);
								SpinTo(ClawPanningRight, 180);
								/* 再关门 */
								SpinTo(ChassisDoor, 0);
								/* 旋转底盘使得空闲盒子在等待位置 */
								SpinAdd(ChassisCenter, 90);
								AutoIn = true;
							} else if (RemoteControl::rcInfo.left_col < -0.9) { /* 将框取出放到地上 */
								AutoIn = false;
								/* 松开爪子 */
								SpinTo(ClawCenter, 0);
								/* 平移舵机位移，使得爪子在框上方 */
								SpinTo(ClawPanningLeft, 180);
								SpinTo(ClawPanningRight, 0);
								/* 夹紧爪子 */
								SpinTo(ClawCenter, 90);
								/* 平移舵机位移，使得爪子归位 */
								SpinTo(ClawPanningLeft, 0);
								SpinTo(ClawPanningRight, 180);
								/* 旋转底盘使得有球盒子在等待位置 */
								SpinAdd(ChassisCenter, -90);
								AutoOut = true;
							} else {};
							
							/* 控制机械臂微小平移，注意两个舵机方向相反 */
							if (-0.3 < RemoteControl::rcInfo.right_col && RemoteControl::rcInfo.right_col < 0.3) ClawPanningFlag = true;
							else if (RemoteControl::rcInfo.right_col > 0.3) {
								RemoteControl::rcInfo.right_col = false;
								SpinAdd(ClawPanningLeft, 5.0);
								SpinAdd(ClawPanningRight, -5.0);
							} else if (RemoteControl::rcInfo.right_col < 0.3) {
								RemoteControl::rcInfo.right_col = false;
								SpinAdd(ClawPanningLeft, -5.0);
								SpinAdd(ClawPanningRight, 5.0);
							} else {};
							
							/* 控制机械臂微小翻转，注意两个舵机方向相反 */
							if (-0.3 < RemoteControl::rcInfo.right_rol && RemoteControl::rcInfo.right_rol < 0.3) ClawSpinFlag = true;
							else if (RemoteControl::rcInfo.right_rol > 0.3) {
								ClawSpinFlag = false;
								SpinAdd(ClawSpinLeft, 5.0);
								SpinAdd(ClawSpinRight, -5.0);
							} else if (RemoteControl::rcInfo.right_rol < -0.3) {
								ClawSpinFlag = false;
								SpinAdd(ClawSpinLeft, -5.0);
								SpinAdd(ClawSpinRight, 5.0);
							} else {};
							
							/* 控制机械臂微小夹紧，注意两个舵机方向相反*/
							if (-0.3 < RemoteControl::rcInfo.dialWheel && RemoteControl::rcInfo.dialWheel < 0.3) ClawCenterFlag = true;
							else if (RemoteControl::rcInfo.dialWheel > 0.3) {
								ClawCenterFlag = false;
								SpinAdd(ClawCenter, 5.0);
							} else if (RemoteControl::rcInfo.dialWheel < -0.3) {
								ClawCenterFlag = false;
								SpinAdd(ClawCenter, -5.0);
							} else {};

							break;
						}
						case DOWN_POS: //左侧三挡，手动舵机模式
						{
							/* 控制底盘旋转 */
							/* 可正常转动，注意调节参数让其每次能够旋转 90 ° */
							if (-0.5 < RemoteControl::rcInfo.left_col && RemoteControl::rcInfo.left_col < 0.5) ChassisCenterFlag = true;
							else if (RemoteControl::rcInfo.left_col > 0.5 && ChassisCenterFlag) {
								ChassisCenterFlag = false;
								SpinAdd(ChassisCenter, 72.0);
							} else if (RemoteControl::rcInfo.left_col < -0.5 && ChassisCenterFlag) {
								ChassisCenterFlag = false;
								SpinAdd(ChassisCenter, -72.0);
							} else {};
								
							/* 控制机械臂夹紧 */
							/* 可正常转动，向外松，向内紧，收紧需要调松 */
							if (-0.5 < RemoteControl::rcInfo.left_rol && RemoteControl::rcInfo.left_rol < 0.5) ClawCenterFlag = true;
							if (RemoteControl::rcInfo.left_rol > 0.5 && ClawCenterFlag) {
								ClawCenterFlag = false;
								SpinTo(ClawCenter, 0.0);
							} else if (RemoteControl::rcInfo.left_rol < -0.5 && ClawCenterFlag) {
								ClawCenterFlag = false;
								SpinTo(ClawCenter, 90.0);
							} else {};

							/* 控制机械臂平移舵机移动，注意两个舵机方向相反 */
							/* 有个舵机没有反应，可能损坏 */
							if (-0.5 < RemoteControl::rcInfo.right_col && RemoteControl::rcInfo.right_col < 0.5) ClawPanningFlag = true;
							else if (RemoteControl::rcInfo.right_col > 0.5 && ClawPanningFlag) {
								ClawPanningFlag = false;
								SpinAdd(ClawPanningLeft, 90.0);
								SpinAdd(ClawPanningRight, -90.0);
							} else if (RemoteControl::rcInfo.right_col < 0.5 && ClawPanningFlag) {
								ClawPanningFlag = false;
								SpinAdd(ClawPanningLeft, -90.0);
								SpinAdd(ClawPanningRight, 90.0);
							} else {};
								
							/* 控制机械臂爪子舵机翻转，注意两个舵机方向相反 */
							/* 可正常转动，已经成功每次 90 度 */
							if (-0.5 < RemoteControl::rcInfo.right_rol && RemoteControl::rcInfo.right_rol < 0.5) ClawSpinFlag = true;
							else if (RemoteControl::rcInfo.right_rol > 0.5 && ClawCenterFlag) {
								ClawSpinFlag = false;
								SpinAdd(ClawSpinLeft, 90.0);
								SpinAdd(ClawSpinRight, -90.0);
							} else if (RemoteControl::rcInfo.right_rol < -0.5 && ClawCenterFlag) {
								ClawSpinFlag = false;
								SpinAdd(ClawSpinLeft, -90.0);
								SpinAdd(ClawSpinRight, 90.0);
							} else {};
						}
						default:
								break;
        }

    }

}