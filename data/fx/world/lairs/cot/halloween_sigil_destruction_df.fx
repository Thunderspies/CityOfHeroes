#############################################################
## Halloween_Sigil_Destruction.fx
#############################################################

FxInfo
LifeSpan 100

#########################################################

Condition
	On	Time
	Time	16

	Event
		EName 	Prime
		Type	Start
		At	Root
		BhvrOverride
			PositionOffset	0 -15 0
		End
		Part	WORLD/Lairs/cot/Base_Fire_Burst2_DF.part
		Part	WORLD/Lairs/cot/Base_Sparks_Burst_DF.part
		Sound Sigil_Destruction 500 500 1
		Lifespan 20
	End
End

Condition
	On	Time
	Time	20

	Event
		EName 	Prime
		Type	Start
		At	Root
		Part	WORLD/Lairs/cot/Base_Smoke_DF.part
		Part	WORLD/Lairs/cot/Base_Smoke_Additive_DF.part
		Part	WORLD/Lairs/cot/Base_Smoke_Origin_DF.part
		Part	WORLD/Lairs/cot/Base_Smoke_Origin_Additive_DF.part
		Lifespan 80
	End
End


Condition
	On	Time
	Time	30

	Event
		EName	PoleFire
		Type	Start
		At	Root
		BhvrOverride
			PositionOffset	0 5 0
		End
		Part	WORLD/Lairs/cot/Pole_Smoke_Vertical_DF.part
		Part	WORLD/Lairs/cot/Pole_Smoke_Vertical_Alpha_DF.part
		Lifespan 80
	End
End

Condition
	On	Time
	Time	38

	Event
		EName	PoleFire2
		Type	Start
		At	Root
		BhvrOverride
			PositionOffset	0 11 0
		End

		Part	WORLD/Lairs/cot/Pole_Smoke_Vertical_DF.part
		Part	WORLD/Lairs/cot/Pole_Smoke_Vertical_Alpha_DF.part
		Lifespan 80
	End
End

Condition
	On	Time
	Time	45

	Event
		EName 	Offset
		Type	Start
		At	Root
		BhvrOverride
			PositionOffset	0 25 10
		End
	End

	Event
		EName 	Offset2
		Type	Start
		At	Root
		BhvrOverride
			PositionOffset	0 25 -10
		End
	End

	Event
		EName	BeamFire
		Type	Start
		At	Root
		BhvrOverride
			PositionOffset	0 25 0
		End
		Part	WORLD/Lairs/cot/Pole_Smoke_Horizontal_DF.part
		Part	WORLD/Lairs/cot/Pole_Smoke_Horizontal_Additive_DF.part

		POther 	Offset
		Lifespan 70
	End

	Event
		EName	BeamFire2
		Type	Start
		At	Root
		BhvrOverride
			PositionOffset	0 25 0
		End
		Part	WORLD/Lairs/cot/Pole_Smoke_Horizontal_DF.part
		Part	WORLD/Lairs/cot/Pole_Smoke_Horizontal_Additive_DF.part

		POther 	Offset2
		Lifespan 70
	End
End

#########################################################

End