#############################################################
## StoneCageHands.fx
#############################################################

FxInfo

Lifespan 21

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	local
		At	WepR
		Sound Rumble2 80 80 0.7
	End
End

Condition
	On 	Time
	Time 	22

	Event
		Type	local
		At	WepR
		Sound PP13 80 80 0.8
	End
End

#############################################################

Condition
	On 	Time
	Time 	4

	Event
		EName 	Stonesickle05
		Type	local
		At	WepR
		Part2	CustomizeablePowers\EarthControl\GenericEarthHandsDust.part
		Part3	CustomizeablePowers\EarthControl\GenericEarthHandsDust2.part
		Sound Rumble2 80 80 0.7
		LifeSpan 21
	End

	Event
		EName 	Stonesickle05
		Type	local
		At	WepL
		Part2	CustomizeablePowers\EarthControl\GenericEarthHandsDust.part
		Part3	CustomizeablePowers\EarthControl\GenericEarthHandsDust2.part
		LifeSpan 21
	End
End

Condition
	On 	Time
	Time 	32

	Event
		EName 	HandClapEnergyBall
		Type	local
		At	WepR
		bhvr	CustomizeablePowers\EarthControl\EarthClapShake.bhvr
		LifeSpan 5
	End
End


Condition
	On 	Time
	Time 	22

	Event
		EName 	HandClapEnergyBall
		Type	local
		At	WepR
		bhvr	CustomizeablePowers\EarthControl\ScaleEarthGraspHandHit.bhvr
		Geom	EarthGraspHit
		LifeSpan 5
	End

	Event
		EName 	Stonesickle05
		Type	local
		At	WepR
		Part	CustomizeablePowers\EarthControl\GenericEarthRockScaterLarge.part
		Part	CustomizeablePowers\EarthControl\GenericEarthRockScaterLarge2.part
		LifeSpan 2
	End

	Event
		EName 	Stonesickle05
		Type	local
		At	WepL
		Part	CustomizeablePowers\EarthControl\GenericEarthRockScaterLarge.part
		Part	CustomizeablePowers\EarthControl\GenericEarthRockScaterLarge2.part
		LifeSpan 2
	End
End

#############################################################

End