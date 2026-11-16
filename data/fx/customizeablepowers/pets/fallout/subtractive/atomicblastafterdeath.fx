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
		Part	CustomizeablePowers\RadiationBlast\Subtractive\RadiationBlastHandRings.part
		Part	CustomizeablePowers\RadiationBlast\Subtractive\RadiationBlastHandBalls.part
		Sound blackhole3 80 80 0.75
		Lifespan 80
	End

	Event
		Type	local
		At	WepR
		Part	CustomizeablePowers\RadiationBlast\Subtractive\RadiationBlastHandRings.part
		Part	CustomizeablePowers\RadiationBlast\Subtractive\RadiationBlastHandBalls.part
		Sound blackhole3 80 80 0.75
		Lifespan 80
	End

	Event
		Type	Local
		At	WepR
		Part	CustomizeablePowers\RadiationBlast\Subtractive\RadiationInfectOthersHands.part
		Part	CustomizeablePowers\RadiationBlast\Subtractive\RadiationHandElectrons.part
		Part	CustomizeablePowers\RadiationBlast\Subtractive\InfectBubbleDotsHands.part
		Part	CustomizeablePowers\RadiationBlast\Subtractive\InfectBubblesHands.part
		PMagnet	WepR
		Sound radiation3_loop 80 80 0.3
		Lifespan 80
	End

	Event
		Type	Local
		At	WepL
		Part	CustomizeablePowers\RadiationBlast\Subtractive\RadiationInfectOthersHands.part
		Part	CustomizeablePowers\RadiationBlast\Subtractive\RadiationHandElectrons.part
		Part	CustomizeablePowers\RadiationBlast\Subtractive\InfectBubbleDotsHands.part
		Part	CustomizeablePowers\RadiationBlast\Subtractive\InfectBubblesHands.part
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
		Part	CustomizeablePowers\RadiationBlast\Subtractive\RadiationLightHit.part
		Part	CustomizeablePowers\RadiationEmission\BurstRingsContinuing.Part
	End
End

#############################################################

End