#############################################################
## BlasterFormNictusQuickBlast.fx
#############################################################

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
	Time 	18
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
	Time 	18
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
	Time 	18

	Event
		EName 	Prime
		Type	Start
		At	head
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

## CLEAN UP #######################################################

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

#########################################################

End