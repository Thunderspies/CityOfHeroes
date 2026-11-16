#########################################################
## Dark Nova Emenation (Projectiles)
#########################################################

FxInfo

LifeSpan	200

## SOUND FX #######################################################

#Condition
#	On	Time
#	Time	0
#
#	Event	
#		Type	Local
#		At	chest
#		Sound	warshadeeyes3 60 50 .72
#	End
#End
#
#
#Condition
#	On 	Time
#	Time 	0
#
#	Event
#		EName 	Prime
#		Type	Start 
#		At	T_head
#		Sound	Sootthrow 80 70 .8
#	End
#End

## PROJECTILE #######################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Start 
		At	T_head
		Bhvr	:QuickBlastProjectile4.bhvr
		Magnet	Chest
		LookAt	Chest
		Part	:SmallBlastHeadCore.part
		##Part	:SmallBlastHeadCore.part
		##Part	:SmallBlastHeadCore.part-
		Part	:SmallBlastHeadGlow.part
		#Part	:SmallBlastSparks.part
		#Part	:SmallBlastHeadStreaks.part
		Part	:WhiteHeadCore.part
		#Part	:WhiteHeadCoreHead.part
		#Part	:QuickBlastHeadStreaksBlue.part
	End
End

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime1
		Type	Start 
		At	T_head
		Bhvr	:QuickBlastProjectile4.bhvr
		Magnet	Chest
		LookAt	Chest
		Part	:SmallBlastHeadBlackCore.part
		##Part	:SmallBlastHeadBlackCore.part
		##Part	:SmallBlastHeadBlackCore.part
		##Part	:SmallBlastHeadBlackCore.part
		Part	:SmallNictusEnergyBlack.part
		Part	:SmallNictusEnergy2.part
		##Part	:SmallNictusEnergyPurple2.part
		Part	:QuickBlastHeadStreaksPurple.part
		Part	:QuickBlastHeadStreaks.part
		##Part	:QuickBlastHeadGlow.part
		Part	:QuickBlastHeadGlowPurple1.part
		##Part	:QuickBlastHeadGlowBlue1.part
		Part	:QuickBlastHeadRing2.part
	End
End

## HIT FX #######################################################

#Condition
#	On	PrimeHit
#		
#	Event
#		Type	Local
#		At	Chest
#		
#		Part	:NictusSunFlareHitStreaks.part
#		Part	:BlastFlash.part
#		Part	:BlastFlashTendrils.part
#		Part	:QuickBlastHitStreaks.part
#		Part	:QuickBlastHitStreaksBlue.part
#		Part	:PeaceBlastHitSparks.part
#		Part	:BlastFlashTendrilsPurple.part
#		Part	:BlastFlashTendrilsBlue.part
#		LifeSpan 10
#	End
#End

## PHYSICS FORCES #######################################################

#Condition
#	On	PrimeHit
#
#	Event
#		Ename	ExplosiveForce
#		Type	Start
#		At	Chest
#		
#		BhvrOverride
#			PhysForceType		Out
#			PhysForceRadius		8
#			PhysForcePower		250
#			PhysForcePowerJitter	100
#		End
#		Lifespan	1
#	End
#End

## CLEAN UP #######################################################

Condition
	On	PrimeHit
		
	Event
		Ename	Prime
		Type	Destroy
	End
End

Condition
	On	Prime1Hit
			
	Event
		Ename	Prime1
		Type	Destroy
	End

End

#########################################################

End		