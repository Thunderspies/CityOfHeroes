#############################################################
## HEADER
#############################################################

FxInfo

LifeSpan 120

#############################################################

Condition
	On	Time
	Time	40

	Event
		Type	Local
		At 	Origin
		Sound Midnight_visage_on 80 80 0.8
	End
End

#############################################################


Condition
	On	Time
	Time	45

	Event
		Type	Local
		At	Root
		Part	:Flash_Core.part
		Part	:Flash_Glow.part
	End
End

Condition
	On	Time
	Time	46

	Event
		Type	Local
		At	Root
		ChildFX	:Child_MidnightVisage.fx
	End
End

#############################################################

End