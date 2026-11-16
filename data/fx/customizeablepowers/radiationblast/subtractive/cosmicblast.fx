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
		Part1	:RadiationInfectOthersHands.part
		Part2	:RadiationHandElectrons.part
		Part3	:InfectBubbleDotsHands.part
		Part4	:InfectBubblesHands.part
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
		Part1	:RadiationLightHitBurst.part
		Part2	:BurstRings.Part
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
		Part1	:RadiationLightHitBurst3.part
		Part2	:BurstRings2.Part
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
		Part3	:NeutrinoStreak.part
		Part3	:NeutrinoStreak_Alpha.part

		Part2	:RadiationHandElectronsLocal.part
		Part3	:InfectBubbleDotsHandsLocal.part
		Part4	:InfectBubblesHandsLocal.part
		Part3	:InfectBubbleDotsHandsStreak.part
		Part4	:InfectBubblesHandsStreak.part
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