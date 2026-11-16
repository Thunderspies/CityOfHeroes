#########################################################
##	Stone_Wall
#########################################################

FxInfo

Input  
	Inpname	WepL
End

LifeSpan	40

Condition
	On 	Time
	Time 	5
	
	Event
		EName 	Stonesickle05
		Type	local
		At	WepL
		
		
		Part2	:GenericEarthHandsDust.part
		Part3	:GenericEarthHandsDust2.part
		Part4	:GenericEarthHandsDustRocks.part
		Part	:GenericEarthHandsDustRocks2.part
		Sound Rumble3 80 80 .8

		LifeSpan	40
	End
End


Condition
	On 	Time
	Time 	54
	
	Event
		EName 	Stonesickle05
		Type	local
		At	WepL
		
		Part1	:GenericEarthRockScater.part
		Part2	:GenericEarthRockScater2.part

		LifeSpan	40
	End
End

End


