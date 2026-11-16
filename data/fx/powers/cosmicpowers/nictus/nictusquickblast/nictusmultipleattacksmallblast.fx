#########################################################
## Umbral Blast - Gravatic Emenation
#########################################################

FxInfo

LifeSpan	200

## PROJECTILE (Target=Caster) #######################################################

## PROJECTILE (Target=Caster) #######################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Start 
		At	T_WepR
		Bhvr	:QuickBlastProjectile4.bhvr
		Magnet	Chest
		LookAt	Chest
		Part	:SmallBlastHeadCore.part
		Part	:SmallBlastHeadCore.part
		Part	:SmallBlastHeadCore.part
		Part	:SmallBlastHeadGlow.part
		Part	:SmallBlastSparks.part
		Part	:SmallBlastHeadStreaks.part
		Part	:WhiteHeadCore.part
		Part	:WhiteHeadCoreHead.part
		Part	:QuickBlastHeadStreaksBlue.part
		#Sound	WarshadeQuick 80 70 .93
	End
End

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime1
		Type	Start 
		At	T_WepR
		Bhvr	:QuickBlastProjectile4.bhvr
		Magnet	Chest
		LookAt	Chest
		Part	:SmallBlastHeadBlackCore.part
		Part	:SmallBlastHeadBlackCore.part
		Part	:SmallBlastHeadBlackCore.part
		Part	:SmallBlastHeadBlackCore.part
		Part	:SmallNictusEnergyBlack.part
		Part	:SmallNictusEnergy2.part
		Part	:SmallNictusEnergyPurple2.part
		Part	:QuickBlastHeadStreaksPurple.part
		Part	:QuickBlastHeadStreaks.part
		Part	:QuickBlastHeadGlow.part
		Part	:QuickBlastHeadGlowPurple1.part
		Part	:QuickBlastHeadGlowBlue1.part
		Part	:QuickBlastHeadRing2.part
	End
End

## HIT FX #######################################################

Condition
	On	PrimeHit
		
	Event
		Type	Local
		At	Chest
		
		Part	:NictusSunFlareHitStreaks.part
		Part	:BlastFlash.part
		Part	:BlastFlashTendrils.part
		Part	:QuickBlastHitStreaks.part
		Part	:QuickBlastHitStreaksBlue.part
		Part	:PeaceBlastHitSparks.part
		Part	:BlastFlashTendrilsPurple.part
		Part	:BlastFlashTendrilsBlue.part
		LifeSpan	10
	End
	
	Event
		Ename	Prime
		Type	Destroy
	End
		
	Event
		Ename	ExplosiveForce
		Type	Start
		At	Chest
		
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		8
			PhysForcePower		250
			PhysForcePowerJitter	100
		End
		Lifespan	1
	End
End

## CLEAN UP #######################################################

Condition
	On	Prime1Hit
			
	Event
		Ename	Prime1
		Type	Destroy
	End
End

#########################################################

End		