#############################################################
## N_Fire.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	Fire_Small01_Loop 50 50 .25
	End
End

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Start
		At	Root
		BhvrOverride
			PositionOffset  0 -1 0
		End
		Part	WORLD/fire/N_Fire_Additive.part
		Part	WORLD/fire/N_Fire_Alpha.part
	End
End

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	StartPositOnly
		At	Root
		BhvrOverride
			PositionOffset  0 0 0
		End
		Part	WORLD/fire/N_Fire_Ground.part
		Part	WORLD/fire/N_Fire_Embers.part
		Part	WORLD/fire/N_Fire_CoreAdd.part
	End
End

#############################################################
End