#########################################################
##	template

FxInfo


Lifespan 100

Condition
	On 	Time
	Time 	33

	Event
		Type	local
		At	Mystic
		Sound Macetwirl 100 100 .9
	End

End



Condition
	On	Time
	Time	55
		
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
	Time	55
		
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
	Time	56
		
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
	Time	57
		
	Event	
		ENAME	Ring
		Type	Start
		At	Mystic
		part	:SwordTwirlRing.part
		Lifespan 3
	End

End


	
End	

	