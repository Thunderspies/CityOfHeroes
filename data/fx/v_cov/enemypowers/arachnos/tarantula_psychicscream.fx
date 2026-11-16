#########################################################
##	FireRing
##
FxInfo

LifeSpan	70	

#################################################################################
############################  Emitter points  ###################################
#################################################################################

Condition
	On 	Time
	Time 	13

	Event
		EName	Emitter
		Type	Local
		At	Belt
		
		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_PsyScrEmitter.bhvr
#		Geom	Testing_Target
		Sound mental11 80 80 .8
	End

#################################################################################
############################  Core Glow and Rings  ##############################
#################################################################################

	Event
		EName	core3
		Type	Local
		At	Emitter
		
		part1	POWERS/Psionics/CryRays.part
		part2	POWERS/Psionics/PsionicGlow2.part	
	End
	
	Event
		EName	core2
		Type	Local
		At	Emitter
		
		part1	POWERS/Psionics/PsionicRingsSmall.part
		part2	POWERS/Psionics/PsionicRingzSmall.part
	End
End

Condition
	On 	Time
	Time	60
	
	Event
		Ename 	core3
		Type	Destroy
	End
End


Condition
	On 	Time
	Time	44
	
	Event
		Ename 	core2
		Type	Destroy
	End

End

#################################################################################
##################################  Ground Ring  ################################
#################################################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	OrbMist
		Type	Local 
		At	root
		geom	FX_RuneStreak
		bhvr	behaviors/Rune_spinFastSmall.bhvr
				
	End

	Event
		EName 	OrbStreak
		Type	Local 
		At	root
		geom	FX_RuneStreak
		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_PsyScrGrndSpin.bhvr		
	End
End

Condition
	On 	Time
	Time 	10

	Event
		EName 	Orb
		Type	Local 
		At	OrbStreak
		altpiv  1
		
		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_PsyScrGrndOffset.bhvr
		Part1	POWERS/Psionics/GlowOrbStar2Warm.part
		part3	POWERS/Psionics/GlowOrbTailWarm2.part
			
		
	End

End

#################################################################################
##################################  Blast01  ####################################
#################################################################################

Condition
	On	Time
	Time	20

	Event
		EName	corez
		Type	Local
		At	Emitter
		Geom	PsionicRigging	
		
	End

	Event
		EName	prime
		Type	Start
		At	Corez
		   Altpiv   2
		Geom	FrontFacingLocalPivot
		bhvr	behaviors/PsionicWave.bhvr
		Magnet	target
		LookAt  target
		
	End

	Event
		ENAME	Geom
		Type	Local
		At	Prime
		  AltPiv 1
		bhvr	behaviors/PushProjectile2.bhvr
		Geom	PsiTeleWave
		

	End

	Event
		ENAME	Ringsz
		Type	Local
		At	Prime
		  AltPiv 1
		bhvr	behaviors/PushProjectile3.bhvr
		Part1	POWERS/Psionics/BlastStreakWidened.part

	End
	
End

Condition
	On 	Time
	Time	25
	
	Event
		Ename 	Geom
		Type	Destroy
	End


End

Condition
	On 	Time
	Time	31
	
	Event
		Ename 	Ringsz
		Type	Destroy
	End


End

#################################################################################
##################################  Blast02  ####################################
#################################################################################

Condition
	On	Time
	Time	30

	Event
		EName	coreza
		Type	Local
		At	Emitter
		Geom	PsionicRigging	
		
	End

	Event
		EName	prime1
		Type	Start
		At	Corez
		   Altpiv   2
		Geom	FrontFacingLocalPivot
		bhvr	behaviors/PsionicWave.bhvr
		Magnet	target
		LookAt  target
		
	End

	Event
		ENAME	Geoma
		Type	Local
		At	Prime1
		  AltPiv 1
		bhvr	behaviors/PushProjectile2.bhvr
		Geom	PsiTeleWave
		

	End

	Event
		ENAME	Ringsza
		Type	Local
		At	Prime1
		  AltPiv 1
		bhvr	behaviors/PushProjectile3.bhvr
		Part1	POWERS/Psionics/BlastStreakWidened.part

	End
	
End

Condition
	On 	Time
	Time	35
	
	Event
		Ename 	Geoma
		Type	Destroy
	End


End

Condition
	On 	Time
	Time	41
	
	Event
		Ename 	Ringsza
		Type	Destroy
	End


End

#################################################################################
##################################  k i l l z  ##################################
#################################################################################


Condition
	On 	Time
	Time 	50

	Event
		EName 	OrbMist
		Type	Destroy 
				
	End

End

Condition
	On 	Time
	Time 	95

	Event
		EName 	Ringsz
		Type	Destroy 
				
	End

End
	

Condition
	On 	Time
	Time 	53

	Event
		EName 	Orb
		Type	Destroy 
				
	End

	
End

End			