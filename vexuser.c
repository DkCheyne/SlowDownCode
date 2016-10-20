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

// Counts how many loops we have been recording IME values after the last turn
// needs to be over 2 to be able to compair
int straightLine = 0;

// The most current IME value for topRight motor (top motors were choosen because they show the largest change in rotation at
// different speeds when "trying" to drive straight)

int newRight = 0;

// The most current IME value for topLeft Motor 
int newLeft = 0;

// This is taken the loop before we start smart driving
int oldRight = 0;

// This is taken the loop before we start smart driving
int oldLeft = 0;

// These are the differences in IME values between any two loops without a turn in the middle
int differenceRight = 0;
int differenceLeft = 0;
int totalDifference = 0;



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

    #define bottomRight kVexMotor_2
    #define bottomLeft kVexMotor_3
    #define topLeft kVexMotor_4
    #define topRight kVexMotor_5

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
/** @brief      Base control                                                   */
/*-----------------------------------------------------------------------------*/
/** @details
 *  This functions controls the movement of the base
 */
void vexBaseControl(int forward, int turn)
{
  
    // The controller is indicating a turn is wanted
    if (abs(turn) > 20)
    {

        // This shows that we don't have enough data to compair IME value on motors. I.E not a straight line
        striatLine = 0;
     
        // Just get the Motors to turn
        vexMotorSet(bottomRight, turn);
        vexMotorSet(topRight, turn);
        vexMotorSet(bottomLeft, -turn);
        vexMotorSet(topLeft,  -turn);
    }
 
    // The controller is indicating we don't want a turn
    if (abs(turn) < 20)
    {
       
       // We don't enough IME data to compair values of straitness
       if (straightLine < 2)
       {
          // We're practically blind here so just make all the motors the same
          vexMotorSet(bottomRight, forward);
          vexMotorSet(topRight, forward);
          vexMotorSet(bottomLeft, forward);
          vexMotorSet(topLeft,  forward);

          // Record IME values right now to compair next loop if we don't try and turn
          oldRight = vexMotorPositionGet(topRight);
          oldLeft = vexMotorPositionGet(topLeft);

          // We just recorded IME values so we can start compairing next loop around
          straightLine = straightLine + 1;

       }

       // We have enough IME data to start doing some compairisons
       else if (straightLine > 1)
       {

        // These are the postions at which the motors are right at this moment
        newRight = vexMotorPositionGet(topRight);
        newLeft = vexMotorPositionGet(topLeft);

        // These are the differences between what the motors position was in the last two loops
        differenceRight = newRight - oldRight;
        differenceLeft = newLeft - oldLeft;

        // Compaing the who moved at a faster rate than the other in the last two loops
        totalDifference = differenceRight - differenceLeft;

        switch (totalDifference)
        {

            // This case is that the right side moved a larger distance than the left did
            case > 0:

                    // So we need to slow down the right side compaired to the left side
                    // I need to experiment with these values in order to get the best results
                    vexMotorSet(bottomRight, forward - 1);
                    vexMotorSet(topRight, forward - 1);
                    vexMotorSet(bottomLeft, forward);
                    vexMotorSet(topLeft,  forward);

            // This case is that the left side movde a larger distance than the right did
            case < 0:

                    // So we need to slow down the left side compaired ot the right side
                    // Again going to need to experiment with this
                    vexMotorSet(bottomRight, forward);
                    vexMotorSet(topRight, forward);
                    vexMotorSet(bottomLeft, forward - 1);
                    vexMotorSet(topLeft,  forward -1 );

            // Edge case, but I just want to make sure its here better safe than sorry
            case 0:

                    vexMotorSet(bottomRight, forward);
                    vexMotorSet(topRight, forward);
                    vexMotorSet(bottomLeft, forward);
                    vexMotorSet(topLeft,  forward);
        } 

       }





    }




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
    vexMotorPositionSet(topRight, 0);




    // Run until asked to terminate
    while(!chThdShouldTerminate())
       
       

        // This is here so that we can move slowly to move more accuratly 
        if (vexControllerGet(Btn5D) == 1)
        {
            vexBaseControl(.2 * vexControllerGet(Ch3), .2 * vexControllerGet(Ch1));
        }

        else 
        {
             vexBaseControl(vexControllerGet(Ch3), vexControllerGet(Ch1));
        }


        // Don't hog cpu
        vexSleep( 25 );
        

    return (msg_t)0;
}

