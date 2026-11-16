#############################################################
## PsionicLanceBlastWindUp.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	10

	Event
		EName	core
		Type	Local
		At	head
		Geom	PsionicRigging
		#bhvr	behaviors/PsionicsScale.bhvr
		Sound psionic7 88 88 .8
	End

	Event
		EName	core2
		Type	Local
		At	core
		Altpiv	1
		bhvr	behaviors/PsionicsScale2.bhvr
		geom	EyeBeams02
	End

	Event
		EName	core3
		Type	Local
		At	core
		Altpiv	3
		bhvr	behaviors/PsionicsScale.bhvr
		geom	EyeBeams01
	End

	Event
		EName	RingsAndParticles
		Type	Local
		At	core
		altpiv	2
		Part1	:PsionicBubbles.part
		Part2	:PsiGlow.part
	End

	Event
		EName	RingsAndParticles1
		Type	Local
		At	core
		altpiv	1
		Part1	:EnergyCoreLocal.part
	End

	Event
		EName	RingsAndParticles2
		Type	Local
		At	core
		altpiv	3
		Part1	:EnergyCoreLocal.part
	End

	Event
		EName	RingsAndParticles3
		Type	Local
		At	core
		altpiv	2
		Part1	:EnergyCore.part
	End
End

#############################################################

End