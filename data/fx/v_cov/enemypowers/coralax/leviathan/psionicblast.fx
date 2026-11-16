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
		EName	offs
		Type	Local
		At	hair

		bhvr	:ROTfwd.bhvr
	end

	Event
		EName	core
		Type	Local
		At	offs
#		Geom	PsionicRigging		
		bhvr	:PsionicsScale.bhvr
		Sound psionic3 80 80 .9
	End
#
#	Event
#		EName	core2
#		Type	Local
#		At	Core
##		  #Altpiv   1
##		geom	EyeBeams02
#		bhvr	:PsionicsScale.bhvr
#	End
#
#	Event
#		EName	core3
#		Type	Local
#		At	Core
#		   #Altpiv   3
##		geom	EyeBeams01
#		bhvr	:PsionicsScale.bhvr
#	End

	Event
		EName	RingsAndParticles
		Type	Local
		At	Core
		#Altpiv	2
		
		Part1	POWERS\Psionics\PsionicBubbles.part
		Part2	POWERS\Psionics\PsiGlow.part
	End

	Event
		EName	RingsAndParticles1
		Type	Local
		At	Core
		#Altpiv	1
		
		Part1	POWERS\Psionics\EnergyCoreLocal.part
		
	End

	Event
		EName	RingsAndParticles2
		Type	Local
		At	Core
		#Altpiv	3
		
		Part1	POWERS\Psionics\EnergyCoreLocal.part
		
	End

	Event
		EName	RingsAndParticles3
		Type	Local
		At	Core
		#Altpiv	2
		
		Part1	POWERS\Psionics\EnergyCore.part
		
	End

End
	
Condition
	On	Time
	Time	27

	Event
		EName	Flash1
		Type	Local
		At	Core
		#Altpiv	1
		
		Part2	POWERS\Psionics\EnergyCoreFlash.part
	End

	Event
		EName	Flash2
		Type	Local
		At	Core
		#Altpiv	2
		
		Part2	POWERS\Psionics\EnergyCoreFlash.part
	End

End

Condition
	On	Time
	Time	42

	Event
		EName	prime
		Type	Start
		At	Core
		   #Altpiv   2
		Geom	FrontFacingLocalPivot
		bhvr	POWERS\Psionics\Medianprojectile3.bhvr
		#bhvr	behaviors/MedianProjectile3.bhvr
		Magnet	T_Head
		LookAt  T_Head
	End

	Event
		Type	Local
		At	Prime
		  #Altpiv 1
		Part1	POWERS\Psionics\BlastStreak.part
		Part2	POWERS\Psionics\BlastStreakGlowHead.part
		bhvr	:ROTfwd.bhvr
		
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
	Time 70

	Event
		EName	RingsAndParticles
		Type	Destroy
	End

End

Condition
	On Time
	Time 70
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