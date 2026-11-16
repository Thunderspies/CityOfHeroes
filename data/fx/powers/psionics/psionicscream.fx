#########################################################
##	FireRing
##
FxInfo

LifeSpan	70	

Input  
	InpName	Origin
End

Input  
	InpName	Target
End

Input  
	InpName	root
End

Input  
	Inpname	head
End




######################
##Head effect
##########################
Condition
	On 	Time
	Time 	13

	Event
		EName	core
		Type	Local
		At	head
		Geom	LocalPivot01
		Sound mental11 80 80 .8
		
	End

	Event
		EName	core3
		Type	Local
		At	core
		  Altpiv   4
		
		part1	:CryRays.part
		part2	:PsionicGlow2.part
				
	
	End
	
	Event
		EName	core2
		Type	Local
		At	core
		  Altpiv   4
		
		part1	:PsionicRingsSmall.part
		part2	:PsionicRingzSmall.part
		
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

#########################################################

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
		bhvr	behaviors/Psionic_Spin.bhvr
				
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
		
		Part1	:GlowOrbStar2Warm.part
		part3	:GlowOrbTailWarm2.part
			
		
	End

End

##########################################################################

Condition
	On	Time
	Time	20

	Event
		EName	corez
		Type	Local
		At	head
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
		Part1	:BlastStreakWidened.part

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

###############################################################################

Condition
	On	Time
	Time	30

	Event
		EName	coreza
		Type	Local
		At	head
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
		Part1	:BlastStreakWidened.part

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

##############################################################################


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