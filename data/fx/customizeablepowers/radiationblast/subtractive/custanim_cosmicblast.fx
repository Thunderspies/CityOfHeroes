#############################################################
## CosmicBlast.fx
#############################################################

FxInfo

Lifespan 200

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Hand2
		Type	Local
		At	WepR
		Part1	CustomizeablePowers\RadiationBlast\Subtractive\RadiationInfectOthersHands.part
		Part2	CustomizeablePowers\RadiationBlast\Subtractive\RadiationHandElectrons.part
		Part3	CustomizeablePowers\RadiationBlast\Subtractive\InfectBubbleDotsHands.part
		Part4	CustomizeablePowers\RadiationBlast\Subtractive\InfectBubblesHands.part
		PMagnet	WepL
		Sound radiation3_loop 80 80 .33
		Lifespan 70
	End

	Event
		EName	Hand2
		Type	Local
		At	WepL
		Part1	CustomizeablePowers\RadiationBlast\Subtractive\RadiationInfectOthersHands.part
		Part2	CustomizeablePowers\RadiationBlast\Subtractive\RadiationHandElectrons.part
		Part3	CustomizeablePowers\RadiationBlast\Subtractive\InfectBubbleDotsHands.part
		Part4	CustomizeablePowers\RadiationBlast\Subtractive\InfectBubblesHands.part
		PMagnet	WepL
		Sound radiation3_loop 80 80 .33
		Lifespan 70
	End
End

#############################################################

Condition
	On 	Time
	Time 	30

	Event
		EName	Handburst
		Type	Local
		At	WepL
		Part1	CustomizeablePowers\RadiationBlast\Subtractive\RadiationLightHitBurst.part
		Part2	CustomizeablePowers\RadiationBlast\Subtractive\BurstRings.Part
		Lifespan 5
	End

	Event
		EName	Handburst
		Type	Local
		At	WepR
		Part1	CustomizeablePowers\RadiationBlast\Subtractive\RadiationLightHitBurst.part
		Part2	CustomizeablePowers\RadiationBlast\Subtractive\BurstRings.Part
		Lifespan 5
	End
End

#############################################################

Condition
	On 	Time
	Time 	35

	Event
		Type	Start
		At	WepL
		Part1	CustomizeablePowers\RadiationBlast\Subtractive\RadiationLightHitBurst3.part
		Part2	CustomizeablePowers\RadiationBlast\Subtractive\BurstRings2.Part
	End

	Event
		Type	Start
		At	WepR
		Part1	CustomizeablePowers\RadiationBlast\Subtractive\RadiationLightHitBurst3.part
		Part2	CustomizeablePowers\RadiationBlast\Subtractive\BurstRings2.Part
	End

	Event
		EName 	Prime
		Type	Start
		At	Mystic
		Bhvr	CustomizeablePowers\RadiationBlast\Medianprojectile1.bhvr
		BhvrOverride
			TrackRate		2.0
			TintGeom		1
		End
		Part3	CustomizeablePowers\RadiationBlast\Subtractive\NeutrinoStreak.part
		Part2	CustomizeablePowers\RadiationBlast\Subtractive\RadiationHandElectronsLocal.part
		Part3	CustomizeablePowers\RadiationBlast\Subtractive\InfectBubbleDotsHandsLocal.part
		Part4	CustomizeablePowers\RadiationBlast\Subtractive\InfectBubblesHandsLocal.part
		Part3	CustomizeablePowers\RadiationBlast\Subtractive\InfectBubbleDotsHandsStreak.part
		Part4	CustomizeablePowers\RadiationBlast\Subtractive\InfectBubblesHandsStreak.part
		Geom	Tintable_Dark_FX_Locknar
		Magnet	Target
		LookAt	Target
		Sound radburst 80 80 .77
	End
End

#############################################################

Condition
	On	PrimeHit

	Event
		Ename	Prime
		Type	Destroy
	End
End

#############################################################

End