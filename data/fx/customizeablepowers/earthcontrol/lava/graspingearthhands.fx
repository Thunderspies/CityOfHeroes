#############################################################
## GraspingEarthHands.fx
#############################################################

FxInfo

Lifespan 30

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	local
		At	WepL
		Part	CustomizeablePowers\EarthControl\GenericEarthHandsDust.part
		Part	CustomizeablePowers\EarthControl\GenericEarthHandsDust2.part
		Sound Rumble2 80 80 0.7
		Lifespan 30
	End
End

Condition
	On 	Time
	Time 	30

	Event
		Type	local
		At	WepL
		Part	CustomizeablePowers\EarthControl\GenericEarthRockScater.part
		Part	CustomizeablePowers\EarthControl\GenericEarthHandsDustRocks.part
		Part	CustomizeablePowers\EarthControl\GenericEarthHandsDustRocks2.part
		Lifespan 3
	End
End

#############################################################

End