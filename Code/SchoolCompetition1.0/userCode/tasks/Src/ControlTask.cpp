//
// Created by LEGION on 2021/10/4.
//
#include "ControlTask.h"


void CtrlHandle(){
    if (RemoteControl::rcInfo.sRight == DOWN_POS){//右侧三档，急停模式
        ChassisStop();
        UserStop();
    }else {//其他正常模式
        switch (RemoteControl::rcInfo.sLeft) {
            case UP_POS://左侧一档
								/* 右摇杆前后拨动控制灵敏性，右摇杆左右拨动控制灵敏性，左摇杆左右拨动控制灵敏性 */
								/* * 后的是一个系数，可以调节到最舒适的手感 */
                ChassisSetVelocity(RemoteControl::rcInfo.right_col*4.2,
                                   RemoteControl::rcInfo.right_rol*4.2,RemoteControl::rcInfo.left_rol*60);
                break;
            case MID_POS://左侧二档
                uint8_t clawState;
                if (RemoteControl::rcInfo.sRight == UP_POS){
                    clawState = 0;
                }else if(RemoteControl::rcInfo.sRight == MID_POS) {
                    clawState = 1;
                }
								break;
						case DOWN_POS:default: //左侧三挡
								break;
        }

    }

}