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

Condition
	On 	Time
	Time 	0
	
	Event
		EName 	Stonesickle05
		Type	local
		At	WepR
		
		
		Part2	:GenericEarthHandsDust.part
		Part3	:GenericEarthHandsDust2.part
		Part4	:GenericEarthHandsDustRocks.part
		Part	:GenericEarthHandsDustRocks2.part

		LifeSpan	40
	End

	Event
		EName 	Stonesickle05
		Type	local
		At	WepL
		
		
		Part2	:GenericEarthHandsDust.part
		Part3	:GenericEarthHandsDust2.part
		Part4	:GenericEarthHandsDustRocks.part
		Part	:GenericEarthHandsDustRocks2.part

		LifeSpan	40
	End
End

End


