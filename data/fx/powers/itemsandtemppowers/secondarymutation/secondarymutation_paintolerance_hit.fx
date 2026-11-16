#############################################################
## SecondaryMutation_PainTolerance_Hit.fx
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
		Sound PainTolerance_Hit_01 100 100 .7
	End

	Event
		EName 	BigSpinner
		Type	Start
		At	Head
		Bhvr	:LegSpiral.bhvr
		BhvrOverride
			InitialVelocity		0.0 -0.04 0.0
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
			Spin			0.0 0.5 0.0
		End
		Lifespan 30
	End

	Event
		Type	Local
		At	Offset_Head_Spiral
		BhvrOverride
			PositionOffset		0 0 -0.7
		End
		part	:DNA_Proteins_Body.part
		Lifespan 20
	End

	Event
		Type	Local
		At	Offset_Head_Spiral
		BhvrOverride
			PositionOffset		0 0 0.7
		End
		part	:DNA_Proteins_Body.part
		Lifespan 20
	End
End

##############################################################

Condition
	On	Time
	Time	5

	Event
		Type	Local
		At	Hair
		part	:Body_Glow.part
		Pother	Chest
		Lifespan 20
	End
End

Condition
	On	Time
	Time	7.5

	Event
		Type	Local
		At	UArmL
		part	:Leg_Glow_point.part
		Lifespan 20
	End

	Event
		Type	Local
		At	UArmR
		part	:Leg_Glow_point.part
		Lifespan 20
	End
End

Condition
	On	Time
	Time	10

	Event
		Type	Local
		At	Chest
		part	:Body_Glow.part
		POther	Hips
		Lifespan 20
	End
End

Condition
	On	Time
	Time	15

	Event
		Type	Start
		At	Chest
		part	:Chest_Glow.part
		part	:Root_Ring.part
		Lifespan 20
	End
End

#############################################################

End