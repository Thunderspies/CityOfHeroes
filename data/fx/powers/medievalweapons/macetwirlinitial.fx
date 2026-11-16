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

Input
	Inpname Root
End

Lifespan 200


Condition
	On 	Time
	Time 	37

	Event
		Type	local
		At	Origin
		Sound Macetwirl 100 100 .9
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
	Time	66
		
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
	Time	66
		
	Event	
		ENAME	Ring
		Type	Posit
		At	WepR
		part1	:MedievalGlow.part
		Lifespan 1
	End

End


Condition
	On	Time
	Time	65
		
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
	Time	66
		
	Event	
		ENAME	Ring
		Type	Start
		At	Root
		part	:SwordTwirlRing.part
		Lifespan 3
	End

End

End	

	