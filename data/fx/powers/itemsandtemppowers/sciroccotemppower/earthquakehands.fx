#############################################################
## EarthQuakeHands.fx
#############################################################

FxInfo
LifeSpan	85

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	local
		At	WepR
		Part	:GenericEarthHandsDust.part
		Part	:GenericEarthHandsDust2.part
		Sound Rumble2 80 80 .8
		LifeSpan	85
	End

	Event
		Type	local
		At	WepL
		Part	:GenericEarthHandsDust.part
		Part	:GenericEarthHandsDust2.part
		LifeSpan	85
	End
End

End


