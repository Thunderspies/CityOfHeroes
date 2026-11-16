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
		At	WepL
		Part1	CustomizeablePowers\RadiationBlast\RadiationInfectOthersHands.part
		Part2	CustomizeablePowers\RadiationBlast\RadiationHandElectrons.part
		Part3	CustomizeablePowers\RadiationBlast\InfectBubbleDotsHands.part
		Part4	CustomizeablePowers\RadiationBlast\InfectBubblesHands.part
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
		Part1	CustomizeablePowers\RadiationBlast\RadiationLightHitBurst.part
		Part2	CustomizeablePowers\RadiationBlast\BurstRings.Part
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
		Part1	CustomizeablePowers\RadiationBlast\RadiationLightHitBurst3.part
		Part2	CustomizeablePowers\RadiationBlast\BurstRings2.Part
	End

	Event
		EName 	Prime
		Type	Start
		At	WepL
		Bhvr	CustomizeablePowers\RadiationBlast\Medianprojectile1.bhvr
		BhvrOverride
			TrackRate		2.0
			TintGeom		1
		End
		Part3	CustomizeablePowers\RadiationBlast\NeutrinoStreak.part
		Part2	CustomizeablePowers\RadiationBlast\RadiationHandElectronsLocal.part
		Part3	CustomizeablePowers\RadiationBlast\InfectBubbleDotsHandsLocal.part
		Part4	CustomizeablePowers\RadiationBlast\InfectBubblesHandsLocal.part
		Part3	CustomizeablePowers\RadiationBlast\InfectBubbleDotsHandsStreak.part
		Part4	CustomizeablePowers\RadiationBlast\InfectBubblesHandsStreak.part
		Geom	Tintable_FX_Locknar
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