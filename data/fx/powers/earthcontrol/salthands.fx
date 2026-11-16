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

LifeSpan	20

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
		Sound Rumble4 80 80 .7

		LifeSpan	20
	End

	Event
		EName 	Stonesickle05
		Type	local
		At	WepL
		
		
		Part2	:GenericEarthHandsDust.part
		Part3	:GenericEarthHandsDust2.part
		Part4	:GenericEarthHandsDustRocks.part
		Part	:GenericEarthHandsDustRocks2.part
		

		LifeSpan	20
	End
End

End


