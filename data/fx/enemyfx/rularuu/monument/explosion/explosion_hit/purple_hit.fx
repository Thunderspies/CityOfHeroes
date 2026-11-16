#########################################################
##	template

FxInfo

Lifespan	50



Condition
	On 	Time
	Time 	0	
	
	Event
		EName 	Hamidon
		Type	local
		At	Waist
		Part	:Purple_01.part
		Sound RularuuHit 80 80 .76
		Lifespan	3
	End

	Event
		EName 	Hamidon
		Type	local
		At	Waist
		Part	:Purple_02.part
		Lifespan	3
	End
End

Condition
	On 	Time
	Time 	5	
	
	Event
		EName 	Hamidon
		Type	local
		At	Waist
		Part	:Purple_Sparks.part
		Lifespan	3
	End
End
