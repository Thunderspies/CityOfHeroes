#############################################################
## HEADER
#############################################################

FxInfo

LifeSpan 25

#############################################################
Condition
	On 	Time
	Time 	7
	Random	1

	Event
		EName	Sound1
		Type	Start
		At	Origin
		Sound Roman_soldier_sword_slash_1 100.0 100.0 .35
	End

	Event
		EName	Sound2
		Type	Start
		At	Origin
		Sound Roman_soldier_sword_slash_2 100.0 100.0 .35
	End

	Event
		EName	Sound3
		Type	Start
		At	Origin
		Sound Roman_soldier_sword_slash_3 100.0 100.0 .35
	End

End
#############################################################
Condition
	On 	Time
	Time 	15

	Event
		EName	Streak
		Type	local
		At	Mystic
		Geom	swoosh
		bhvr	behaviors/Medieval1.bhvr
		Lifespan 1
	End
End

#############################################################

End