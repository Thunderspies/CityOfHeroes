#############################################################
## HEADER
#############################################################

FxInfo

LifeSpan 90

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	HeadOffset
		Type	Local
		At	head
		Geom	PsionicRigging
	End
End

Condition
	On	Time
	Time	15

	Event
		Type	Local
		At	HeadOffset
		Altpiv	1
		geom	EyeBeams02
		bhvr	behaviors\PsionicsScale2.bhvr
		Sound Scare_01 88 88 .9
		Lifespan 30
	End

	Event
		Type	Local
		At	HeadOffset
		Altpiv	3
		bhvr	behaviors\PsionicsScale.bhvr
		geom	EyeBeams01
		Lifespan 30
	End

	Event
		Type	Local
		At	HeadOffset
		Altpiv	2
		Part1	:PsionicBubbles.part
		Part2	:PsiGlow.part
		Lifespan 30
	End

	Event
		Type	Local
		At	HeadOffset
		Altpiv	1
		Part1	:EnergyCoreLocal.part
		Lifespan 25
	End

	Event
		Type	Local
		At	HeadOffset
		Altpiv	3
		Part1	:EnergyCoreLocal.part
		Lifespan 25
	End


	Event
		Type	Local
		At	HeadOffset
		Altpiv	2
		Part1	:EnergyCore.part
		Lifespan 25
	End
End

Condition
	On	Time
	Time	20

	Event
		Type	Local
		At	HeadOffset
		Altpiv	1
		Part2	:EnergyCoreFlash.part
		Lifespan 20
	End

	Event
		Type	Local
		At	HeadOffset
		Altpiv	2
		Part2	:EnergyCoreFlash.part
		Lifespan 20
	End
End

#############################################################

End