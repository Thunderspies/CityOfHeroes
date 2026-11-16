#########################################################
##	Stone_Wall
#########################################################

FxInfo

Input  
	Inpname	WepL
End

Input  
	Inpname	WepR
End

LifeSpan	85

Condition
	On 	Time
	Time 	20
	
	Event
		EName 	Stonesickle05
		Type	local
		At	WepR
		
		
		Part2	:GenericEarthHandsDust.part
		Part3	:GenericEarthHandsDust2.part
		Part4	:GenericEarthHandsDustRocks.part
		Part	:GenericEarthHandsDustRocks2.part
		Sound Rumble2 80 80 .8

		LifeSpan	85
	End

	Event
		EName 	Stonesickle05
		Type	local
		At	WepL
		
		
		Part2	:GenericEarthHandsDust.part
		Part3	:GenericEarthHandsDust2.part
		Part4	:GenericEarthHandsDustRocks.part
		Part	:GenericEarthHandsDustRocks2.part

		LifeSpan	85
	End
End

End


