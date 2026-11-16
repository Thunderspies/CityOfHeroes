#############################################################
## AtomicBlastAfterDeath.fx
#############################################################

FxInfo

Lifespan 190

#############################################################

Condition
	On 	Time
	Time 	15

	Event
		Type	Local
		At	WepR
		BHVR	behaviors\genericParticleFade.bhvr
		Part1	:RadiationInfectOthersHands.part
		Part2	:RadiationHandElectrons.part
		Part3	:InfectBubbleDotsHands.part
		Part4	:InfectBubblesHands.part
		Part1	:RadiationBlastHandRings.part
		Part2	:RadiationBlastHandBalls.part
		PMagnet	WepR
		Sound radiation3_loop 80 80 .3
		Lifespan 80
	End

	Event
		Type	Local
		At	WepL
		BHVR	behaviors\genericParticleFade.bhvr
		Part1	:RadiationInfectOthersHands.part
		Part2	:RadiationHandElectrons.part
		Part3	:InfectBubbleDotsHands.part
		Part4	:InfectBubblesHands.part
		Part1	:RadiationBlastHandRings.part
		Part2	:RadiationBlastHandBalls.part
		Sound blackhole3 80 80 .75
		PMagnet	WepL
		Lifespan 80
	End
End

#############################################################

Condition
	On	Time
	Time	100

	Event
		Ename	LightRays
		Type	Posit
		At	T_Chest
		BHVR	behaviors\genericParticleFade.bhvr
		part1	:RadiationLightHit.part
		Part2	:BurstRingsContinuing.Part
	End
End

#############################################################

End