#############################################################
## TorchFire_Ghostly.fx
#############################################################

FxInfo
LifeSpan 100

#########################################################


Condition
	On 	Time
	Time 	0



	Event
		EName 	Prime
		Type	Start
		At	Root
		Part	WORLD/Lairs/cot/Base_Smoke.part
		Sound Torch1_loop 22.0 22.0 1.0
		Lifespan 80
	End
End

Condition
	On	Time
	Time	10

	Event
		EName	PoleFire
		Type	Start
		At	Root
		BhvrOverride
			PositionOffset	0 3 0
		End
		Part	WORLD/Lairs/cot/Pole_Smoke_Vertical.part
		Part	WORLD/Lairs/cot/Pole_Smoke_Vertical_Alpha.part
		Lifespan 80
	End
End

Condition
	On	Time
	Time	20

	Event
		EName	PoleFire2
		Type	Start
		At	Root
		BhvrOverride
			PositionOffset	0 11 0
		End
		Part	WORLD/Lairs/cot/Pole_Smoke_Vertical.part
		Part	WORLD/Lairs/cot/Pole_Smoke_Vertical_Alpha.part
		Lifespan 80
	End
End

Condition
	On	Time
	Time	25

	Event
		EName 	Offset
		Type	Start
		At	Root
		BhvrOverride
			PositionOffset	0 21 10
		End
		Sound Torch1_loop 22.0 22.0 1.0
	End

	Event
		EName 	Offset2
		Type	Start
		At	Root
		BhvrOverride
			PositionOffset	0 21 -10
		End
		Sound Torch1_loop 22.0 22.0 1.0
	End

	Event
		EName	BeamFire
		Type	Start
		At	Root
		BhvrOverride
			PositionOffset	0 21 0
		End
		Part	WORLD/Lairs/cot/Pole_Smoke_Horizontal.part
		POther 	Offset
		Lifespan 70
	End

	Event
		EName	BeamFire
		Type	Start
		At	Root
		BhvrOverride
			PositionOffset	0 21 0
		End
		Part	WORLD/Lairs/cot/Pole_Smoke_Horizontal.part
		POther 	Offset2
		Lifespan 70
	End
End

#########################################################

End