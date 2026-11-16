#############################################################
## HEADER
#############################################################

FxInfo

Lifespan 30

#############################################################
Condition
	On 	Time
	Time 	12
	Random	1

	Event
		EName	Sound1
		Type	local
		At	Origin
		Sound Roman_soldier_sword_hack_1 100.0 100.0 .35
	End

	Event
		EName	Sound2
		Type	local
		At	Origin
		Sound Roman_soldier_sword_hack_2 100.0 100.0 .35
	End

	Event
		EName	Sound3
		Type	local
		At	Origin
		Sound Roman_soldier_sword_hack_3 100.0 100.0 .35
	End

End
############################################################
Condition
	On 	Time
	Time 	22

	Event
		EName	Streak
		Type	local
		At	Mystic
		Geom	swoosh
		bhvr	behaviors\Medieval1.bhvr
		Lifespan 1
	End
End

#############################################################

End