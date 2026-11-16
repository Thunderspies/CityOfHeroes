#############################################################
## PowerPunchAttached.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	WepL
		Part1	powers\powerpunch\powerpunch1.part
		Part2	powers\powerpunch\powerpunch2.part
		Part1	powers\powerpunch\powerpunch3.part
		Part3   powers\powerpunch\powerpunchGlow.part
		Part3   powers\powerpunch\powerpunchRings.part
		Sound powerup7 100 100 0.5
	End

	Event
		Type	Local
		At	WepR
		Part1	powers\powerpunch\powerpunch1.part
		Part2	powers\powerpunch\powerpunch2.part
		Part1	powers\powerpunch\powerpunch3.part
		Part3   powers\powerpunch\powerpunchGlow.part
		Part3   powers\powerpunch\powerpunchRings.part
		Sound forcefield5_loop 60 60 0.33
	End
End

#############################################################

End