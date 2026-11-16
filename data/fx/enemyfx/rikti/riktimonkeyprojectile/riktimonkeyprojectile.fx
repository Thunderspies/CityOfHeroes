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
	Time	0

	Event
		EName	core
		Type	Local
		At	head
		Geom	PsionicRigging		
		Sound psionic4 88 88 .8
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
	Time	10

	Event
		EName	prime
		Type	Start
		At	Core
		   Altpiv   2
		Geom	FrontFacingLocalPivot
		bhvr	:PsionicLance.bhvr
		Magnet	T_Head
		LookAt  T_Head
	End

	Event
		Type	Local
		At	Prime
		  AltPiv 1
		Geom	PsiLance
		Part1	:BlastStreakSmall.part
		Part2	:BlastStreakGlow.part
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
	Time 32
	
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
	Time 66

	Event
		EName	RingsAndParticles
		Type	Destroy
	End

End

Condition
	On Time
	Time 28
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