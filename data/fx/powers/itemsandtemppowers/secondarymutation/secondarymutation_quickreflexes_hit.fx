#############################################################
## SecondaryMutation_QuickReflexes_Hit.fx
#############################################################

FxInfo
Lifespan 100

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type Local
		At Origin
		Sound QuickReflexes_Hit_01 100 100 .7
	End

	Event
		EName 	BigSpinnerL
		Type	Local
		At	LArmL
		Bhvr	:ArmSpiral.bhvr
		BhvrOverride
			PositionOffset		1.0 0.0 0.0
			Spin			-0.1 0.0 0.0
		End
		Lifespan 30
	End

	Event
		EName 	Offset_LArmL_Spiral
		Type	Local
		At	BigSpinnerL
		Bhvr	:ArmSpiral.bhvr
		BhvrOverride
			Spin			0.8 0.0 0.0
		End
		Lifespan 30
	End

	Event
		Type	Local
		At	Offset_LArmL_Spiral
		BhvrOverride
			PositionOffset		0 0 -0.2
		End
		part	:DNA_Proteins_Arm.part
		Lifespan 12
	End

	Event
		Type	Local
		At	Offset_LArmL_Spiral
		BhvrOverride
			PositionOffset		0 0 0.2
		End
		part	:DNA_Proteins_Arm.part
		Lifespan 12
	End

	Event
		Type	Local
		At	UArmL
		part	:Leg_Glow.part
		POther	LArmL
		Lifespan 20
	End
End


#############################################################

Condition
	On	Time
	Time	3

	Event
		EName 	BigSpinnerR
		Type	Local
		At	LArmR
		Bhvr	:ArmSpiral2.bhvr
		BhvrOverride
			PositionOffset		-1.0 0.0 0.0
			Spin			0.1 0.0 0.0
		End
		Lifespan 30
	End

	Event
		EName 	Offset_LArmR_Spiral
		Type	Local
		At	BigSpinnerR
		Bhvr	:ArmSpiral2.bhvr
		BhvrOverride
			Spin			0.8 0.0 0.0
		End
		Lifespan 30
	End

	Event
		Type	Local
		At	Offset_LArmR_Spiral
		BhvrOverride
			PositionOffset		0 0 0.2
		End
		part	:DNA_Proteins_Arm.part
		Lifespan 12
	End

	Event
		Type	Local
		At	Offset_LArmR_Spiral
		BhvrOverride
			PositionOffset		0 0 -0.2
		End
		part	:DNA_Proteins_Arm.part
		Lifespan 12
	End

	Event
		Type	Local
		At	UArmR
		part	:Leg_Glow.part
		POther	LArmR
		Lifespan 20
	End
End

##############################################################

Condition
	On	Time
	Time	5

	Event
		Type	Local
		At	LArmL
		part	:Leg_Glow_Point.part
		Lifespan 20
	End
End

Condition
	On	Time
	Time	10

	Event
		Type	Local
		At	LArmL
		part	:Leg_Glow.part
		POther	WepL
		Lifespan 20
	End
End

Condition
	On	Time
	Time	12

	Event
		Type	Local
		At	WepL
		part	:Extremity_Glow.part
		part	:Extremity_Ring.part
		Lifespan 20
	End
End

##############################################################

Condition
	On	Time
	Time	8

	Event
		Type	Local
		At	LArmR
		part	:Leg_Glow_Point.part
		Lifespan 20
	End
End

Condition
	On	Time
	Time	13

	Event
		Type	Local
		At	LArmR
		part	:Leg_Glow.part
		POther	WepR
		Lifespan 20
	End
End

Condition
	On	Time
	Time	15

	Event
		Type	Local
		At	WepR
		BhvrOverride
			PositionOffset		0 0 0.35
		End
		part	:Extremity_Glow.part
		part	:Extremity_Ring.part
		Lifespan 20
	End
End

#############################################################

End