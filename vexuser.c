/*-----------------------------------------------------------------------------*/
/*                                                                             */
/*                        Copyright (c) Derek Cheyne                           */
/*                                   2016                                      */
/*                            All Rights Reserved                              */
/*                                                                             */
/*-----------------------------------------------------------------------------*/
/*                                                                             */
/*    Module:     vexuser.c                                                    */
/*    Author:     Derek Cheyne                                                 */
/*    Created:    11 September 2016                                            */
/*                                                                             */                      
/*                                                                             */
/*-----------------------------------------------------------------------------*/
/*                                                                             */
/*    The author is supplying this software for use with the VEX cortex        */
/*    control system. This file can be freely distributed and teams are        */
/*    authorized to freely use this program , however, it is requested that    */
/*    improvements or additions be shared with the Vex community via the vex   */
/*    forum.  Please acknowledge the work of the authors when appropriate.     */
/*    Thanks.                                                                  */
/*                                                                             */
/*    Licensed under the Apache License, Version 2.0 (the "License");          */
/*    you may not use this file except in compliance with the License.         */
/*    You may obtain a copy of the License at                                  */
/*                                                                             */
/*      http://www.apache.org/licenses/LICENSE-2.0                             */
/*                                                                             */
/*    Unless required by applicable law or agreed to in writing, software      */
/*    distributed under the License is distributed on an "AS IS" BASIS,        */
/*    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. */
/*    See the License for the specific language governing permissions and      */
/*    limitations under the License.                                           */
/*                                                                             */
/*    The author can be contacted on the vex forums as jpearman                */
/*    or electronic mail using jbpearman_at_mac_dot_com                        */
/*    Mentor for team 8888 RoboLancers, Pasadena CA.                           */
/*                                                                             */
/*-----------------------------------------------------------------------------*/

#include <stdlib.h>

#include "ch.h"         // needs for all ChibiOS programs
#include "hal.h"        // hardware abstraction layer header
#include "vex.h"        // vex library header

// Digi IO configuration
static  vexDigiCfg  dConfig[kVexDigital_Num] = {
        { kVexDigital_1,    kVexSensorDigitalOutput, kVexConfigOutput,      0 },
        { kVexDigital_2,    kVexSensorDigitalOutput, kVexConfigOutput,      0 },
        { kVexDigital_3,    kVexSensorDigitalInput,  kVexConfigInput,       0 },
        { kVexDigital_4,    kVexSensorDigitalInput,  kVexConfigInput,       0 },
        { kVexDigital_5,    kVexSensorDigitalInput,  kVexConfigInput,       0 },
        { kVexDigital_6,    kVexSensorDigitalInput,  kVexConfigInput,       0 },
        { kVexDigital_7,    kVexSensorDigitalInput,  kVexConfigInput,       0 },
        { kVexDigital_8,    kVexSensorDigitalInput,  kVexConfigInput,       0 },
        { kVexDigital_9,    kVexSensorDigitalInput,  kVexConfigInput,       0 },
        { kVexDigital_10,   kVexSensorDigitalInput,  kVexConfigInput,       0 },
        { kVexDigital_11,   kVexSensorDigitalInput,  kVexConfigInput,       0 },
        { kVexDigital_12,   kVexSensorDigitalInput,  kVexConfigInput,       0 }
};

static  vexMotorCfg mConfig[kVexMotorNum] = {
        { kVexMotor_1,      kVexMotorUndefined,      kVexMotorNormal,       kVexSensorIME,        kImeChannel_1},
        { kVexMotor_2,      kVexMotorUndefined,      kVexMotorNormal,       kVexSensorIME,        kImeChannel_2},
        { kVexMotor_3,      kVexMotorUndefined,      kVexMotorNormal,       kVexSensorIME,        kImeChannel_3},
        { kVexMotor_4,      kVexMotorUndefined,      kVexMotorNormal,       kVexSensorNone,        0 },
        { kVexMotor_5,      kVexMotorUndefined,      kVexMotorNormal,       kVexSensorNone,        0 },
        { kVexMotor_6,      kVexMotorUndefined,      kVexMotorNormal,       kVexSensorNone,        0 },
        { kVexMotor_7,      kVexMotorUndefined,      kVexMotorNormal,       kVexSensorNone,        0 },
        { kVexMotor_8,      kVexMotorUndefined,      kVexMotorNormal,       kVexSensorNone,        0 },
        { kVexMotor_9,      kVexMotorUndefined,      kVexMotorNormal,       kVexSensorNone,        0 },
        { kVexMotor_10,     kVexMotorUndefined,      kVexMotorNormal,       kVexSensorNone,        0 }
};


/*-----------------------------------------------------------------------------*/
/** Global Varibles here
*/
int speedArray[2] = {0, 0};


/*-----------------------------------------------------------------------------*/
/** @brief      User setup                                                     */
/*-----------------------------------------------------------------------------*/
/** @details
 *  The digital and motor ports can (should) be configured here.
 */
void
vexUserSetup()
{
    vexDigitalConfigure( dConfig, DIG_CONFIG_SIZE( dConfig ) );
    vexMotorConfigure( mConfig, MOT_CONFIG_SIZE( mConfig ) );

    #define bottomRight kVexMotor_1
    #define bottomLeft kVexMotor_2
    #define topLeft kVexMotor_3
    #define topRight kVexMotor_4

}

