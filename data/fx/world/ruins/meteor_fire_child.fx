#############################################################
## MediumFireSlow.fx
#############################################################

FxInfo
LifeSpan 0
Input
	InpName	Origin
End

#############################################################

Condition
	On 	Cycle
	Time 	60
	Chance	0.1

	Event
		EName 	Prime
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset 	0 -1 0
		End
		Part	:Fire_FlareUp_Add.part
		Part	:Fire_FlareUp_Alpha.part
		Lifespan 45
	End
End


Condition
	On 	Cycle
	Time 	90
	Chance	0.1
	Event
		EName 	Prime
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset 	0 -1 0
		End
		Part	:Fire_Combustible_Core.part
		Part	:Fire_Combustible_Secondary.part
		Lifespan 30
	End

End



#############################################################

End