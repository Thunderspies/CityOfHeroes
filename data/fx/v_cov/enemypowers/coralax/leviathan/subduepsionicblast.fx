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
	Time	24


	Event
		EName	RotFwd
		Type	Local
		At	wepr
		bhvr	:ROTfwd.bhvr
	End

	Event
		EName	core
		Type	Local
		At	RotFwd
		Geom	PsionicRigging	
		bhvr	:PsionicsScaleSmall.bhvr
		Sound psionic6 88 88 .8
	End

	Event
		EName	RingsAndParticles1
		Type	Local
		At	wepr
#		altpiv	1
		bhvr	:ROTfwd.bhvr
		Part1	POWERS\Psionics\EnergyCoreLocal.part
		
	End

	Event
		EName	RingsAndParticles2
		Type	Local
		At	core
#		altpiv	3
		
		Part1	POWERS\Psionics\EnergyCoreLocal.part
		
	End

	Event
		EName	RingsAndParticles
		Type	Local
		At	core
#		altpiv	2
#		
		Part1	POWERS\Psionics\PsionicBubbles.part
		Part2	POWERS\Psionics\PsiGlow.part
	End

	Event
		EName	RingsAndParticles3
		Type	Local
		At	core
#		altpiv	2
		
		Part1	POWERS\Psionics\EnergyCore.part
		
	End

End
	
Condition
	On	Time
	Time	34

	Event
		EName	Flash1
		Type	Local
		At	core
#		altpiv	1
		
		Part2	POWERS\Psionics\EnergyCoreFlash.part
	End

	Event
		EName	Flash2
		Type	Local
		At	core
#		altpiv	2
		
		Part2	POWERS\Psionics\EnergyCoreFlash.part
	End

End

Condition
	On	Time
	Time	44

	Event
		EName	prime
		Type	Start
		At	Core
#		   Altpiv   2
		Geom	FrontFacingLocalPivot
		bhvr	behaviors/MedianProjectile3.bhvr
		Magnet	T_Head
		LookAt  T_Head
	End

	Event
		Type	Local
		At	Prime
#		  AltPiv 1
		bhvr	:RotFwdBig.bhvr
		Part1	POWERS\Psionics\BlastStreakSubdue.part
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
	Time 70
	
	Event
		EName	Core1
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
		EName	RingsAndParticles
		Type	Destroy
	End

End

Condition
	On Time
	Time 65

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
	Time 67

	Event
		EName	RingsAndParticles1
		Type	Destroy
	End

	Event
		EName	RingsAndParticles2
		Type	Destroy
	End

End

Condition
	On Time
	Time 65

	Event
		EName	RingsAndParticles3
		Type	Destroy
	End

End


End			