#############################################################
## Steam_High.fx
#############################################################

FxInfo
LifeSpan 0

#############################################################

Input
	InpName	Origin
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Head
		POther	Neck
	End


	Event
		Type	Local
		At	LArmL
		POther	WepL
	End

	Event
		Type	Local
		At	LArmR
		POther	WepR
	End


	Event
		Type	Local
		At	FootL
		POther	LLegL
	End

	Event
		Type	Local
		At	FootR
		POther	LLegR
	End
End

#############################################################

End