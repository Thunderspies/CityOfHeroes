#########################################################
## Dark Nova Bolt
#########################################################

FxInfo

LifeSpan	200

## SOUND FX #######################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	origin
		Sound warshadeeyes3 60 60 .72
	End
End

Condition
	On 	Time
	Time 	18
	Event
		EName	SwordFlash2
		Type	Local
		At	head
		Sound WarshadeBlast1 80 80 .89
	End
End

## ATTACK FX #######################################################

Condition
	On 	Time
	Time 	18
	Event
		EName	SwordFlash2
		Type	Local
		At	head
		BHVR	Behaviors/GenericParticleFade.bhvr
		Part1	:PeaceHandsFlash.part
		Part	:QuickBlastHandStreaks.part
		Part	:QuickBlastHandStreaksBlue.part
		Part	:QuickBlastHandGlow.part
		Part	:QuickBlastHandRing.part
		LifeSpan 5
	End
End

## PROJECTILE #######################################################

Condition
	On 	Time
	Time 	18
	Event
		EName 	Prime
		Type	Start
		At	head
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
	Time 	18
	Event
		EName 	Prime1
		Type	Start
		At	head
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

## PROJECTILE #######################################################

End