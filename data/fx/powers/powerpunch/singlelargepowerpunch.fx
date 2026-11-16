#############################################################
## SingleLargePowerPunch.fx
#############################################################

FxInfo

Lifespan 55

#############################################################

Condition
	On 	Time
	Time 	10

	Event
		EName 	hand2
		Type	Local
		At	WepL
		Part1	POWERS\PowerPunch\Largepowerpunch1.part
		Part2	POWERS\PowerPunch\Largepowerpunch2.part
		Part3   POWERS\PowerPunch\LargepowerpunchGlow.part
		Sound PunchA 100 100 0.6
	End

	Event
		EName 	hand
		Type	Local
		At	WepL
		Part1	POWERS\PowerPunch\Largepowerpunch3.part
		Part3   POWERS\PowerPunch\LargepowerpunchRings.part
	End

	Event
		EName 	hand2
		Type	Local
		At	WepR
		Part1	POWERS\PowerPunch\Largepowerpunch1.part
		Part2	POWERS\PowerPunch\Largepowerpunch2.part
		Part3   POWERS\PowerPunch\LargepowerpunchGlow.part
	End

	Event
		EName 	hand
		Type	Local
		At	WepR
		Part1	POWERS\PowerPunch\Largepowerpunch3.part
		Part3   POWERS\PowerPunch\LargepowerpunchRings.part
	End
End

#############################################################

End