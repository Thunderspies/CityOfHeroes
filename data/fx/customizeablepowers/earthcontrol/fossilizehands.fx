#############################################################
## FossilizeHands.fx
#############################################################

FxInfo

Lifespan 40

#############################################################

Condition
	On 	Time
	Time 	5

	Event
		Type	local
		At	WepL
		Part	CustomizeablePowers\EarthControl\GenericEarthHandsDust.part
		Part	CustomizeablePowers\EarthControl\GenericEarthHandsDust2.part
		Part	CustomizeablePowers\EarthControl\GenericEarthHandsDustRocks.part
		Part	CustomizeablePowers\EarthControl\GenericEarthHandsDustRocks2.part
		Sound Rumble3 80 80 0.8
		Lifespan 40
	End
End

Condition
	On 	Time
	Time 	54

	Event
		Type	local
		At	WepL
		Part	CustomizeablePowers\EarthControl\GenericEarthRockScater.part
		Part	CustomizeablePowers\EarthControl\GenericEarthRockScater2.part
		Lifespan 40
	End
End

#############################################################

End