#########################################################

FxInfo
Flags	InheritAlpha
Lifespan 100

##########################################################

Condition
	On 	Time
	Time 	20

	Event
		EName 	Stonesickle05
		Type	local
		At	WepR


		Part2	POWERS\EarthControl\GenericEarthHandsDust.part
		Part3	POWERS\EarthControl\GenericEarthHandsDust2.part
		Part4	POWERS\EarthControl\GenericEarthHandsDustRocks.part
		Part	POWERS\EarthControl\GenericEarthHandsDustRocks2.part
		Sound Rumble2 80 80 .8

		LifeSpan	55
	End

	Event
		EName 	Stonesickle05
		Type	local
		At	WepL


		Part2	POWERS\EarthControl\GenericEarthHandsDust.part
		Part3	POWERS\EarthControl\GenericEarthHandsDust2.part
		Part4	POWERS\EarthControl\GenericEarthHandsDustRocks.part
		Part	POWERS\EarthControl\GenericEarthHandsDustRocks2.part

		LifeSpan	55
	End
End

###############################################################################

End