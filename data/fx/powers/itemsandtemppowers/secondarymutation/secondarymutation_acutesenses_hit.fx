#############################################################
## SecondaryMutation_AcuteSenses_Hit.fx
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
		Sound AcuteSenses_Hit_01 100 100 .7
	End

	Event
		EName 	BigSpinner
		Type	Local
		At	Hair
		Bhvr	:LegSpiral.bhvr
		BhvrOverride
			PositionOffset		0.0 1.0 0.0
			Spin			0.0 -0.1 0.0
		End
		Lifespan 30
	End

	Event
		EName 	Offset_Head_Spiral
		Type	Local
		At	BigSpinner
		Bhvr	:LegSpiral.bhvr
		BhvrOverride
			Spin			0.0 0.85 0.0
		End
		Lifespan 30
	End

	Event
		Type	Local
		At	Offset_Head_Spiral
		BhvrOverride
			PositionOffset		0 0 -0.35
		End
		part	:DNA_Proteins.part
		Lifespan 12
	End

	Event
		Type	Local
		At	Offset_Head_Spiral
		BhvrOverride
			PositionOffset		0 0 0.35
		End
		part	:DNA_Proteins.part
		Lifespan 12
	End
End

##############################################################

Condition
	On	Time
	Time	5

	Event
		Type	Local
		At	Head
		part	:Leg_Glow.part
		Lifespan 20
	End
End

Condition
	On	Time
	Time	12

	Event
		Type	Local
		At	Head
		part	:Extremity_Glow.part
		part	:Extremity_Ring.part
		Lifespan 20
	End
End

#############################################################

End