#############################################################
## N_Fire.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Local
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
		Type	Local
		At	Prime
		Part	WORLD/fire/N_Fire_Ground.part
		Part	WORLD/fire/N_Fire_Embers.part
		Part	WORLD/fire/N_Fire_CoreAdd.part
	End
End


#############################################################
End