#########################################################
##	cot_waterfall_med.fx
##
FxInfo
	
LifeSpan	200

#################################################################################
############################  Emitter points  ###################################
#################################################################################

Condition
	On	Time
	Time	0
	
	Event
		EName	EmitterPos
		Type	Local
		At	Belt
		
		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_MentalEmitter.bhvr
	End

	Event
		EName	core
		Type	Posit
		At	EmitterPos
			
		Sound psionic3 80 80 .9
	End

#################################################################################
################################  Blast Core  ###################################
#################################################################################

	Event
		EName	core2
		Type	Local
		At	core
		  
		geom	EyeBeams02
		bhvr	behaviors/PsionicsScale2.bhvr
	End

	Event
		EName	core3
		Type	Local
		At	core

		geom	EyeBeams01
		bhvr	behaviors/PsionicsScale.bhvr
	End

	Event
		EName	RingsAndParticles
		Type	Local
		At	core
		
		Part1	POWERS\Psionics\PsionicBubbles.part
		Part2	POWERS\Psionics\PsiGlow.part
	End

	Event
		EName	RingsAndParticles1
		Type	Local
		At	core
		
		Part1	POWERS\Psionics\EnergyCoreLocal.part
		
	End

	Event
		EName	RingsAndParticles2
		Type	Local
		At	core
		
		Part1	POWERS\Psionics\EnergyCoreLocal.part
		
	End

	Event
		EName	RingsAndParticles3
		Type	Local
		At	core
		
		Part1	POWERS\Psionics\EnergyCore.part
		
	End

End

#################################################################################
#########################  Blast Core Inner Rings  ##############################
#################################################################################	

Condition
	On	Time
	Time	40

	Event
		EName	Flash1
		Type	Local
		At	core
		
		Part2	POWERS\Psionics\EnergyCoreFlash.part
	End

	Event
		EName	Flash2
		Type	Local
		At	core
		
		Part2	POWERS\Psionics\EnergyCoreFlash.part
	End

End

#################################################################################
#############################  Blast Projectile  ################################
#################################################################################

Condition
	On	Time
	Time	15

	Event
		EName	prime
		Type	Start
		At	Core

		Geom	FrontFacingLocalPivot
		bhvr	POWERS\Psionics\Medianprojectile3.bhvr
		#bhvr	behaviors/MedianProjectile3.bhvr
		Magnet	T_Head
		LookAt  T_Head
	End

	Event
		Type	Local
		At	Prime

		Part1	POWERS\Psionics\BlastStreak.part
		Part2	POWERS\Psionics\BlastStreakGlowHead.part
		
	End
	
End

#################################################################################
#################################  K I L L A Z  #################################
#################################################################################

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