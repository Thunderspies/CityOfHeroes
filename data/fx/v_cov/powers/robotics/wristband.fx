#############################################################
## Header
#############################################################

FxInfo

LifeSpan  47

Flags InheritAnimScale

## SOUNDS ###########################################################

Condition
	On 	Time
	Time 	10

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

#############################################################

End