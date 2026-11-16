#########################################################
##	cot_waterfall_med.fx
##
FxInfo
	
LifeSpan	200

###########################################################
Condition
	On	Time
	Time	15

	Event
		EName	core
		Type	Local
		At	head
		Geom	PsionicRigging		
		Sound psionic4 88 88 .8
	End

	Event
		EName	RingsAndParticles
		Type	start
		At	core
		altpiv	2
		
		Part1	ENEMYFX\Rikti\RiktiMonkeyProjectile\PsionicBubbles.part
		Part2	ENEMYFX\Rikti\RiktiMonkeyProjectile\PsiGlow.part
		LifeSpan	20
	End

	Event
		EName	RingsAndParticles1
		Type	start
		At	core
		altpiv	1
		
		Part1	ENEMYFX\Rikti\RiktiMonkeyProjectile\EnergyCoreLocal.part
		
		LifeSpan	5
	End

	Event
		EName	RingsAndParticles2
		Type	start
		At	core
		altpiv	3
		
		Part1	ENEMYFX\Rikti\RiktiMonkeyProjectile\EnergyCoreLocal.part
		LifeSpan	5
		
	End

	Event
		EName	RingsAndParticles3
		Type	start
		At	core
		altpiv	2
		
		Part1	ENEMYFX\Rikti\RiktiMonkeyProjectile\EnergyCore.part
		LifeSpan	10
		
	End

End
	
Condition
	On	Time
	Time	23

	Event
		EName	prime
		Type	Start
		At	Core
		   Altpiv   2
		Geom	FrontFacingLocalPivot
		bhvr	ENEMYFX\Rikti\RiktiMonkeyProjectile\PsionicLance.bhvr
		Magnet	T_Head
		LookAt  T_Head
	End

	Event
		Type	Local
		At	Prime
		  AltPiv 1
		Geom	PsiLance
		Part1	ENEMYFX\Rikti\RiktiMonkeyProjectile\BlastStreakSmall.part
		Part2	ENEMYFX\Rikti\RiktiMonkeyProjectile\BlastStreakGlow.part
	End
	
End

Condition
	On	PrimeHit
		
	Event
		EName	Prime
		Type	Destroy
	End
End

End			