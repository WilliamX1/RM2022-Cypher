//
// Created by LEGION on 2021/10/4.
//
#include "UserTask.h"

PID_Regulator_t userPidRegulator = {
        .kp = 60,
        .ki = 0,
        .kd = 0,
        .componentKpMax = 10000,
        .componentKiMax = 0,
        .componentKdMax = 0,
        .outputMax = 10000 //2006电机输出电流上限，可以调小，勿调大
};

PID_Regulator_t userPidAngleRegulator = {
				.kp = 10,
				.ki = 0,
				.kd = 0,
				.componentKpMax = 100,
				.componentKiMax = 0,
				.componentKdMax = 0,
				.outputMax = 1000
};

MOTOR_INIT_t userMotorInit = {
        .speedPIDp = &userPidRegulator,
        .anglePIDp = nullptr,
        ._motorID = MOTOR_ID_1,
        .reductionRatio = 36.0f,
        .ctrlType = POSITION_Double,
};

Motor UserMotor(MOTOR_ID_5,&userMotorInit);

/* 定义并声明舵机 */

/* 底盘转盘舵机 */
/* DS3230 270° */
SERVO_INIT_T Servo_Center_Init = {		// 初始化函数名称
	.servoType = POSITION_270,			// 需要自定义 270° 电机，在 Servo.cpp 中定义相关动作
	.servoID = SERVO_ID_1, 					// 舵机 ID: 由 SERVO_ID_1 ~ SERVO_ID_7 组成，与硬件接线有关，不可重复
	.firstAngle = 0, 								// 开机角度：舵机初次上电时转到的角度
	.angleLimit_Min = 0, 						// 最小角度：舵机可以转到的最小角度 
	.angleLimit_Max = 270, 					// 最大角度：舵机可以转到的最大角度
};
Servo ChassisCenterServo(&Servo_Center_Init); 	// 声明舵机，调用先前的初始化函数

/* 底盘前后左右（所有方向）开关门舵机 */
/* 180° */
SERVO_INIT_T Servo_FB_Init = {		// 初始化函数名称
	.servoType = POSITION_180,			// 需要自定义 270° 电机，在 Servo.cpp 中定义相关动作
	.servoID = SERVO_ID_2, 					// 舵机 ID: 由 SERVO_ID_1 ~ SERVO_ID_7 组成，与硬件接线有关，不可重复
	.firstAngle = 0, 								// 开机角度：舵机初次上电时转到的角度
	.angleLimit_Min = 0, 						// 最小角度：舵机可以转到的最小角度 
	.angleLimit_Max = 180, 					// 最大角度：舵机可以转到的最大角度
};
Servo ChassisDoorServo(&Servo_FB_Init);		// 声明舵机，调用先前的初始化函数

/* 机械臂中心舵机 */
SERVO_INIT_T Servo_Mid_Init = {		// 初始化函数名称
	.servoType = POSITION_180,			// 需要自定义 270° 电机，在 Servo.cpp 中定义相关动作
	.servoID = SERVO_ID_3, 					// 舵机 ID: 由 SERVO_ID_1 ~ SERVO_ID_7 组成，与硬件接线有关，不可重复
	.firstAngle = 0, 								// 开机角度：舵机初次上电时转到的角度
	.angleLimit_Min = 0, 						// 最小角度：舵机可以转到的最小角度 
	.angleLimit_Max = 180, 					// 最大角度：舵机可以转到的最大角度
};
Servo ClawCenterServo(&Servo_Mid_Init);		// 声明舵机，调用先前的初始化函数

/* 机械臂伸缩臂舵机（左边） */
SERVO_INIT_T Servo_Panning_L_Init = {		// 初始化函数名称
	.servoType = POSITION_180,			// 需要自定义 270° 电机，在 Servo.cpp 中定义相关动作
	.servoID = SERVO_ID_4, 					// 舵机 ID: 由 SERVO_ID_1 ~ SERVO_ID_7 组成，与硬件接线有关，不可重复
	.firstAngle = 0, 								// 开机角度：舵机初次上电时转到的角度
	.angleLimit_Min = 0, 						// 最小角度：舵机可以转到的最小角度 
	.angleLimit_Max = 180, 					// 最大角度：舵机可以转到的最大角度
};
Servo ClawPanningLeftServo(&Servo_Panning_L_Init);		// 声明舵机，调用先前的初始化函数

/* 机械臂伸缩臂舵机（右边） */
SERVO_INIT_T Servo_Panning_R_Init = {		// 初始化函数名称
	.servoType = POSITION_180,			// 需要自定义 270° 电机，在 Servo.cpp 中定义相关动作
	.servoID = SERVO_ID_5, 					// 舵机 ID: 由 SERVO_ID_1 ~ SERVO_ID_7 组成，与硬件接线有关，不可重复
	.firstAngle = 0, 								// 开机角度：舵机初次上电时转到的角度
	.angleLimit_Min = 0, 						// 最小角度：舵机可以转到的最小角度 
	.angleLimit_Max = 180, 					// 最大角度：舵机可以转到的最大角度
};
Servo ClawPanningRightServo(&Servo_Panning_R_Init);		// 声明舵机，调用先前的初始化函数

