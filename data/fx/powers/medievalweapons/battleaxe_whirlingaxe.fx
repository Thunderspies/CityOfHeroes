#############################################################
## HEADER
#############################################################

FxInfo

Lifespan 40

#############################################################

Condition
	On 	Time
	Time 	13

	Event
		Type	local
		At	Origin
		Sound Macetwirl 100 100 .9
	End
End

Condition
	On 	Time
	Time 	35

	Event
		EName	Streak
		Type	local
		At	Mystic
		Geom	swoosh
		bhvr	behaviors\Medieval1.bhvr
		Lifespan 1
	End
End

Condition
	On	Time
	Time	36

	Event
		ENAME	Ring
		Type	Posit
		At	WepR
		part1	:MedievalRing.part
		part1	:MedievalGlow.part
		Lifespan 1
	End

	Event
		ENAME	Sparks
		Type	Start
		At	WepR
		part	:MedievalSparks.part
		Lifespan 3
	End
End

Condition
	On	Time
	Time	37

	Event
		ENAME	Ring
		Type	Start
		At	Root
		part	:SwordTwirlRing.part
		Lifespan 3
	End
End

#############################################################

End