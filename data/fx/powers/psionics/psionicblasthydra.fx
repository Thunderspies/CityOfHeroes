#########################################################
##	cot_waterfall_med.fx
##
FxInfo

LifeSpan 200

Condition
	Event
		EName	looker
		Type	Posit
		At	chest
		LookAt	Target
	End

	Event
		EName	focus
		Type	Local
		At	looker
		Bhvr	POWERS/mentalpowers/HydraMindControl.bhvr
		Sound mental4 80 80 .75
	End
End

###########################################################
Condition
	On	Time
	Time	30

	Event
		EName	core
		Type	Local
		At	focus
		Geom	PsionicRigging		
		#bhvr	behaviors/PsionicsScale.bhvr
		Sound psionic3 80 80 .9
	End

	Event
		EName	core2
		Type	Local
		At	core
		  Altpiv   1
		geom	EyeBeams02
		bhvr	behaviors/PsionicsScale2.bhvr
	End

	Event
		EName	core3
		Type	Local
		At	core
		   Altpiv   3
		geom	EyeBeams01
		bhvr	behaviors/PsionicsScale.bhvr
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
	
Condition
	On	Time
	Time	40

	Event
		EName	Flash1
		Type	Local
		At	core
		altpiv	1
		
		Part2	:EnergyCoreFlash.part
	End

	Event
		EName	Flash2
		Type	Local
		At	core
		altpiv	2
		
		Part2	:EnergyCoreFlash.part
	End

End

Condition
	On	Time
	Time	60

	Event
		EName	prime
		Type	Start
		At	Core
		   Altpiv   2
		Geom	FrontFacingLocalPivot
		bhvr	behaviors/MedianProjectile3.bhvr
		Magnet	T_Head
		LookAt  T_Head
	End

	Event
		Type	Local
		At	Prime
		  AltPiv 1
		Part1	:BlastStreak.part
		Part2	:BlastStreakGlowHead.part
		
	End
	
End

Condition
	On	PrimeHit
		
	Event
		EName	Prime
		Type	Destroy
	End
End

Condition
	On Time
	Time 112
	
	Event
		EName	Core
		Type	Destroy
	End

	Event
		EName	Core2
		Type	Destroy
	End

	Event
		EName	Core3
		Type	Destroy
	End

	Event
		EName	Flash1
		Type	Destroy
	End

	Event
		EName	Flash2
		Type	Destroy
	End

	
End

Condition
	On Time
	Time 146

	Event
		EName	RingsAndParticles
		Type	Destroy
	End

End

Condition
	On Time
	Time 108
	Event
		EName	RingsAndParticles3
		Type	Destroy
	End

	Event
		EName	RingsAndParticles1
		Type	Destroy
	End

	Event
		EName	RingsAndParticles2
		Type	Destroy
	End
	
End

End			