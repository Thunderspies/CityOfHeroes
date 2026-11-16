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
		Part1	:RadiationInfectOthersHands.part
		Part2	:RadiationHandElectrons.part
		Part3	:InfectBubbleDotsHands.part
		Part4	:InfectBubblesHands.part
		PMagnet	WepL
		Sound radiation3_loop 80 80 0.33
		Lifespan 90
	End

	Event
		EName	Hand2
		Type	Local
		At	WepR
		Part1	:RadiationInfectOthersHands.part
		Part2	:RadiationHandElectrons.part
		Part3	:InfectBubbleDotsHands.part
		Part4	:InfectBubblesHands.part
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
		EName 	Prime
		Type	Start
		At	Root
		Bhvr	CustomizeablePowers\RadiationBlast\MedianProjectile1.bhvr
		Part2	:ElectronHazeBalls.part
		Part3	:ElectronHazeClouds.part
		Part3	:ElectronHazeClouds_Alpha.part
		Part4	:ElectronHazeRings.part
		Sound electronhaze 60 60 .92
		Magnet	T_Root
		LookAt	T_Root
	End
End

Condition
	On 	Time
	Time 	60

	Event
		EName	Prime2
		Type	Local
		At	Prime
		Bhvr	CustomizeablePowers\RadiationBlast\ElectronHaze.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Tintable_RadiationWave
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