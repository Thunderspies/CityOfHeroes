#############################################################
## Header
#############################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Local
		At	WepR
		Geom	GEO_WepR_Jack_Hammer01
		Sound axeout 60 60 .33
	End
End

Condition
	On 	Time
	Time 	10

	Event
		Type	Local
		At	WepR
		Sound jackhammer_loop 30 30 .25
	End
End

#############################################################

End