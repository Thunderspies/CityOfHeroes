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
		At	WepR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\RadiationBlast\RadiationInfectOthersHands.part
		Part2	CustomizeablePowers\RadiationBlast\RadiationHandElectrons.part
		Part3	CustomizeablePowers\RadiationBlast\InfectBubbleDotsHands.part
		Part4	CustomizeablePowers\RadiationBlast\InfectBubblesHands.part
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
		At	WepR
		Part1	CustomizeablePowers\RadiationBlast\RadiationLightHitBurst2.part
		Part2	CustomizeablePowers\RadiationBlast\BurstRings.Part
		Lifespan 6
	End
End

#############################################################

Condition
	On 	Time
	Time 	17

	Event
		EName 	Prime
		Type	Start
		At	WepR
		Bhvr	CustomizeablePowers\RadiationBlast\Fastprojectile3.bhvr
		Part1	CustomizeablePowers\RadiationBlast\BlastStreakGlowHead.part
		Part1	CustomizeablePowers\RadiationBlast\LightHead.part
		Part3	CustomizeablePowers\RadiationBlast\InfectBubbleRingssHandsStreak.part
		Part5	CustomizeablePowers\RadiationBlast\InfectBubbleDotsHandsStreak.part
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