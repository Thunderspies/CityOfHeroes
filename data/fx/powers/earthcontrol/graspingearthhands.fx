#########################################################
##	Stone_Wall
#########################################################

FxInfo

Input  
	Inpname	WepL
End

Lifespan 30

Condition
	On 	Time
	Time 	0
	
	Event
		EName 	Stonesickle05
		Type	local
		At	WepL
		
		
		Part2	:GenericEarthHandsDust.part
		Part3	:GenericEarthHandsDust2.part
		Sound Rumble2 80 80 .7

		LifeSpan	30
	End

End

Condition
	On 	Time
	Time 	30

	Event
		EName 	Stonesickle05
		Type	local
		At	WepL
		
		
		Part	:GenericEarthRockScater.part
		Part4	:GenericEarthHandsDustRocks.part
		Part	:GenericEarthHandsDustRocks2.part

		LifeSpan	3
	End

End

End


