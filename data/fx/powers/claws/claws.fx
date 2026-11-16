#############################################################
## Claws.fx
#############################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On	Time
	Time	12

	Event
		EName	Right
		Type 	Local
		At	WepR
		Geom	GEO_WepR_ClawR
		Sound Claw1d 100.0 100.0 .7
	End

	Event
		Ename	Left
		Type	Local
		At	WepL
		Geom	GEO_WepL_ClawL
	End
End

#############################################################

End