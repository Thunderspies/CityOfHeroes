#############################################################
## StreetSteam01.fx
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
		Type	Local
		At	Origin #Origin
		BhvrOverride
			PositionOffset	0.0 -1.0 0.0
		End
		Part	:Steam_High.part
		Part	:Steam_High_Highlight.part
		Sound Steam4_Loop 35.0 35.0 .1
	End
End

#############################################################

End