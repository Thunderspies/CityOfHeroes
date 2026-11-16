#########################################################
##	template

FxInfo

Input
	Inpname Origin
End

Input
	Inpname Mystic
End

Input
	Inpname WepR
End

Lifespan 200


Condition
	On 	Time
	Time 	37

	Event
		Type	local
		At	Origin
		Sound Swordtwirl 100 100 .9
	End

End


Condition
	On 	Time
	Time 	62

	Event
		EName	Streak
		Type	Start
		At	Mystic
		Geom	swoosh
		bhvr	behaviors/Medieval1.bhvr
		Sound Whoosh5 100.0 100.0 .6
		LifeSpan 1
	End

End

Condition
	On	Time
	Time	63
		
	Event	
		ENAME	Ring
		Type	Posit
		At	WepR
		part1	:MedievalRing.part
		Lifespan 1
	End

End

Condition
	On	Time
	Time	63
		
	Event	
		ENAME	Ring
		Type	Posit
		At	WepR
		part1	:MedievalGlow.part
		Lifespan 1
	End

End

	
End	

	