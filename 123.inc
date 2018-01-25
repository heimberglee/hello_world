

#Declare Memory

  INT    Orig_dPE15_FS9, // variables for volume calculation
  INT    Orig_dPE25_FS9,
  STRING ErStpInfTS_FS9[60],
  INT    AccuVolume_FS9,
  INT    ErOccTS_FS9,
  STRING ErModInfTS_FS9[80],
  STRING ErActTxtTS_FS9[80];

#Declare setvalue
  INT    Activate_teststep_FS9_T,
  INT    Tw0_FS9_T,                
  INT    Tw1_FS9_T,                
  INT    PS1_FS9_T,                
  INT    no_L5_FS9_T,              
  INT    no_loops_circx_FS9_T;              

#Declare Result

  INT    PS1_FS9,              
  INT    Tw0_FS9,                  
  INT    Tw1_FS9,                  
  INT    PA13_FS9,                 
  INT    PA33_FS9,
  INT    PA43_FS9,
  INT    PE15_FS9,                 
  INT    PE25_FS9,                 
  INT    gPE15_FS9,                
  INT    gPE25_FS9,                
  INT    dPE15_FS9,                
  INT    dPE25_FS9,                
  INT    TRun_L5_FS9_H,            
  INT    no_L5_FS9,                
  INT    no_loops_circ1_FS9,                
  INT    no_loops_circ2_FS9,                
  INT    TRun_FS9;                 

#Declare Memory

  INT    TesResTS_FS9,
  INT    SkipCheckResTS_FS9,
  INT    TestCountTS_FS9,
  INT    TesDum_TS_FS9,
  INT    Dum1_TS_FS9,
  INT    Dum2_TS_FS9,
  INT    Err1_TS_FS9,
  INT    Err2_TS_FS9,
  INT    Err3_TS_FS9,
  INT    Err4_TS_FS9,
  INT    Err5_TS_FS9,
  INT    Err6_TS_FS9,
  INT    t3Read_TS_FS9,
  INT    t10SetP1_PuV1_FS9,
  INT    t32SetP1_PuV1_FS9,
  INT    t44MV1B_on_FS9,
  INT    t44MV3B_on_FS9,
  INT    t68Read_TS_FS9,
  INT    cnt_call_FS9;         

#include "../TestSteps/TS_FS9/TLP_FS9.inc";

#Declare Constant INT FFFFh_FS9 = 65535;  
  
#Declare Labeltyp exit_step_FS9;  

#Declare MEMORY 
  INT Vhag1,
  INT Vhag2,
  INT Vsec1,
  INT Vsec2;

