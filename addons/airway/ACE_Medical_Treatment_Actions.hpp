class ACEGVAR(medical_treatment,actions) {
    class CheckPulse;
    class CheckAirway: CheckPulse {
        displayName = "Check Airway";
        displayNameProgress = "Checking Airway...";
        icon = "";
        category = "airway";
        treatmentLocations = TREATMENT_LOCATIONS_ALL;
        medicRequired = 0;
        treatmentTime = 2.5;
        allowedSelections[] = {"Head"};
        allowSelfTreatment = 0;
        condition = QUOTE(!(_patient call ACEFUNC(common,isAwake)));
        callbackSuccess = QFUNC(checkAirway);
        ACM_rollToBack = 1;
    };

    class HeadTurn: CheckAirway {
        displayName = "Perform Head Turning";
        displayNameProgress = "Head Turning...";
        icon = "";
        medicRequired = 0;
        treatmentTime = 5;
        condition = QUOTE(!(_patient call ACEFUNC(common,isAwake)) && (_patient getVariable [ARR_2(QQGVAR(AirwayItem),'')] == ''));
        callbackSuccess = QFUNC(performHeadTurn);
        ACM_cancelRecovery = 1;
    };

    class HeadTiltChinLift: CheckAirway {
        displayName = "Perform Head Tilt-Chin Lift";
        displayNameProgress = "Tilting Head...";
        icon = "";
        medicRequired = 0;
        treatmentTime = 4;
        condition = QUOTE(!(_patient call ACEFUNC(common,isAwake)) && !(_patient getVariable [ARR_2(QQGVAR(HeadTilt_State),false)]) && (_patient getVariable [ARR_2(QQGVAR(AirwayItem),'')] == ''));
        callbackSuccess = QUOTE([ARR_3(_medic,_patient,true)] call FUNC(setHeadTiltChinLift));
        ACM_cancelRecovery = 1;
    };
    class CancelHeadTiltChinLift: HeadTiltChinLift {
        displayName = "Cancel Head Tilt-Chin Lift";
        displayNameProgress = "";
        icon = "";
        medicRequired = 0;
        treatmentTime = 0.01;
        condition = QUOTE(_patient getVariable [ARR_2(QQGVAR(HeadTilt_State),false)] && !(IN_RECOVERYPOSITION(_patient)));
        callbackSuccess = QUOTE([ARR_3(_medic,_patient,false)] call FUNC(setHeadTiltChinLift));
    };

    class RecoveryPosition: CheckAirway {
        displayName = "Establish Recovery Position";
        displayNameProgress = "Establishing Recovery Position...";
        icon = "";
        medicRequired = 0;
        treatmentTime = QGVAR(treatmentTimeRecoveryPosition);
        allowedSelections[] = {"Body"};
        condition = QUOTE(!(_patient call ACEFUNC(common,isAwake)) && (_patient getVariable [ARR_2(QQGVAR(AirwayItem),'')] == '') && !(IN_RECOVERYPOSITION(_patient)));
        callbackSuccess = QUOTE([ARR_3(_medic,_patient,true)] call FUNC(setRecoveryPosition));
        ACM_rollToBack = 0;
    };
    class CancelRecoveryPosition: RecoveryPosition {
        displayName = "Cancel Recovery Position";
        displayNameProgress = "Cancelling Recovery Position...";
        icon = "";
        medicRequired = 0;
        treatmentTime = 1;
        condition = QUOTE(!(_patient call ACEFUNC(common,isAwake)) && IN_RECOVERYPOSITION(_patient));
        callbackSuccess = QUOTE([ARR_3(_medic,_patient,false)] call FUNC(setRecoveryPosition));
        ACM_rollToBack = 1;
        ACM_cancelRecovery = 1;
    };

    class UseSuctionBag: CheckAirway {
        displayName = "Use Suction Bag";
        displayNameProgress = "Using Suction Bag...";
        icon = "";
        medicRequired = QGVAR(allowSuctionBag);
        treatmentTime = QUOTE([_patient] call FUNC(getSuctionTime));
        items[] = {"ACM_SuctionBag"};
        consumeItem = 1;
        condition = QUOTE(!(_patient call ACEFUNC(common,isAwake)));
        callbackSuccess = QUOTE([ARR_3(_medic,_patient,0)] call FUNC(handleSuction));
        ACM_cancelRecovery = 1;
    };
    class UseAccuvac: UseSuctionBag {
        displayName = "Use ACCUVAC";
        displayNameProgress = "Using ACCUVAC...";
        icon = "";
        medicRequired = QGVAR(allowACCUVAC);
        treatmentTime = QUOTE([_patient] call FUNC(getSuctionTime));
        items[] = {"ACM_ACCUVAC"};
        consumeItem = 0;
        callbackSuccess = QUOTE([ARR_3(_medic,_patient,1)] call FUNC(handleSuction));
    };

    class InsertGuedelTube: CheckAirway {
        displayName = "Insert Guedel Tube";
        displayNameProgress = "Inserting Guedel Tube...";
        icon = "";
        medicRequired = QGVAR(allowOPA);
        treatmentTime = QGVAR(treatmentTimeOPA);
        items[] = {"ACM_GuedelTube","ACM_GuedelTube_Used"};
        consumeItem = 1;
        condition = QUOTE(!(_patient call ACEFUNC(common,isAwake)) && (_patient getVariable [ARR_2(QQGVAR(AirwayItem),'')] == ''));
        callbackSuccess = QUOTE([ARR_3(_medic,_patient,'OPA')] call FUNC(insertAirwayItem));
        ACM_cancelRecovery = 1;
    };
    class InsertIGel: InsertGuedelTube {
        displayName = "Insert i-gel";
        displayNameProgress = "Inserting i-gel...";
        icon = "";
        medicRequired = QGVAR(allowSGA);
        treatmentTime = QGVAR(treatmentTimeSGA);
        items[] = {"ACM_IGel","ACM_IGel_Used"};
        callbackSuccess = QUOTE([ARR_3(_medic,_patient,'SGA')] call FUNC(insertAirwayItem));
    };

    class RemoveGuedelTube: CheckAirway {
        displayName = "Remove Guedel Tube";
        displayNameProgress = "Removing Guedel Tube...";
        icon = "";
        medicRequired = 0;
        treatmentTime = 2;
        condition = QUOTE(!(_patient call ACEFUNC(common,isAwake)) && (_patient getVariable [ARR_2(QQGVAR(AirwayItem),'')] == 'OPA'));
        callbackSuccess = QFUNC(removeAirwayItem);
    };
    class RemoveIGel: RemoveGuedelTube {
        displayName = "Remove i-gel";
        displayNameProgress = "Removing i-gel...";
        icon = "";
        medicRequired = 0;
        condition = QUOTE(!(_patient call ACEFUNC(common,isAwake)) && (_patient getVariable [ARR_2(QQGVAR(AirwayItem),'')] == 'SGA'));
    };
};
