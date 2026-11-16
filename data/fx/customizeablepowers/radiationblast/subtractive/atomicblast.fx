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
		Type	Local
		At	WepR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part1	:RadiationInfectOthersHands.part
		Part2	:RadiationHandElectrons.part
		Part3	:InfectBubbleDotsHands.part
		Part4	:InfectBubblesHands.part
		Part1	:RadiationBlastHandRings.part
		Part2	:RadiationBlastHandBalls.part
		PMagnet	WepR
		Sound radiation3_loop 80 80 .4
		Lifespan 65
	End

	Event
		Type	Local
		At	WepL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part1	:RadiationInfectOthersHands.part
		Part2	:RadiationHandElectrons.part
		Part3	:InfectBubbleDotsHands.part
		Part4	:InfectBubblesHands.part
		Part1	:RadiationBlastHandRings.part
		Part2	:RadiationBlastHandBalls.part
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
		Bhvr	Behaviors\CameraShake01.bhvr
		part1	:RadiationflashThinRing.part
		part2	:Radiationflash2.part
		part3	:Radiationsmoke.part
		part3	:Radiationsmoke_Alpha.part

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
		Part1	:InfectOthersStemSmoke.part
		Part1	:InfectOthersStemSmoke_Alpha.part

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
		Part1	:InfectOthersPlumeSmoke.part
		Part1	:InfectOthersPlumeSmoke_Alpha.part

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
		Part1	:InfectOthersRing.part
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
		Part1	:InfectOthersRing.part
		Lifespan 2
	End
End

#############################################################

End