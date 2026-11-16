#########################################################
##	template

FxInfo

Input
	Inpname Mystic
End

Input
	Inpname WepR
End

Input
	Inpname Root
End


Input
	Inpname Origin
End

Lifespan 100

Condition
	On 	Time
	Time 	13

	Event
		Type	local
		At	Origin
		Sound Swordtwirl 100 100 .9
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
		bhvr	behaviors/Medieval1.bhvr
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
		Lifespan 1
	End

End

Condition
	On	Time
	Time	36
		
	Event	
		ENAME	Glow
		Type	Posit
		At	WepR
		part1	:MedievalGlow.part
		Lifespan 1
	End

End

Condition
	On	Time
	Time	36
		
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
		At	Mystic
		part	:SwordTwirlRing.part
		Lifespan 3
	End

End

	
End	

	