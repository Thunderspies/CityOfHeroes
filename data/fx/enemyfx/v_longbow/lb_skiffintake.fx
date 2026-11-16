#########################################################
##	Fireweapon
########################################################
FxInfo

#####################################################################################
#################################  Right 01 Intake  #################################
#####################################################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	InEmitR1
		Type	local
		At	Chest

		bhvr	ENEMYFX\V_LongBow\LB_B_SkiffIntakeR1.bhvr
#		Geom	Testing_TargetB
	End

End

Condition
	On	Time
	Time	1

	Event
		EName	IntakeR1
		Type	Local
		At	InEmitR1

		Part	ENEMYFX\V_LongBow\LB_P_SkiffIntake.part
	End
End

#####################################################################################
#################################  Left 01 Intake  ##################################
#####################################################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	InEmitL1
		Type	local
		At	Chest
		BhvrOverride
			behavior
			PositionOffset		2.2 1.65 -.1
		End
		bhvr	ENEMYFX\V_LongBow\LB_B_SkiffIntakeR1.bhvr
#		Geom	Testing_TargetB
	End

End

Condition
	On	Time
	Time	1

	Event
		EName	IntakeL1
		Type	Local
		At	InEmitL1

		Part	ENEMYFX\V_LongBow\LB_P_SkiffIntake.part
	End
End

#####################################################################################
#################################  Right 02 Intake  #################################
#####################################################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	InEmitR2
		Type	local
		At	Chest

		bhvr	ENEMYFX\V_LongBow\LB_B_SkiffIntakeR2.bhvr
		#Geom	Testing_TargetB
	End

End

Condition
	On	Time
	Time	1

	Event
		EName	IntakeR2
		Type	Local
		At	InEmitR2

		Part	ENEMYFX\V_LongBow\LB_P_SkiffIntake02.part
	End
End

#####################################################################################
#################################  Left 02 Intake  ##################################
#####################################################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	InEmitL2
		Type	local
		At	Chest
		bhvrOverride
			behavior
			PositionOffset		4.794 2.2 -.5
		End

		bhvr	ENEMYFX\V_LongBow\LB_B_SkiffIntakeR2.bhvr
#		Geom	Testing_TargetB
	End

End

Condition
	On	Time
	Time	1

	Event
		EName	IntakeL2
		Type	Local
		At	InEmitL2

		Part	ENEMYFX\V_LongBow\LB_P_SkiffIntake02.part
		PMagnet	InEmitL2
	End
End

End