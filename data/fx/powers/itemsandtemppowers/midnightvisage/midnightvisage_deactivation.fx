#############################################################
## HEADER
#############################################################

FxInfo

LifeSpan 120

#############################################################

Condition
	On	Time
	Time	0


	Event
		Type	Local
		At 	Origin
		Sound Midnight_visage_off 80 80 .8
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Root
		ChildFX	:Child_MidnightVisage.fx
	End
End

#############################################################

End