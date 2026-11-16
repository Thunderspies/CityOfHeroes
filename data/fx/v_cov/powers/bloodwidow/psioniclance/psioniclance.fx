#############################################################
## Blood Widow "Psionic Lance" FX
#############################################################

FxInfo

LifeSpan 200

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	core
		Type	Local
		At	head
		Geom	PsionicRigging
		Lifespan 32
	End
End

#############################################################

#Condition
#	On	Time
#	Time	0
#
#	Event
#		EName	core2
#		Type	Local
#		At	core
#		Altpiv	1
#		geom	EyeBeams02
#		bhvr	behaviors/PsionicsScale2.bhvr
#		Lifespan 32
#	End
#
#	Event
#		EName	core3
#		Type	Local
#		At	core
#		Altpiv	3
#		geom	EyeBeams01
#		bhvr	behaviors/PsionicsScale.bhvr
#		Lifespan 32
#	End
#
#	Event
#		EName	RingsAndParticles
#		Type	Local
#		At	core
#		Altpiv	2
#		Part1	POWERS\Psionics\PsionicBubbles.part
#		Part2	POWERS\Psionics\PsiGlow.part
#	End
#End
#
#Condition
#	On	Time
#	Time	0
#
#	Event
#		EName	RingsAndParticles1
#		Type	Local
#		At	core
#		Altpiv	1
#		Part1	POWERS\Psionics\EnergyCoreLocal.part
#		Lifespan 28
#	End
#
#	Event
#		EName	RingsAndParticles2
#		Type	Local
#		At	core
#		Altpiv	3
#		Part1	POWERS\Psionics\EnergyCoreLocal.part
#		Lifespan 28
#	End
#
#	Event
#		EName	RingsAndParticles3
#		Type	Local
#		At	core
#		Altpiv	2
#		Part1	POWERS\Psionics\EnergyCore.part
#		Lifespan 28
#	End
#End
#
#Condition
#	On	Time
#	Time	10
#
#	Event
#		EName	Flash1
#		Type	Local
#		At	core
#		Altpiv	1
#		Part2	POWERS\Psionics\EnergyCoreFlash.part
#		Lifespan 22
#	End
#
#	Event
#		EName	Flash2
#		Type	Local
#		At	core
#		Altpiv	2
#		Part2	POWERS\Psionics\EnergyCoreFlash.part
#		Lifespan 22
#	End
#End

## PROJECTILE ###########################################################

Condition
	On	Time
	Time	10

	Event
		EName	prime
		Type	Start
		At	Core
		Altpiv	2
		Geom	FrontFacingLocalPivot
		bhvr	behaviors/PsionicLance.bhvr
		Magnet	T_Head
		LookAt  T_Head
		Sound psionic4 88 88 .8
	End

	Event
		Type	Local
		At	Prime
		Altpiv	1
		Geom	PsiLance
		Part1	POWERS\Psionics\BlastStreakSmall.part
		Part2	POWERS\Psionics\BlastStreakGlow.part
	End
End

#############################################################

Condition
	On	PrimeHit

	Event
		EName	Prime
		Type	Destroy
	End
End

#############################################################

End