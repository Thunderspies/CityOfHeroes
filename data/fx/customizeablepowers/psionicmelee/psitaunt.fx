#
FxInfo

	LifeSpan 150

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	EmitterOffset
		Type	PositOnly
		At	Head
		LookAt	Target
	End

	Event
		EName	Emitter
		Type	Local
		At	EmitterOffset
		BhvrOverride
			PYRRotate	-15 0 0
			PositionOffset	  0 .2 0
		End
	End
End

#############################################################

Condition
	On	Time
	Time	20

	Event
		EName	Sound
		Type	Local
		At	Head
		Sound scramble1 80 80 .7
	End
End

Condition
	On	Time
	Time	10

	Event
		EName	Core
		Type	Local
		At	Emitter
		Geom	PsionicRigging
		Lifespan 70
	End

	Event
		Type	Local
		At	core
		altpiv	1
		Bhvr 	Behaviors/GenericParticleFade.bhvr
		Part1	:EnergyCoreLocal.part
		Part2	:EnergyCoreFlash.part
		Part 	:InfuseLight2.part
		Lifespan 60
	End

	Event
		Type	Local
		At	core
		altpiv	3
		Bhvr 	Behaviors/GenericParticleFade.bhvr
		Part1	:EnergyCoreLocal.part
		Lifespan 60
	End

	Event
		Type	Local
		At	core
		altpiv	2
		Bhvr 	Behaviors/GenericParticleFade.bhvr
		Part2	:PsiGlow.part
		Part1	:EnergyCore.part
		Part1	:EnergyCore_Blur.part
		Lifespan 60
	End
End

#############################################################

End
