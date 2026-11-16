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
		Part	:White_01.part
		Sound RularuuHit 80 80 .76
		Lifespan	3
	End

	Event
		EName 	Hamidon
		Type	local
		At	Waist
		Part	:White_02.part
		Part	:White_03.part
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
		Part	:White_Sparks.part
		Lifespan	3
	End
End
