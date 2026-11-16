#########################################################
##	Fire_Ball
#########################################################

FxInfo

LifeSpan 200

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
	Time 	28

	Event
		EName	SwordFlash2
		Type	Local
		At	head
		Sound WarshadeQuick 80 80 .93
	End
End

## ATTACK FX #######################################################

Condition
	On 	Time
	Time 	28

	Event
		EName	SwordFlash2
		Type	Local
		At	head
		Part1	:PeaceHandsFlash.part
		Part	:QuickBlastHandStreaks.part
		Part	:QuickBlastHandGlow.part
		Part	:QuickBlastHandRing.part
		Part	:QuickBlastHandStreaksBlue.part
		LifeSpan 10
	End
End

## PROJECTILE #######################################################

Condition
	On 	Time
	Time 	28

	Event
		EName 	Prime
		Type	Start
		At	head
		Bhvr	:QuickBlastProjectile.bhvr
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		10
			PhysForcePower		100
			PhysForcePowerJitter	50
		End
		Part	:QuickBlastHeadCore.part
		Part	:SolidBlackCore.part
		Part	:SolidBlackCore.part
		Part	:SolidBlackCore.part
		Part	:SolidBlackCore.part
		Part	:QuickBlastNictusEnergy2.part
		Part	:QuickBlastNictusEnergyPurple.part
		Part	:QuickBlastNictusEnergyPurple2.part
		Part	:PeaceBlastSparks.part
		Magnet	Target
		LookAt	Target
	End
End

Condition
	On 	Time
	Time 	28

	Event
		EName 	Prime2
		Type	Start
		At	head
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

## AOE FX #######################################################


Condition
	On	PrimeHit

	Event
		Type	Local
		At	T_root
		Sound WarShadeAORHit 110 110 .93
	End

	Event
		Type	Local
		At	T_root
		BHVR	Behaviors/RootToChestAdjustment.bhvr
		Part1	:BlastRing.part
		LifeSpan 4
	End

	Event
		Type	Local
		At	T_root
		BHVR	Behaviors/RootToChestAdjustment.bhvr
		Part1	:BlastThinRing.part
		LifeSpan 5
	End

	Event
		Ename	ExplosiveForce
		Type	Start
		At	T_root
		BHVR	Behaviors/RootToChestAdjustment.bhvr
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		25
			PhysForcePower		450
			PhysForcePowerJitter	50
		End
		Lifespan 1
	End

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

#########################################################

End