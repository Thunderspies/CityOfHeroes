#############################################################
## VolcanicGassHands.fx
#############################################################

FxInfo

Lifespan 40

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	local
		At	WepL
		Part	CustomizeablePowers\PlantControl\GenericEarthHandsDust.part
		Part	CustomizeablePowers\PlantControl\GenericEarthHandsDust2.part
		Sound Rumble1 80 80 0.7
		Lifespan 40
	End
End

#############################################################

End