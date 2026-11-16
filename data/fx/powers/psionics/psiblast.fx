#########################################################
##	cot_waterfall_med.fx
##
FxInfo
	
LifeSpan	200

Input  
	InpName	Origin
End

Input  
	InpName	Target
End

Input  
	InpName	head
End

Input  
	InpName	T_head
End

###########################################################
Condition
	On	Time
	Time	10

	Event
		EName	core
		Type	Local
		At	head
		Geom	PsionicRigging		
		#bhvr	behaviors/PsionicsScale.bhvr
		Sound scramble1 80 80 .7
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
		EName	RingsAndParticles1
		Type	Local
		At	core
		altpiv	1
		
		Part1	:EnergyCoreLocal.part
		
		Lifespan   70
	End


	Event
		EName	RingsAndParticles2
		Type	Local
		At	core
		altpiv	3
		
		Part1	:EnergyCoreLocal.part
		
		Lifespan   70
	End

	Event
		EName	RingsAndParticles
		Type	Local
		At	core
		altpiv	2
		
		Part1	:PsionicBubbles.part
		Part2	:PsiGlow.part
		Lifespan   70
	End

	Event
		EName	RingsAndParticles3
		Type	Local
		At	core
		altpiv	2
		
		Part1	:EnergyCore.part
		
		Lifespan   70
	End

End
	
Condition
	On	Time
	Time	20

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
	Time	40

	Event
		EName	prime
		Type	Start
		At	Core
		   Altpiv   2
		Geom	FrontFacingLocalPivot
		#bhvr	behaviors/MedianProjectile3.bhvr
		bhvr	:MedianProjectile3.bhvr
		Magnet	T_Head
		LookAt  T_Head
	End

	Event
		Type	Local
		At	Prime
		  AltPiv 1
		Part1	:BlastStreak.part
		Part2	:ScrambleThoughtsTenticles.part
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
	Time 100
	Event
		EName	Flash1
		Type	Destroy
	End

	Event
		EName	Flash2
		Type	Destroy
	End

	Event
		EName	RingsAndParticles
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
End

Condition
	On Time
	Time 90
	Event
		EName	RingsAndParticles3
		Type	Destroy
	End
End

End			