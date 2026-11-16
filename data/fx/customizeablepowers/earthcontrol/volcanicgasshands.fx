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
	Time 	0
	
	Event
		EName 	Stonesickle05
		Type	local
		At	WepL
		
		
		Part2	CustomizeablePowers\EarthControl\GenericEarthHandsDust.part
		Part3	CustomizeablePowers\EarthControl\GenericEarthHandsDust2.part
		Sound Rumble1 80 80 .7
	

		LifeSpan	40
	End

End

Condition
	On 	Time
	Time 	50

	Event
		EName 	Stonesickle05
		Type	local
		At	WepL
		
		Part	CustomizeablePowers\EarthControl\GenericEarthRockScater.part
		Part	CustomizeablePowers\EarthControl\GenericEarthRockScater2.part
		Part4	CustomizeablePowers\EarthControl\GenericEarthHandsDustRocks.part
		Part	CustomizeablePowers\EarthControl\GenericEarthHandsDustRocks2.part

		LifeSpan	3
	End

End

End


