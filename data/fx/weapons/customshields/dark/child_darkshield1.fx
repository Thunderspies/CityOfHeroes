#############################################################
## DarkShield1.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Origin
		Part3	:Darkness_Puddle1.part
		Part1	:Darkness_Puddle1_Subtractive.part
		Part4	:Darkness_Puddle2.part
		Part2	:Darkness_Puddle2_Subtractive.part
	End
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	PositOnly
		At	Origin
		Part	:Ghosts1.part
		Part	:Ghosts2.part
		#Part	:Ghosts_Streaks.part
	End
End

## TRAILS ###########################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Origin
		Part1	:Darkness_Trail1.part
		Part1	:Darkness_Trail1_Subtractive.part
	End
End

#########################################################

End