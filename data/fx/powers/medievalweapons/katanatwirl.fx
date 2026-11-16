#########################################################
##	template

FxInfo


Lifespan 100

Condition
	On 	Time
	Time 	3

	Event
		Type	local
		At	Mystic
		Sound Macetwirl 100 100 .9
	End

End



Condition
	On	Time
	Time	25
		
	Event	
		ENAME	Ring
		Type	Start
		At	Mystic
		part1	:MedievalRing.part
		Lifespan 1
	End

End

Condition
	On	Time
	Time	25
		
	Event	
		ENAME	Glow
		Type	Start
		At	Mystic
		part1	:MedievalGlow.part
		Lifespan 1
	End

End

Condition
	On	Time
	Time	26
		
	Event	
		ENAME	Sparks
		Type	Start
		At	Mystic
		part	:MedievalSparks.part
		Lifespan 3
	End

End

Condition
	On	Time
	Time	27
		
	Event	
		ENAME	Ring
		Type	Start
		At	Mystic
		part	:SwordTwirlRing.part
		Lifespan 3
	End

End


	
End	

	