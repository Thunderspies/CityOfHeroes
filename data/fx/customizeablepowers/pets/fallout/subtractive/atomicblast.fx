#############################################################
## AtomicBlast.fx
#############################################################

FxInfo

Lifespan 120

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	local
		At	WepR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\RadiationBlast\Subtractive\RadiationBlastHandRings.part
		Part2	CustomizeablePowers\RadiationBlast\Subtractive\RadiationBlastHandBalls.part
		Lifespan 65
	End

	Event
		Type	Local
		At	WepR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\RadiationBlast\Subtractive\RadiationInfectOthersHands.part
		Part2	CustomizeablePowers\RadiationBlast\Subtractive\RadiationHandElectrons.part
		Part3	CustomizeablePowers\RadiationBlast\Subtractive\InfectBubbleDotsHands.part
		Part4	CustomizeablePowers\RadiationBlast\Subtractive\InfectBubblesHands.part
		PMagnet	WepR
		Sound radiation3_loop 80 80 .4
		Lifespan 65
	End

	Event
		Type	local
		At	WepL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\RadiationBlast\Subtractive\RadiationBlastHandRings.part
		Part2	CustomizeablePowers\RadiationBlast\Subtractive\RadiationBlastHandBalls.part
		Lifespan 65
	End

	Event
		Type	Local
		At	WepL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\RadiationBlast\Subtractive\RadiationInfectOthersHands.part
		Part2	CustomizeablePowers\RadiationBlast\Subtractive\RadiationHandElectrons.part
		Part3	CustomizeablePowers\RadiationBlast\Subtractive\InfectBubbleDotsHands.part
		Part4	CustomizeablePowers\RadiationBlast\Subtractive\InfectBubblesHands.part
		PMagnet	WepL
		Lifespan 65
	End
End

#############################################################

Condition
	On	Time
	Time	29

	Event
		Type	start
		At	Root
		Sound atomicblast3 100 100 1.0
	End
End

Condition
	On	Time
	Time	42

	Event
		Type	start
		At	Root
		Bhvr	behaviors\CameraShake01.bhvr
		part1	CustomizeablePowers\RadiationBlast\Subtractive\RadiationflashThinRing.part
		part2	CustomizeablePowers\RadiationBlast\Subtractive\Radiationflash2.part
		part3	CustomizeablePowers\RadiationBlast\Subtractive\Radiationsmoke.part
	End
End

#############################################################

Condition
	On	Time
	Time	45

	Event
		EName	StemSmoke
		Type	start
		At	Root
		Part1	CustomizeablePowers\RadiationBlast\Subtractive\InfectOthersStemSmoke.part
		Lifespan 30
	End
End

Condition
	On	Time
	Time	55

	Event
		EName	PlumeSmoke
		Type	start
		At	Root
		BhvrOverride
			PositionOffset		0.0 11.0 0.0
		End
		Part1	CustomizeablePowers\RadiationBlast\Subtractive\InfectOthersPlumeSmoke.part
		Lifespan 28
	End
End

#############################################################

Condition
	On	Time
	Time	41

	Event
		EName	RingsOffset01
		Type	start
		At	Root
		bhvr    Behaviors\UpWardsMotion.bhvr
	End

	Event
		Type	Local
		At	RingsOffset01
		Part1	CustomizeablePowers\RadiationBlast\Subtractive\InfectOthersRing.part
		Lifespan 2
	End
End

Condition
	On	Time
	Time	61

	Event
		EName	RingsOffset02
		Type	start
		At	Root
		bhvr    Behaviors\UpWardsMotion2.bhvr
	End

	Event
		Type	Local
		At	RingsOffset02
		Part1	CustomizeablePowers\RadiationBlast\Subtractive\InfectOthersRing.part
		Lifespan 2
	End
End

#############################################################

End