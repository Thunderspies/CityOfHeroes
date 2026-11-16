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
		Type	local
		At	WepL
		Part	POWERS\RadiationControl\RadiationBlastHandRings.part
		Part	POWERS\RadiationControl\RadiationBlastHandBalls.part
		Sound blackhole3 80 80 0.75
		Lifespan 80
	End

	Event
		Type	local
		At	WepR
		Part	POWERS\RadiationControl\RadiationBlastHandRings.part
		Part	POWERS\RadiationControl\RadiationBlastHandBalls.part
		Sound blackhole3 80 80 0.75
		Lifespan 80
	End

	Event
		Type	Local
		At	WepR
		Part	POWERS\RadiationControl\RadiationInfectOthersHands.part
		Part	POWERS\RadiationControl\RadiationHandElectrons.part
		Part	POWERS\RadiationControl\InfectBubbleDotsHands.part
		Part	POWERS\RadiationControl\InfectBubblesHands.part
		PMagnet	WepR
		Sound radiation3_loop 80 80 0.3
		Lifespan 80
	End

	Event
		Type	Local
		At	WepL
		Part	POWERS\RadiationControl\RadiationInfectOthersHands.part
		Part	POWERS\RadiationControl\RadiationHandElectrons.part
		Part	POWERS\RadiationControl\InfectBubbleDotsHands.part
		Part	POWERS\RadiationControl\InfectBubblesHands.part
		PMagnet	WepL
		Lifespan 80
	End
End

Condition
	On	Time
	Time	100

	Event
		Type	Posit
		At	T_Chest
		BHVR	Behaviors\genericParticleFade.bhvr
		Part	POWERS\RadiationControl\RadiationLightHit.part
		Part	POWERS\RadiationControl\BurstRingsContinuing.Part
	End
End

#############################################################

End