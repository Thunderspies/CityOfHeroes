#############################################################
## Medieval_Axe_1.fx
#############################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	WepR
		SetState RIGHTHAND
		Sound axeout 60 60 .6
	End
End

#############################################################

End