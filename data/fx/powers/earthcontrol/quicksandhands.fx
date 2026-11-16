#########################################################
##	Stone_Wall
#########################################################

FxInfo

Input  
	Inpname	WepL
End

LifeSpan	75

Condition
	On 	Time
	Time 	0
	
	Event
		EName 	Stonesickle05
		Type	local
		At	WepL
		
		
		Part2	:GenericEarthHandsDust.part
		Part3	:GenericEarthHandsDust2.part
		Part4	:GenericEarthHandsDustRocks.part
		Part	:GenericEarthHandsDustRocks2.part
		Sound Rumble2 80 80 .8

		LifeSpan	75
	End
End

End


