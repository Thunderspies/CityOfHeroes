#############################################################
## TarotCards_Attack.fx
#############################################################

FxInfo
Lifespan 1850

#############################################################

Condition
	On	Time
	Time	0
	
	Event
		Type Local
		At Origin
		Sound Devolution_Hit_01 100 100 .7
	End

	Event
		EName 	BigSpinner
		Type	Start
		At	Head
		Bhvr	:LegSpiral.bhvr
		BhvrOverride
			PositionOffset		0.0 -0.5 0.0
			InitialVelocity		0.0 -0.1 0.0
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
			PositionOffset		0 0 -0.8
		End
		part	:DNA_Proteins_Devolution.part
		Lifespan 50
	End

	Event
		Type	Local
		At	Offset_Head_Spiral
		BhvrOverride
			PositionOffset		0 0 0.8
		End
		part	:DNA_Proteins_Devolution.part

		Lifespan 50
	End
End

########################################################

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
		Type	Local
		At	Hips
		part	:Body_Glow.part
		POther	LLegR
		Lifespan 20
	End

	Event
		Type	Local
		At	Hips
		part	:Body_Glow.part
		POther	LLegL
		Lifespan 20
	End
End

Condition
	On	Time
	Time	17.5

	Event
		Type	Local
		At	LLegL
		part	:Leg_Glow_point.part
		Lifespan 20
	End
End

Condition
	On	Time
	Time	20

	Event
		Type	Local
		At	LLegL
		part	:Body_Glow.part
		POther	FootL
		Lifespan 20
	End

	Event
		Type	Local
		At	LLegR
		part	:Body_Glow.part
		POther	FootR
		Lifespan 20
	End
End

Condition
	On	Time
	Time	25

	Event
		Type	Start
		At	Root
		part	:Root_Glow_Devolution.part
		part	:Root_Glow_Flat.part
		part	:Root_Ring.part
		Part	:Root_Smoke.part
		Part	:Root_Smoke_Lingering.part
		part	:DNA_Proteins_Devolution_Falling.part

		Lifespan 5
	End
End

Condition
	On	Time
	Time	1800

	Event
		Type	Start
		At	Root
		part	:Root_Glow_Devolution.part
		part	:Root_Glow_Flat.part
		part	:Root_Ring.part
		Part	:Root_Smoke.part
		Part	:Root_Smoke_Lingering.part
		Lifespan 5
	End
End

#############################################################

End