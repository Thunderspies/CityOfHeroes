#############################################################
## Header
#############################################################

FxInfo

LifeSpan 90

Flags InheritAnimScale

## SOUNDS ###########################################################

Condition
	On 	Time
	Time 	37

	Event
		Type	Start
		At	Root
		Sound CompBeep6_loop 120 120 .46
	End

	Event
		Type	Start
		At	Root
		Sound M1.ogg 120 120 .46
		Lifespan 10
	End
End

Condition
	On 	Time
	Time 	85

	Event
		Type	Start
		At	Root
		Sound med4 50 50 .46
		Lifespan 10
	End

End

#############################################################

End