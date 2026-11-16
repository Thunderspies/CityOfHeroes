#############################################################
## NeutrinoBlast.fx
#############################################################

FxInfo

Lifespan 200

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	WepL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part1	:RadiationInfectOthersHands.part
		Part2	:RadiationHandElectrons.part
		Part3	:InfectBubbleDotsHands.part
		Part4	:InfectBubblesHands.part
		PMagnet	WepR
		Sound radiation3_loop 80 80 .34
		Lifespan 60
	End

	Event
		Type	Local
		At	WepR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part1	:RadiationInfectOthersHands.part
		Part2	:RadiationHandElectrons.part
		Part3	:InfectBubbleDotsHands.part
		Part4	:InfectBubblesHands.part
		PMagnet	WepR
		Sound radiation3_loop 80 80 .34
		Lifespan 60
	End
End

Condition
	On 	Time
	Time 	19

	Event
		Type	Start
		At	WepL
		Part1	:RadiationLightHitBurst2.part
		Part2	:BurstRings.Part
		Lifespan 6
	End

	Event
		Type	Start
		At	WepR
		Part1	:RadiationLightHitBurst2.part
		Part2	:BurstRings.Part
		Lifespan 6
	End
End

#############################################################

Condition
	On 	Time
	Time 	22

	Event
		EName	Core
		Type	Local
		At	Chest
		Geom	PsionicRigging
		BhvrOverride
			PositionOffset	0.0 0.0 0.0
		End
	End

	Event
		EName 	Prime
		Type	Start
		At	Core
		Bhvr	CustomizeablePowers\RadiationBlast\Fastprojectile3.bhvr
		Part1	:BlastStreakGlowHead.part
		Part1	:LightHead.part
		Part1	:LightHead_Alpha.part
		Part3	:InfectBubbleRingssHandsStreak.part
		Part5	:InfectBubbleDotsHandsStreak.part
		Sound neutrino1 60 60 .83
		Magnet	Target
		LookAt	Target
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