/* 机械臂转动爪子（左边） */
SERVO_INIT_T Servo_Spin_L_Init = {		// 初始化函数名称
	.servoType = POSITION_180,			// 需要自定义 270° 电机，在 Servo.cpp 中定义相关动作
	.servoID = SERVO_ID_7, 					// 舵机 ID: 由 SERVO_ID_1 ~ SERVO_ID_7 组成，与硬件接线有关，不可重复
	.firstAngle = 0, 								// 开机角度：舵机初次上电时转到的角度
	.angleLimit_Min = 0, 						// 最小角度：舵机可以转到的最小角度 
	.angleLimit_Max = 180, 					// 最大角度：舵机可以转到的最大角度
};
Servo ClawSpinLeftServo(&Servo_Spin_L_Init);		// 声明舵机，调用先前的初始化函数

/* 机械臂转动爪子（右边） */
SERVO_INIT_T Servo_Spin_R_Init = {		// 初始化函数名称
	.servoType = POSITION_180,			// 需要自定义 270° 电机，在 Servo.cpp 中定义相关动作
	.servoID = SERVO_ID_7, 					// 舵机 ID: 由 SERVO_ID_1 ~ SERVO_ID_7 组成，与硬件接线有关，不可重复
	.firstAngle = 0, 								// 开机角度：舵机初次上电时转到的角度
	.angleLimit_Min = 0, 						// 最小角度：舵机可以转到的最小角度 
	.angleLimit_Max = 180, 					// 最大角度：舵机可以转到的最大角度
};
Servo ClawSpinRightServo(&Servo_Spin_R_Init);		// 声明舵机，调用先前的初始化函数


/***
 * 在这里放入xxx.stop()即可使舵机,电机在遥控器急停挡位断电
 */
void UserStop(){
  UserMotor.Stop();
	ChassisCenterServo.stop();
	ChassisDoorServo.stop();
	ClawCenterServo.stop();
	ClawPanningLeftServo.stop();
	ClawPanningRightServo.stop();
	ClawSpinLeftServo.stop();
	ClawSpinRightServo.stop();
}


void SpinTo(SERVOKIND kind, float t) {
	switch (kind) {
		case ChassisCenter: ChassisCenterServo.SetTargetAngle(t); break;
		case ChassisDoor: ChassisDoorServo.SetTargetAngle(t); break;
		case ClawCenter: ClawCenterServo.SetTargetAngle(t); break;
		case ClawPanningLeft: ClawPanningLeftServo.SetTargetAngle(t); break;
		case ClawPanningRight: ClawPanningRightServo.SetTargetAngle(t); break;
		case ClawSpinLeft: ClawSpinLeftServo.SetTargetAngle(t); break;
		case ClawSpinRight: ClawSpinRightServo.SetTargetAngle(t); break;
		default: break;
	}
};

void SpinAdd(SERVOKIND kind, float t) {
	switch (kind) {
			case ChassisCenter: ChassisCenterServo.SetTargetAngle(t + ChassisCenterServo.GetCurrentAngle()); break;
			case ChassisDoor: ChassisDoorServo.SetTargetAngle(t + ChassisDoorServo.GetCurrentAngle()); break;
			case ClawCenter: ClawCenterServo.SetTargetAngle(t + ClawCenterServo.GetCurrentAngle()); break;
			case ClawPanningLeft: ClawPanningLeftServo.SetTargetAngle(t + ClawPanningLeftServo.GetCurrentAngle()); break;
			case ClawPanningRight: ClawPanningRightServo.SetTargetAngle(t + ClawPanningRightServo.GetCurrentAngle()); break;
			case ClawSpinLeft: ClawSpinLeftServo.SetTargetAngle(t + ClawSpinLeftServo.GetCurrentAngle()); break;
			case ClawSpinRight: ClawSpinRightServo.SetTargetAngle(t + ClawSpinRightServo.GetCurrentAngle()); break;
			default: break;
	}
};

/***
 * 在这里写入初始化内容
 */
void UserInit(){
	/* 可以写入开机后一次性任务，例如舵机、电机转到初始角度，或者点击以某规律转动来检查电机状态是否正常 */
	/* 将舵机转到 10° */

}

/***
 * 用户自定义任务主循环
 */
void UserHandle(){
	UserMotor.Handle();
	/* 在这里放入 ServoXXX.Handle() 即可使舵机正常运行 */
	ChassisCenterServo.Handle();
	ChassisDoorServo.Handle();
	ClawCenterServo.Handle();
	ClawPanningLeftServo.Handle();
	ClawPanningRightServo.Handle();
	ClawSpinLeftServo.Handle();
	ClawSpinRightServo.Handle();
}