#########################################################
## Umbral Blast - Shadow Bolt
#########################################################

FxInfo

LifeSpan	200

## SOUND FX ######################################################

Condition
	On 	Time
	Time 	18
	Event
		EName	SwordFlash2
		Type	Local
		At	WepR
		Sound WarshadeQuick 80 80 .93
		LifeSpan 10
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

## HANDS ######################################################

Condition
	On 	Time
	Time 	18
	Event
		EName	SwordFlash2
		Type	Local
		At	WepR
		Part1	:PeaceHandsFlash.part
		Part	:QuickBlastHandStreaks.part
		Part	:QuickBlastHandGlow.part
		Part	:QuickBlastHandRing.part
		Part	:QuickBlastHandStreaksBlue.part
		LifeSpan 10
	End

	Event
		EName	SwordFlash2
		Type	Local
		At	WepL
		Part1	:PeaceHandsFlash.part
		Part	:QuickBlastHandStreaks.part
		Part	:QuickBlastHandGlow.part
		Part	:QuickBlastHandRing.part
		Part	:QuickBlastHandStreaksBlue.part
		LifeSpan 10
	End
End

## PROJECTILE ######################################################

Condition
	On 	Time
	Time 	18

	Event
		EName 	Prime
		Type	Start 
		At	mystic
		Bhvr	:QuickBlastProjectile.bhvr
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		8
			PhysForcePower		100
			PhysForcePowerJitter	50
		End
		Magnet	Target
		LookAt	Target
		Part	:QuickBlastHeadCore.part
		Part	:SolidBlackCore.part
		Part	:SolidBlackCore.part
		Part	:SolidBlackCore.part
		Part	:SolidBlackCore.part		
		Part	:QuickBlastNictusEnergy2.part	
		Part	:QuickBlastNictusEnergyPurple.part
		Part	:QuickBlastNictusEnergyPurple2.part
		Part	:PeaceBlastSparks.part
	End

	Event
		EName 	Prime2
		Type	Start 
		At	mystic
		Bhvr	:QuickBlastProjectile.bhvr
		Magnet	Target
		LookAt	Target
		Part	:PurpleHaze.part
		Part	:PurpleHaze.part
		Part	:WhiteHeadCore.part
		Part	:WhiteHeadCoreHead.part
		Part	:QuickBlastHeadStreaksBlue.part
		Part	:QuickBlastHeadStreaksPurple.part
		Part	:QuickBlastHeadStreaks.part
		Part	:QuickBlastHeadGlow.part
		Part	:QuickBlastHeadGlowPurple.part
		Part	:QuickBlastHeadGlowBlue.part
		Part	:QuickBlastHeadRing.part
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
	On	Prime2Hit
			
	Event
		Ename	Prime2
		Type	Destroy
	End
End

########################################################

End		