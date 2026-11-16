#############################################################
## FX_policedrone_alphastrike.fx
#############################################################

FxInfo
Flags	InheritAlpha

#############################################################

Condition

	On 	Time
	Time 	0

	Event
		Type	Local
		At	origin
		Sound PRadio2_loop 66 66 .3
	End
End

Condition
	On 	Time
	Time 	0

	Event
		EName	Glass
		Type	Local
		At	chest
		Geom	PoliceGlass
	End


End

#############################################################

End