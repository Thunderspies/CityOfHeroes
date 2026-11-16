#########################################################
## Umbral Blast - Shadow Bolt
#########################################################

FxInfo

LifeSpan	200

## SOUND FX ######################################################

Condition
	On 	Time
	Time 	13
	Event
		EName	Sound1
		Type	Local
		At	WepR
		Sound WarshadeBlast1 80 80 .89
		LifeSpan 5
	End
End

## EYE FX ######################################################

Condition
	On	Time
	Time	0

	Event
		EName 	Eyes
		Type	Local 
		At	Root
		ChildFX POWERS\CosmicPowers\Nictus\ActivationEyes\ActivationEyes.fx
	End
End

## HANDS FX ######################################################

Condition
	On 	Time
	Time 	13
	Event
		EName	SwordFlash2
		Type	Local
		At	WepR
		Part1	:PeaceHandsFlash.part
		Part	:QuickBlastHandStreaks.part
		Part	:QuickBlastHandStreaksBlue.part
		Part	:QuickBlastHandGlow.part
		Part	:QuickBlastHandRing.part
		LifeSpan 5
	End
End

## PROJECTILE ######################################################

Condition
	On 	Time
	Time 	16

	Event
		EName 	Prime
		Type	Start 
		At	WepR
		Bhvr	:SmallBlastProjectile.bhvr
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		8
			PhysForcePower		100
			PhysForcePowerJitter	50
		End
		Magnet	Target
		LookAt	Target
		Part	:SmallBlastHeadCore.part
		Part	:SmallBlastHeadCore.part
		Part	:SmallBlastHeadCore.part
		Part	:SmallBlastHeadGlow.part
		Part	:SmallBlastSparks.part
		Part	:SmallBlastHeadStreaks.part
		Part	:WhiteHeadCore.part
		Part	:WhiteHeadCoreHead.part
		Part	:QuickBlastHeadStreaksBlue.part
		Part	:QuickBlastHeadStreaksPurple.part
		Part	:QuickBlastHeadStreaks.part
		Part	:QuickBlastHeadGlow.part
		Part	:QuickBlastHeadGlowPurple1.part
		Part	:QuickBlastHeadGlowBlue1.part
		Part	:QuickBlastHeadRing2.part
	End
End

Condition
	On 	Time
	Time 	16

	Event
		EName 	Prime1
		Type	Start 
		At	WepR
		Bhvr	:SmallBlastProjectile.bhvr
		Magnet	Target
		LookAt	Target
		Part	:SmallBlastHeadBlackCore.part
		Part	:SmallBlastHeadBlackCore.part
		Part	:SmallBlastHeadBlackCore.part
		Part	:SmallBlastHeadBlackCore.part
		Part	:SmallNictusEnergyBlack.part
		Part	:SmallNictusEnergy2.part
		Part	:SmallNictusEnergyPurple2.part
	End
End

## CLEAN UP ######################################################

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

########################################################

End			