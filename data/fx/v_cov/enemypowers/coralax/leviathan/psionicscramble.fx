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
		Type	local
		At	EmitterPos
		bhvr	:PsionicsScaleSmall.bhvr
		Sound scramble1 80 80 .7
		Lifespan 90
	End

	End

Condition
	On	Time
	Time	32

	Event
		EName	EmitterPos
		Type	Local
		At	wepR
		
		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_WillEmitter.bhvr
	End

	Event
		EName	core
		Type	local
		At	EmitterPos
		bhvr	:PsionicsScaleSmall.bhvr
		Sound scramble1 80 80 .7
		Lifespan 90
	End

#################################################################################
################################  Blast Rings  ##################################
#################################################################################

	Event
		EName	RingsAndParticles1
		Type	Local
		At	core
		
		Part1	POWERS\Psionics\EnergyCoreLocal.part
		Lifespan   70
	End


	Event
		EName	RingsAndParticles2
		Type	Local
		At	core
		
		Part1	POWERS\Psionics\EnergyCoreLocal.part
		Lifespan   70
	End

	Event
		EName	RingsAndParticles
		Type	Local
		At	core
		
		Part1	POWERS\Psionics\PsionicBubbles.part
		Part2	POWERS\Psionics\PsiGlow.part
		Lifespan   70
	End

	Event
		EName	RingsAndParticles3
		Type	Local
		At	core
		
		Part1	POWERS\Psionics\EnergyCore.part
		Lifespan   70
	End

End

#################################################################################
############################  Inner Rings Flash  ################################
#################################################################################
	
Condition
	On	Time
	Time	31

	Event
		EName	Flash1
		Type	Local
		At	core
		
		Part2	POWERS\Psionics\EnergyCoreFlash.part
		lifespan 80
	End

	Event
		EName	Flash2
		Type	Local
		At	core
		
		Part2	POWERS\Psionics\EnergyCoreFlash.part
		lifespan 80
	End
End

#################################################################################
############################  Light Projectile  #################################
#################################################################################
	
Condition
	On	Time
	Time	71

	Event
		EName	prime
		Type	Start
		At	Core

		Geom	FrontFacingLocalPivot
		#bhvr	behaviors/MedianProjectile3.bhvr
		bhvr	POWERS\Psionics\MedianProjectile3.bhvr
		Magnet	T_Head
		LookAt  T_Head
	End

	Event
		Type	Local
		At	Prime
#		BhvrOverride  ####### gives a different result than bhvr file
#		PyrRotate		-90 0 0
#		PositionOffset		0 0 0
#		Scale			3 3 3
#		End

		bhvr	:PsionicsScaleRotSmall.bhvr
		Part1	POWERS\Psionics\BlastStreak.part
		Part2	:ScrambleThoughtsTenticles.part
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

End			