#############################################################
## Random Sound Test
#############################################################

FxInfo

Lifespan 1

#############################################################

Condition
	On	Time
	Time	0
	Random	1

	Event
		Type	Start
		At	origin
		Sound PunchHit1 100.0 100.0 .8
	End

	Event
		Type	Start
		At	origin
		Sound PunchHit2 100.0 100.0 .8
	End

	Event
		Type	Start
		At	origin
		Sound PP13 80 80 .8
	End
End

#############################################################

End