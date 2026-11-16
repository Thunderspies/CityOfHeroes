#############################################################
## ElectronHaze.fx
#############################################################

FxInfo

Lifespan 200

#############################################################

Condition
	On 	Time
	Time 	30

	Event
		EName	Hand2
		Type	Local
		At	WepL
		Part	:RadiationInfectOthersHands.part
		Part	:RadiationHandElectrons.part
		Part	:InfectBubbleDotsHands.part
		Part	:InfectBubblesHands.part
		PMagnet	WepL
		Sound radiation3_loop 80 80 0.33
		Lifespan 90
	End

	Event
		EName	Hand2
		Type	Local
		At	WepR
		Part	:RadiationInfectOthersHands.part
		Part	:RadiationHandElectrons.part
		Part	:InfectBubbleDotsHands.part
		Part	:InfectBubblesHands.part
		PMagnet	WepL
		Sound radiation3_loop 80 80 0.33
		Lifespan 90
	End
End

#############################################################

Condition
	On 	Time
	Time 	55

	Event
		EName	HandburstL
		Type	Local
		At	WepL
		Part	:RadiationLightHitBurst.part
		Part	:BurstRings.Part
		Lifespan 15
	End

	Event
		EName	HandburstR
		Type	Local
		At	WepR
		Part	:RadiationLightHitBurst.part
		Part	:BurstRings.Part
		Lifespan 15
	End
End

Condition
	On 	Time
	Time 	55

	Event
		EName 	Prime
		Type	Start
		At	Root
		Bhvr	behaviors\MedianProjectile1.bhvr
		Magnet	T_Root
		LookAt	T_Root
		Part	:ElectronHazeBalls.part
		Part	:ElectronHazeClouds.part
		Part	:ElectronHazeRings.part
		Sound electronhaze 60 60 0.92
	End
End

Condition
	On 	Time
	Time 	60

	Event
		EName	Prime2
		Type	Local
		At	Prime
		Bhvr	Behaviors\ElectronHaze.bhvr
		Geom	RadiationWave
	End
End

#############################################################

Condition
	On	PrimeHit

	Event
		Ename	Prime
		Type	Destroy
	End

	Event
		Ename	Prime2
		Type	Destroy
	End
End

#############################################################

End