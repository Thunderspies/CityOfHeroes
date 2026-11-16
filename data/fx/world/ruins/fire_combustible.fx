#############################################################
## Fire_Combustible.fx
#############################################################

FxInfo
LifeSpan 0
Input
	InpName	Origin
End
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
		Part	:Fire_Combustible_Core.part
	End

	Event
		EName 	Prime
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset  0 -3 0
		End
		Part	:Fire_Combustible_Secondary.part
	End

#############################################################
End