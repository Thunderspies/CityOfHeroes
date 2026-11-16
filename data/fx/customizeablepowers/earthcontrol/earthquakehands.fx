#########################################################
##	Stone_Wall
#########################################################

FxInfo
LifeSpan	85

#########################################################

Condition
	On 	Time
	Time 	20

	Event
		EName 	Stonesickle05
		Type	local
		At	WepR


		Part2	CustomizeablePowers\EarthControl\GenericEarthHandsDust.part
		Part3	CustomizeablePowers\EarthControl\GenericEarthHandsDust2.part
		Part4	CustomizeablePowers\EarthControl\GenericEarthHandsDustRocks.part
		Part	CustomizeablePowers\EarthControl\GenericEarthHandsDustRocks2.part
		Sound Rumble2 80 80 .8

		LifeSpan	85
	End

	Event
		EName 	Stonesickle05
		Type	local
		At	WepL


		Part2	CustomizeablePowers\EarthControl\GenericEarthHandsDust.part
		Part3	CustomizeablePowers\EarthControl\GenericEarthHandsDust2.part
		Part4	CustomizeablePowers\EarthControl\GenericEarthHandsDustRocks.part
		Part	CustomizeablePowers\EarthControl\GenericEarthHandsDustRocks2.part

		LifeSpan	85
	End
End

#########################################################

End


