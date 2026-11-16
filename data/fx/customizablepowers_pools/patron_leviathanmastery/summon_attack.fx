#########################################################
##
##
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
		Part	CustomizeablePowers\EarthControl\GenericEarthHandsDust.part
		Part	CustomizeablePowers\EarthControl\GenericEarthHandsDust2.part
		Part	CustomizeablePowers\EarthControl\GenericEarthHandsDustRocks.part
		Part	CustomizeablePowers\EarthControl\GenericEarthHandsDustRocks2.part
		Sound Rumble2 80 80 0.8
		Lifespan 55
	End

	Event
		EName 	Stonesickle05
		Type	local
		At	WepL
		Part	CustomizeablePowers\EarthControl\GenericEarthHandsDust.part
		Part	CustomizeablePowers\EarthControl\GenericEarthHandsDust2.part
		Part	CustomizeablePowers\EarthControl\GenericEarthHandsDustRocks.part
		Part	CustomizeablePowers\EarthControl\GenericEarthHandsDustRocks2.part
		Lifespan 55
	End
End

###############################################################################


End