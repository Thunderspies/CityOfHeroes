#########################################################
##	FireRing
##
FxInfo

LifeSpan	100	

#################################################################################
############################  Emitter points  ###################################
#################################################################################

Condition
	On 	Time
	Time 	30

	Event
		EName	RotFwd
		Type	Local
		At	wepr
		bhvr	:ROTfwdsmall.bhvr
	End

	Event
		EName	core
		Type	Local
		At	RotFwd

#		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_PsyScrEmitter.bhvr
#		Geom	Testing_Target
		Sound mentalwail2 80 80 .75
		
	End

#################################################################################
##############################  Mouth Cone Glow  ################################
#################################################################################

	Event
		EName	core3
		Type	Local
		At	RotFWD
		
		part1	POWERS\Psionics\CryRays.part
		part2	POWERS\Psionics\PsionicGlow2.part
#		bhvr	:ROTfwd.bhvr
		Lifespan 60
	End
End

#################################################################################
#########################  Core Small Glowing Rings  ############################
#################################################################################

#Condition
#	On 	Time
#	Time 	34
#
#	Event
#		EName	core2
#		Type	Local
#		At	core
#		
#		part1	POWERS\Psionics\PsionicRingsSmall.part
#		part2	POWERS\Psionics\PsionicRingzSmall.part
#		Lifespan 34
#	End
#
#End

#################################################################################
#########################  Ground Vertical Emitters  ############################
#################################################################################

Condition
	On 	Time
	Time 	33

	Event
		EName 	OrbStreak
		Type	Local 
		At	RotFwd
		geom	FX_RuneStreak
		bhvr	behaviors/Psionic_Spin.bhvr			
	End

	Event
		EName 	Orb
		Type	Local 
		At	RotFwd #OrbStreak
		altpiv  1
		BhvrOverride
			PositionOffset    0.0 -5.0 0.0
		End

#		bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_PsyScrGrndOffset.bhvr
#		Part1	:GlowOrbStar2Warm.part
		part3	:GlowOrbTailWarm2.part
		Lifespan 63
	End
End

#################################################################################
#################################  Rising Rings  ################################
#################################################################################

Condition
	On 	Time
	Time 	33
	
	Event
		EName	RingParent
		Type	Local
		At	RotFwd

		Bhvr	V_COV\EnemyPowers\Arachnos\Tarantula_B_PsyWailScale.bhvr
	End

	Event
		EName 	AOERings
		Type	Local 
		At	RotFwd
		
		part1	POWERS\Psionics\PsionicCryRing.Part
		part2	POWERS\Psionics\PsionicCryRing2.Part
		part3	POWERS\Psionics\GlowRings.part
		Lifespan 25
	End

	Event
		EName 	AOERings2
		Type	Local 
		At	RingParent
				
		part4	POWERS\Psionics\DeepScanGlow.part
		Lifespan 28
	End
End	

End			