#############################################################
## SecondaryMutation_HeightenedSpeed_Hit.fx
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
		Sound HeightenedSpeed_Hit_01 100 100 .7
	End

	Event
		EName 	BigSpinnerL
		Type	Local
		At	LLegL
		Bhvr	:LegSpiral.bhvr
		BhvrOverride
			PositionOffset		0.0 1.0 0.0
			Spin			0.0 -0.1 0.0
		End
		Lifespan 30
	End

	Event
		EName 	Offset_FootL_Spiral
		Type	Local
		At	BigSpinnerL
		Bhvr	:LegSpiral.bhvr
		BhvrOverride
			Spin			0.0 0.7 0.0
		End
		Lifespan 30
	End

	Event
		Type	Local
		At	Offset_FootL_Spiral
		BhvrOverride
			PositionOffset		0 0 -0.35
		End
		part	:DNA_Proteins.part
		Lifespan 20
	End

	Event
		Type	Local
		At	Offset_FootL_Spiral
		BhvrOverride
			PositionOffset		0 0 0.35
		End
		part	:DNA_Proteins.part
		Lifespan 20
	End

	Event
		Type	Local
		At	ULegL
		part	:Leg_Glow.part
		POther	LLegL
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
		At	LLegR
		Bhvr	:LegSpiral.bhvr
		BhvrOverride
			PositionOffset		0.0 1.0 0.0
			Spin			0.0 0.1 0.0
		End
		Lifespan 30
	End

	Event
		EName 	Offset_FootR_Spiral
		Type	Local
		At	BigSpinnerR
		Bhvr	:LegSpiral.bhvr
		BhvrOverride
			Spin			0.0 -0.7 0.0
		End
		Lifespan 30
	End

	Event
		Type	Local
		At	Offset_FootR_Spiral
		BhvrOverride
			PositionOffset		0 0 -0.35
		End
		part	:DNA_Proteins.part
		Lifespan 20
	End

	Event
		Type	Local
		At	Offset_FootR_Spiral
		BhvrOverride
			PositionOffset		0 0 0.35
		End
		part	:DNA_Proteins.part
		Lifespan 20
	End

	Event
		Type	Local
		At	ULegR
		part	:Leg_Glow.part
		POther	LLegR
		Lifespan 20
	End
End

#############################################################

Condition
	On	Time
	Time	5

	Event
		Type	Local
		At	LLegL
		part	:Leg_Glow_Point.part
		Lifespan 20
	End
End

Condition
	On	Time
	Time	10

	Event
		Type	Local
		At	LLegL
		part	:Leg_Glow.part
		POther	FootL
		Lifespan 20
	End
End

Condition
	On	Time
	Time	15

	Event
		Type	Local
		At	FootL
		BhvrOverride
			PositionOffset		0 0 0.35
		End
		part	:Extremity_Glow.part
		part	:Extremity_Ring.part
		Lifespan 20
	End
End

#############################################################

Condition
	On	Time
	Time	8

	Event
		Type	Local
		At	LLegR
		part	:Leg_Glow_Point.part
		Lifespan 20
	End
End

Condition
	On	Time
	Time	13

	Event
		Type	Local
		At	LLegR
		part	:Leg_Glow.part
		POther	FootR
		Lifespan 20
	End
End

Condition
	On	Time
	Time	18

	Event
		Type	Local
		At	FootR
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