/*-----------------------------------------------------------------------------*/
/** @brief      User initialize                                                */
/*-----------------------------------------------------------------------------*/
/** @details
 *  This function is called after all setup is complete and communication has
 *  been established with the master processor.
 *  Start other tasks and initialize user variables here
 */
void
vexUserInit()
{


}

/*-----------------------------------------------------------------------------*/
/** @brief      Autonomous                                                     */
/*-----------------------------------------------------------------------------*/
/** @details
 *  This thread is started when the autonomous period is started
 */
msg_t
vexAutonomous( void *arg )
{
    (void)arg;

    // Must call this
    vexTaskRegister("auton");

    while(1)
        {
        
      

        // Don't hog cpu
        // Must be at end of auton
        vexSleep( 25 );
        }

    return (msg_t)0;
}



/*-----------------------------------------------------------------------------*/
/** @brief      Base Control                                                   */
/*-----------------------------------------------------------------------------*/
/** @details
 *  This function determines how the base moves
 */
 void vexBaseControl(int valueForward, int valueTurn, int Turbo)
{
    int unadjustedTurn = 0;

    // 0 means that the turbo button isn't pressed (btn5D)
    if(Turbo == 0)
    {
        // Execute the code for userControl


        //Going Forward Jerk Control
        // asb(vexControllerGet) is there so that if we are trying to turn it doesn't fight itself.
        if (abs((valueForward - speedArray[0])) > 2  && abs(vexControllerGet(Ch3)) < 40)
        {
            
           // Often one motor is slightly stronger than the other so we have to correct for it
		   // This block is for moving Forward
           if ((vexMotorPositionGet(bottomLeft) - vexMotorPositionGet(bottomRight) > 1))
           {
             vexMotorSet(bottomRight, speedArray[0] + 1);
             vexMotorSet(bottomLeft, speedArray[0] + 2);
             valueForward = speedArray[0];
             valueForward = valueForward + 2;
             speedArray[0] = valueForward;
           }

           if ((vexMotorPositionGet(bottomLeft) - vexMotorPositionGet(bottomRight) < 1)) 
           {
            vexMotorSet(bottomRight, speedArray[0] + 1);
            vexMotorSet(bottomLeft, speedArray[0] + 2);
            valueForward = speedArray[0];
            valueForward = valueForward + 2;
            speedArray[0] = valueForward;
            }
        
        }
        
        else if(abs(valueForward - speedArray[0]) <= 2 )
        {
            vexMotorSet(bottomRight, valueForward);
            vexMotorSet(bottomLeft, valueForward);
            vexMotorSet(topRight, valueForward);
            speedArray[0] = valueForward;
            valueForward = valueForward + 2;
            speedArray[0] = valueForward;
        }
        
        // Turning Jerk Control
		// This block is for turning
        if ((valueTurn - speedArray[1]) > 2)
        {
            vexMotorSet(bottomRight, (speedArray[1] + 2) );
            vexMotorSet(bottomLeft, (speedArray[1] + 2) );
            vexMotorSet(topRight, (speedArray[1] + 2) );
            vexMotorSet(topLeft, (speedArray[1] + 2) );
            valueTurn = speedArray[1];
            valueTurn = valueTurn + 2;
            speedArray[1] = valueTurn;
        }
        else if( (valueTurn - speedArray[1] < 2) )
        {
            vexMotorSet(bottomRight, (valueTurn) );
            vexMotorSet(bottomLeft, valueTurn);
            vexMotorSet(topRight, (valueTurn) );
            vexMotorSet(topLeft, valueTurn);

        }

        
    }  
   // This is here so that drivers can bypass the jerk control
    else 
    {
            vexMotorSet(bottomRight, (valueTurn) );
            vexMotorSet(bottomLeft, valueTurn);
            vexMotorSet(topRight, (valueTurn) );
            vexMotorSet(topLeft, valueTurn);
    }
}








/*-----------------------------------------------------------------------------*/
/** @brief      Driver control                                                 */
/*-----------------------------------------------------------------------------*/
/** @details
 *  This thread is started when the driver control period is started
 */
msg_t
vexOperator( void *arg )
{
    int16_t     blink = 0;

    (void)arg;

    // Must call this
    vexTaskRegister("operator");

    // Nneed to reset encoders right before all the code is run just so that we have a base foreverything to work
    vexMotorPositionSet(bottomRight, 0);
    vexMotorPositionSet(bottomLeft, 0);
    vexMotorPositionSet(topLeft, 0);




    // Run until asked to terminate
    while(!chThdShouldTerminate())
        {
            if(vexControllerGet(Btn7L) == 0)
            {
                 // Button so that we can move slowly for accurate control.
                if(vexControllerGet(Btn5U) == 1)
                {
                    vexBaseControl(.2 * vexControllerGet(Ch3), .2 * vexControllerGet(Ch1), vexControllerGet(Btn5D) );
                }

                else 
                {// the 1 signifies that this call came from the user portion of movement
                    vexBaseControl(vexControllerGet(Ch3), vexControllerGet(Ch1), vexControllerGet(Btn5D));
                }

            }
           


        // Don't hog cpu
        vexSleep( 25 );
        }

    return (msg_t)0;
}
