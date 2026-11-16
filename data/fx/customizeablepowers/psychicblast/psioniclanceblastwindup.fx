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
		#bhvr	behaviors\PsionicsScale.bhvr
		Geom	PsionicRigging
		Sound psionic7 88 88 .8
	End

	Event
		EName	core2
		Type	Local
		At	core
		Altpiv	1
		bhvr	CustomizeablePowers\PsychicBlast\PsionicsScale2.bhvr
		BhvrOverride
			Alpha		25
			TintGeom	1
		End
		geom	Tintable_EyeBeams02
	End

	Event
		EName	core3
		Type	Local
		At	core
		Altpiv	3
		bhvr	CustomizeablePowers\PsychicBlast\PsionicsScale.bhvr
		BhvrOverride
			Alpha		25
			TintGeom	1
		End
		geom	Tintable_EyeBeams01
	End

	Event
		Type	Local
		At	core
		altpiv	2
		Part1	CustomizeablePowers\PsychicBlast\PsionicBubbles.part
		Part2	CustomizeablePowers\PsychicBlast\PsiGlow.part
	End

	Event
		Type	Local
		At	core
		altpiv	1
		Part1	CustomizeablePowers\PsychicBlast\EnergyCoreLocal.part

	End

	Event
		Type	Local
		At	core
		altpiv	3
		Part1	CustomizeablePowers\PsychicBlast\EnergyCoreLocal.part

	End

	Event
		Type	Local
		At	core
		altpiv	2
		Part1	CustomizeablePowers\PsychicBlast\EnergyCore.part
	End
End

#############################################################

End