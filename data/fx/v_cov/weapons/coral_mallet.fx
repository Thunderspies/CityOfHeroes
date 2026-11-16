#############################################################
## Header
#############################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time 	9

	Event
		EName 	Prime
		Type	Local
		At	WepR
		Geom	CoralMallet
		Bhvr	Behaviors/CoralMalletColor_Fade_UP.bhvr
		Sound maceout 60 60 .5
	End
End

#############################################################

End