// ****************************************************************************************************
Procedure TS_FS9;
  WD_Write_pos (InfoRow+1,1,"FS9-01 start FS9               ",1);
  Step("Test Step FS9: Accumulator volume test, low pressure leakage test to outside and pressure sensor test",1,7500);
  // ****************************************************************************************************
    /**************************************************************************************************
      Remark: The ECU tests run parallel to other test steps.
    **************************************************************************************************/
    TesResTS_FS9=0;
    TesDum_TS_FS9=0;

    Get_Time(Tstart_TRun);

    /** Set 10 Bar in secondary circuit first, then close off secondary circuit. This is to have the accumulator in full stroke when the high pressure is set to
        secondary circuit afterwards */

    Set   ( MV1B );
    Set   ( MV2B );
    Set   ( MV3B );
    Set   ( MV4B );
    Prokist_module ("SET_PWM_MV6", 4000, PSK_Ret, PSK_Error_Str, PSK_Error, 600); // 600 = PWM mit 60%
    Prokist_module ("SET_PWM_MV8", 4000, PSK_Ret, PSK_Error_Str, PSK_Error, 600); // 600 = PWM mit 60%

    Pump1_o=(PS1_FA9_T[0]+PS1_FA9_T[1])/2;

    Parallel;
      BeginTask;
        Wait_Set_value(Pump1_E1,On,PE1,PS1_FA9_T[0],PS1_FA9_T[1],TwPset_Gldf9,5000,Dum1_TS_FA9,Err1_TS_FA9);
      EndTask;
      BeginTask;
        Wait_Set_value(Pump1_E2,On,PE2,PS1_FA9_T[0],PS1_FA9_T[1],TwPset_Gldf9,5000,Dum1_TS_FA9,Err2_TS_FA9);
      EndTask;
      BeginTask;
        Wait_Set_value(Pump1_A1,On,PA1,PS1_FA9_T[0],PS1_FA9_T[1],TwPset_Gldf9,5000,Dum1_TS_FA9,Err3_TS_FA9);
      EndTask;
      BeginTask;
        Wait_Set_value(Pump1_A2,On,PA2,PS1_FA9_T[0],PS1_FA9_T[1],TwPset_Gldf9,5000,Dum1_TS_FA9,Err4_TS_FS9);
      EndTask;
      BeginTask;
        If(A3_used==TRUE);
          Wait_Set_value(Pump1_A3,On,PA3,PS1_FA9_T[0],PS1_FA9_T[1],TwPset_Gldf9,5000,Dum1_TS_FA9,Err5_TS_FS9);
        EndIf; 
      EndTask;
      BeginTask;                 
        If(A4_used==TRUE);
          Wait_Set_value(Pump1_A4,On,PA4,PS1_FA9_T[0],PS1_FA9_T[1],TwPset_Gldf9,5000,Dum1_TS_FA9,Err6_TS_FS9);          
        EndIf; 
      EndTask;
    EndParallel;

    ReSet   ( MV1A );
    ReSet   ( MV2A );
    ReSet   ( MV3A );
    ReSet   ( MV4A );
    ReSet   ( MV1B );
    ReSet   ( MV2B );
    ReSet   ( MV3B );
    ReSet   ( MV4B );
    ReSet   ( MV5 );
    ReSet   ( MV7 );

    Step("First pneumatic test situation: Accumulator full stroke test",2,1500);
      Step("Test preparation",3,1000);
        Step("Set pressure",4,750);
          /**************************************************************************************************
            Define set pressure for pump1:
            PPump11_FS9 = PS1_FS9
          **************************************************************************************************/
          PS1_FS9=(PS1_FS9_T[0]+PS1_FS9_T[1])/2;
          Test               (  TesDum_TS_FS9, 1, PS1_FS9 );
          TesResTS_FS9       = TesResTS_FS9 OR TesDum_TS_FS9;          

          Step("Parallel",5 ,0); EndStep;
            Parallel;
              BeginTask;
                /** Set pressure via pressure ramp */
                Call Press_Ramp1_FS9;
              EndTask;

              BeginTask; //Sys9
                Step("BeginTask",6 ,0); EndStep;
                  /**********************************************************************************************
                    To fix the secondary circuit volume up to the RVR:
                    Switch on MV6 (for hyd_circuit = (TCS_II OR ESP_TCS))
                    Switch on MV8 (for hyd_circuit = ESP_TCS)
                  **********************************************************************************************/
                  If (iNoOfMVs>=10); // TCS_II or ESP_TCS 
                    Prokist_module ("SET_PWM_MV6", 4000, PSK_Ret, PSK_Error_Str, PSK_Error, 600); // 600 = PWM mit 60%
                    If(iNoOfMVs==12); // ESP_TCS
                      Prokist_module ("SET_PWM_MV8", 4000, PSK_Ret, PSK_Error_Str, PSK_Error, 600); // 600 = PWM mit 60%
                    EndIf;
                  EndIf; 
                Step("EndTask",6 ,0); EndStep;
              Endtask;              
              BeginTask;
                Step("Task: Set pressure PS1_FS9_T to E1",6,750);
                  Wait_Set_value(Pump2_E1,On,PE1,PS1_FS9_T[0],PS1_FS9_T[1],TwPset_Gldf9,5000,Dum1_TS_FS9,Err1_TS_FS9);
                EndStep(Err1_TS_FS9);
              EndTask;
              BeginTask;
                Step("Task: Set pressure PS1_FS9_T to E2",6,750);
                  Wait_Set_value(Pump2_E2,On,PE2,PS1_FS9_T[0],PS1_FS9_T[1],TwPset_Gldf9,5000,Dum1_TS_FS9,Err2_TS_FS9);                
                EndStep(Err2_TS_FS9);
              EndTask;
              BeginTask;
                Step("Task: Set pressure PS1_FS9_T to A1",6,750);
                  Wait_Set_value(Pump2_A1,On,PA1,PS1_FS9_T[0],PS1_FS9_T[1],TwPset_Gldf9,5000,Dum1_TS_FS9,Err3_TS_FS9);                
                EndStep(Err3_TS_FS9);
              EndTask;
              BeginTask;
                Step("Task: Set pressure PS1_FS9_T to A2",6,750);
                  Wait_Set_value(Pump2_A2,On,PA2,PS1_FS9_T[0],PS1_FS9_T[1],TwPset_Gldf9,5000,Dum1_TS_FS9,Err4_TS_FS9);                
                EndStep(Err4_TS_FS9);
              EndTask;
              BeginTask;
                Step("Task: Set pressure PS1_FS9_T to A3",6,750);
                  If(A3_used==TRUE);
                    Wait_Set_value(Pump2_A3,On,PA3,PS1_FS9_T[0],PS1_FS9_T[1],TwPset_Gldf9,5000,Dum1_TS_FS9,Err5_TS_FS9);                
                  EndIf;
                EndStep(Err5_TS_FS9);
              EndTask;
              BeginTask;
                Step("Task: Set pressure PS1_FS9_T to A4 (for hyd_circuit <> ABS_3channel)",6,750);
                  If(A4_used==TRUE);
                    Wait_Set_value(Pump2_A4,On,PA4,PS1_FS9_T[0],PS1_FS9_T[1],TwPset_Gldf9,5000,Dum1_TS_FS9,Err6_TS_FS9);                  
                  EndIf; 
                EndStep(Err6_TS_FS9);
              EndTask;
            EndParallel;
          Step("EndParallel",5 ,0); EndStep;

          WD_Write_pos (InfoRow+1,1,"FS9-06 Shutt off all ports               ",1);

          Step("Parallel (shut off connections)",5,450);
            /********************************************************************************************
             Shut off E1
             Shut off E2
             Shut off A1
             Shut off A2
             Shut off A3
             Shut off A4
             ********************************************************************************************/
            Parallel;
              Set (  ShutOFF_E1 );
              Set (  ShutOFF_E2 );
              Set (  ShutOFF_A1 );
              Set (  ShutOFF_A2 );
              Set (  ShutOFF_A3 );
              Set (  ShutOFF_A4 );
            EndParallel;
          Step("EndParallel (shut off connections)",5,450);

          TW0_FS9=TW0_FS9_T;
          Test                ( TesDum_TS_FS9, 1, TW0_FS9 );
          TesResTS_FS9       = TesResTS_FS9 OR TesDum_TS_FS9;  

          Step ("Wait-time TW0_FS9 (settle pressure)", 6, 1);
            Wait_time(TW0_FS9);
          EndStep;
        EndStep;  // Step("Set pressure",4,750);
      EndStep;  // Step("Test preparation",3,1000);

      Step("Accumulator full stroke test (part 1)",3,1000);         
        /********************************************************************************************
          Measure pressure at Ak (initial pressure before accumulator full stroke test):
          PA13_FS9 = PA1
          PA33_FS9 = PA3 A3_used==TRUE
          PA43_FS9 = PA4 A3_used<>TRUE
        *******************************************************************************************/
        PA13_FS9           = PA1;
        IF(A3_used==TRUE);
          PA33_FS9           = PA3;
          Test               (  TesDum_TS_FS9, 1, PA13_FS9, PA33_FS9 );
          TesResTS_FS9       = TesResTS_FS9 OR TesDum_TS_FS9; 
        Else;
          PA43_FS9           = PA4;
          Test               (  TesDum_TS_FS9, 1, PA13_FS9, PA43_FS9 );
          TesResTS_FS9       = TesResTS_FS9 OR TesDum_TS_FS9;
        EndIf;  
        /**********************************************************************************************
          Switch MV1B
          Switch MV3B
          Remark (1):
          () switch MVkB to fill secondary circuit
          () accumulator piston will drive to full stroke position
          () therefore the pressure at Ak will drop
          Remark (2):
          () only one OS-valve (MV1B/3B) is used per circuit, so this is also a functional test of MV1B/3B
          () to save testing time the resulting pressure at Ak will be measured after external-leakage test; time for external-leakage test replaces the pressure-relax-wait-time after MVkB switching
        **********************************************************************************************/
        Set  (  MV1B );
        Set  (  MV2B );
        Set  (  MV3B );
        Set  (  MV4B );
        EndStep;
      EndStep;  // Step("Accumulator full stroke test",3,1000);         
    EndStep;  // Step("First pneumatic test situation: Accumulator full stroke test",2,1500);

    Parallel;
      BeginTask;
        Wait_Set_value(Pump2_E1,On,PE1,PS1_FS9_T[0],PS1_FS9_T[1],TwPset_Gldf9,5000,Dum1_TS_FS9,Err1_TS_FS9);
      EndTask;
      BeginTask;
        Wait_Set_value(Pump2_E2,On,PE2,PS1_FS9_T[0],PS1_FS9_T[1],TwPset_Gldf9,5000,Dum1_TS_FS9,Err2_TS_FS9);                
      EndTask;
      BeginTask;
        Wait_Set_value(Pump2_A1,On,PA1,PS1_FS9_T[0],PS1_FS9_T[1],TwPset_Gldf9,5000,Dum1_TS_FS9,Err3_TS_FS9);                
      EndTask;
      BeginTask;
        Wait_Set_value(Pump2_A2,On,PA2,PS1_FS9_T[0],PS1_FS9_T[1],TwPset_Gldf9,5000,Dum1_TS_FS9,Err4_TS_FS9);                
      EndTask;
      BeginTask;
        If(A3_used==TRUE);
          Wait_Set_value(Pump2_A3,On,PA3,PS1_FS9_T[0],PS1_FS9_T[1],TwPset_Gldf9,5000,Dum1_TS_FS9,Err5_TS_FS9);                        
        EndIf;     
      EndTask;
      BeginTask;
        If(A4_used==TRUE); 
          Wait_Set_value(Pump2_A4,On,PA4,PS1_FS9_T[0],PS1_FS9_T[1],TwPset_Gldf9,5000,Dum1_TS_FS9,Err6_TS_FS9);                  
        EndIf; 
      EndTask;
    EndParallel;        
    
    Parallel;
      Set (  ShutOFF_E1 );
      Set (  ShutOFF_E2 );
      Set (  ShutOFF_A1 );
      Set (  ShutOFF_A2 );
      Set (  ShutOFF_A3 );
      Set (  ShutOFF_A4 );
    EndParallel;

    WD_Write_pos (InfoRow+1,1,"FS9-07 Start meas. total leakage              ",1);
    Step("Second pneumatic test situation: Fine leakage to outside",2,2000);
      Step("Test preparation (settle pressure)",3,500);
        /**********************************************************************************************
          Call routine "TS_FD9SwitchMVk":
          () wait until pressure gradient goes below 1bar/s simultaneously at E1, E2 (Note: Bigger accumulator volumes need longer settling times).
        **********************************************************************************************/
        Call TL_WaitPset4_FS9;
        Wait_time(5000);
      EndStep;  // Step("Test preparation (settle pressure)",3,500);

      Step("Fine leakage test to outside",3,500);
        /**********************************************************************************************
          Measure pressure at Ek (initial pressure before fine leakage test):
          PE15_FS9 = PE1
          PE25_FS9 = PE2
        **********************************************************************************************/
        PE15_FS9           = PE1;
        Test               (  TesDum_TS_FS9, 1, PE15_FS9 );
        TesResTS_FS9       = TesResTS_FS9 OR TesDum_TS_FS9;

        PE25_FS9           = PE2;
        Test               (  TesDum_TS_FS9, 1, PE25_FS9 );
        TesResTS_FS9       = TesResTS_FS9 OR TesDum_TS_FS9;
        EndStep;
        // switch off MVs
        ReSet  (  MV1B );
        ReSet  (  MV2B );
        ReSet  (  MV3B );
        ReSet  (  MV4B );
        ReSet  (  MV6  );
        ReSet  (  MV8  );

        TW1_FS9=TW1_FS9_T;
        Test                ( TesDum_TS_FS9, 1, TW1_FS9 );
        TesResTS_FS9       = TesResTS_FS9 OR TesDum_TS_FS9;  

        Step ("Wait-time TW1_FS9 (settle pressure)", 6, 1);          
          Wait_time(TW1_FS9-2000);
        EndStep;
        // switch on MVs
        Set  (  MV1B );
        Set  (  MV2B );
        Set  (  MV3B );
        Set  (  MV4B );
        Prokist_module ("SET_PWM_MV6", 4000, PSK_Ret, PSK_Error_Str, PSK_Error, 600); // 600 = PWM mit 60%
        Prokist_module ("SET_PWM_MV8", 4000, PSK_Ret, PSK_Error_Str, PSK_Error, 600); // 600 = PWM mit 60%
        Wait_time(2000);
        dPE15_FS9          = PE1 - PE15_FS9;
        // Volume calculation
        Orig_dPE15_FA9     = dPE15_FA9;
        dPE15_FA9          = dPE15_FA9 * RFac_LeakOutside1;
        
        dPE25_FS9          = PE2 - PE25_FS9;
        // Volume calculation
        Orig_dPE25_FA9     = dPE25_FA9;
        dPE25_FA9          = dPE25_FA9 * RFac_LeakOutside2;

        WD_Write_pos (InfoRow+1,1,"FS9-10 call ECU test init                 ",1);
        Step("Parallel",5,0); EndStep;
          Step("Task: Determine external leakage",6,2000);
            /******************************************************************************************
              Call routine TL_ExtLeakFA8:
              () fine leakage to outside will be detected with a pressure gradient measurement
             ******************************************************************************************/
            // Task is carried out by procedure: 
            Call TL_ExtLeakFS9;
          EndStep;  // Step("Task: Determine external leakage",6,2000);
        Step("EndParallel",5,0); EndStep;
 
        Step("Evaluation fine leakage test to outside (TL_ExtLeakFS9)",4,1);
          Test               (  TesDum_TS_FS9, 1, gPE15_FS9, gPE25_FS9 );
          TesResTS_FS9       = TesResTS_FS9 OR TesDum_TS_FS9;  

          /**********************************************************************************************
            Following values were measured by presiding procedure TL_ExtLeakFA8:
            () fine leakage pressure gradient at Ek:
            gPE15_FS9
            gPE25_FS9
            () overall pressure drop at Ek while leakage measurement (rough leakage test):
            dPE15_FS9 = PE1 - PE15_FS9
            dPE25_FS9 = PE2 - PE25_FS9
            () run time for fine leakage test routine:
            TRun_L5_FS9_H
            () number of required loops for fine leakage test:
            no_L5_FS9
            () number of required loops per cicuit for fine leakage test:
            no_loops_circ1_FS9
            no_loops_circ2_FS9
            Remark:
            too large values of dPEk5_FS9 are possibly caused by: 
            () coarse external leakage, or 
            () leakage at the accumulator o-ring towards the enclosed area sealed by the staked accumulator covers
          **********************************************************************************************/

          Test               (  TesDum_TS_FS9, 1, dPE15_FS9, dPE25_FS9 );
          TesResTS_FS9       = TesResTS_FS9 OR TesDum_TS_FS9;    
          Test               (  TesDum_TS_FS9, 1, TRun_L5_FS9_H );
          TesResTS_FS9       = TesResTS_FS9 OR TesDum_TS_FS9;
          Test               (  TesDum_TS_FS9, 1, no_L5_FS9 );
          TesResTS_FS9       = TesResTS_FS9 OR TesDum_TS_FS9;
          Test               (  TesDum_TS_FS9, 1, no_loops_circ1_FS9 );
          TesResTS_FS9       = TesResTS_FS9 OR TesDum_TS_FS9;
          Test               (  TesDum_TS_FS9, 1, no_loops_circ2_FS9 );
          TesResTS_FS9       = TesResTS_FS9 OR TesDum_TS_FS9;
        EndStep;  // Step("Evaluation fine leakage test to outside (TL_ExtLeakFA8)",4,1);
      EndStep;  // Step("Fine leakage test to outside",3,500);
    EndStep;  // Step("Second pneumatic test situation: Fine leakage to outside",2,2000);

    // Step("Relief all connections parallel",3,100);
      Parallel;
        BeginTask;
          Call Press_Ramp2_FS9;
        EndTask;
        BeginTask;
          // Step("Relief A1",6,100);
            Wait_Set_value(Pump2_A1,On,PA1,(Pzero_Gldf9_T[0]-500),(Pzero_Gldf9_T[1]+500),TwPset_Gldf9,Tout1_Gldf9,Dum1_TS_FS9,Err1_TS_FS9);
          // EndStep;
        EndTask;
        BeginTask;
          // Step("Relief A2",6,100);
            Wait_Set_value(Pump2_A2,On,PA2,(Pzero_Gldf9_T[0]-500),(Pzero_Gldf9_T[1]+500),TwPset_Gldf9,Tout1_Gldf9,Dum1_TS_FS9,Err1_TS_FS9);
          // EndStep;
        EndTask;
        BeginTask;
          // Step("Relief A3",6,100);
            Wait_Set_value(Pump2_A3,On,PA3,(Pzero_Gldf9_T[0]-500),(Pzero_Gldf9_T[1]+500),TwPset_Gldf9,Tout1_Gldf9,Dum1_TS_FS9,Err1_TS_FS9);
          // EndStep;
        EndTask;
        BeginTask;
          // Step("Relief A4",5,100);
            Wait_Set_value(Pump2_A4,On,PA4,(Pzero_Gldf9_T[0]-500),(Pzero_Gldf9_T[1]+500),TwPset_Gldf9,Tout1_Gldf9,Dum1_TS_FS9,Err1_TS_FS9);
          // EndStep;
        EndTask;
        BeginTask;
          // Step("Relief E1",5,100);
            Wait_Set_value(Pump2_E1,On,PE1,(Pzero_Gldf9_T[0]-500),(Pzero_Gldf9_T[1]+500),TwPset_Gldf9,Tout1_Gldf9,Dum1_TS_FS9,Err1_TS_FS9);    
          // EndStep;
        EndTask;
        BeginTask;
          // Step("Relief E2",5,100);
            Wait_Set_value(Pump2_E2,On,PE2,(Pzero_Gldf9_T[0]-500),(Pzero_Gldf9_T[1]+500),TwPset_Gldf9,Tout1_Gldf9,Dum1_TS_FS9,Err1_TS_FS9);
          // EndStep;
        EndTask;
      EndParallel;
      // EndStep;//Relief all connections parallel
      set(Relief_A1,Relief_A2,Relief_A3,Relief_A4,Relief_E1,Relief_E2);
      Wait_time(2000);

      // switch off MVs
      ReSet  (  MV1B );
      ReSet  (  MV2B );
      ReSet  (  MV3B );
      ReSet  (  MV4B );
      ReSet  (  MV6  );
      ReSet  (  MV8  );
    LABEL exit_step_FS9;
    // All actions done!
    Step("End of TS_FS9",2,0); 
      /**********************************************************************************************
        Determine run time of TS_FS9:
        TRun_FS9
      **********************************************************************************************/
      Elapse_time        (  tstart_TRun, TRun_FS9 );
      Test               (  TesDum_TS_FS9, 1, TRun_FS9 );
      TesResTS_FS9       = TesResTS_FS9 OR TesDum_TS_FS9;
    EndStep;  // Step("End of TS_FS9",2,0);

  EndStep;  // Step("Test Step FS9: Accumulator volume test, low pressure leakage test to outside and pressure sensor test",1,7500);
EndProcedure; // Procedure TS_FS9;

// ****************************************************************************************************
Procedure SKIP_CHECK_FS9;
#declare memory string step_name;

  step_name="TS_FS9";
  Test               (  SkipCheckResTS_FS9, SMART_SKIP, step_name , TestCountTS_FS9 );  
EndProcedure; // Procedure SKIP_CHECK_FS9;

//  *** end of Module ***
