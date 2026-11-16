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
	On 	time
	Time 	0

	Event
		EName	SmokeAnchor
		Type	Local
		At	Root
	End

	Event
		Type	Local
		At	SmokeAnchor
		BhvrOverride
			PositionOffset	-2.0 0.0 0.0
		End
		Part1	:Steam_Long_Street.part
		Part	:Steam_Long_Highlight_Street.part
	End

	Event
		Type	Local
		At	SmokeAnchor
		BhvrOverride
			PositionOffset	2.0 0.0 0.0
		End
		Part1	:Steam_Long_Street.part
		Part	:Steam_Long_Highlight_Street.part
	End
End

#############################################################

End