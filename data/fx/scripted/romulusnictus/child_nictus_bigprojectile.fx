#########################################################
## Dark Nova Blast
#########################################################

FxInfo

## SOUND FX #######################################################

#Condition
#	On	Time
#	Time	0
#
#	Event
#		Type	Local
#		At	origin
#		Sound	warshadeeyes3 60 50 .72
#	End
#End
#
#Condition
#	On 	Time
#	Time 	27
#	Event
#		EName	SwordFlash2
#		Type	Local
#		At	head
#		Sound	WarshadeQuick 80 70 .93
#	End
#End

## ATTACK FX #######################################################

Condition
	On 	Time
	Time 	0
	Event
		EName	SwordFlash2
		Type	Local
		At	Origin
		Part1	POWERS\CosmicPowers\Nictus\NictusQuickBlast\PeaceHandsFlash.part
		Part	POWERS\CosmicPowers\Nictus\NictusQuickBlast\QuickBlastHandStreaks.part
		Part	POWERS\CosmicPowers\Nictus\NictusQuickBlast\QuickBlastHandGlow.part
		Part	POWERS\CosmicPowers\Nictus\NictusQuickBlast\QuickBlastHandRing.part
		Part	POWERS\CosmicPowers\Nictus\NictusQuickBlast\QuickBlastHandStreaksBlue.part
		LifeSpan 10
	End
End

## PROJECTILE #######################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Start
		At	Origin
		Bhvr	POWERS\CosmicPowers\Nictus\NictusQuickBlast\QuickBlastProjectile.bhvr
		Part	POWERS\CosmicPowers\Nictus\NictusQuickBlast\QuickBlastHeadCore.part
		Part	POWERS\CosmicPowers\Nictus\NictusQuickBlast\SolidBlackCore.part
		Part	POWERS\CosmicPowers\Nictus\NictusQuickBlast\SolidBlackCore.part
		Part	POWERS\CosmicPowers\Nictus\NictusQuickBlast\SolidBlackCore.part
		Part	POWERS\CosmicPowers\Nictus\NictusQuickBlast\SolidBlackCore.part
		Part	POWERS\CosmicPowers\Nictus\NictusQuickBlast\QuickBlastNictusEnergy2.part
		Part	POWERS\CosmicPowers\Nictus\NictusQuickBlast\QuickBlastNictusEnergyPurple.part
		Part	POWERS\CosmicPowers\Nictus\NictusQuickBlast\QuickBlastNictusEnergyPurple2.part
		Part	POWERS\CosmicPowers\Nictus\NictusQuickBlast\PeaceBlastSparks.part
		Magnet	T_Chest
		LookAt	T_Chest
	End

	Event
		EName 	Prime2
		Type	Start
		At	Origin
		Bhvr	POWERS\CosmicPowers\Nictus\NictusQuickBlast\QuickBlastProjectile.bhvr
		Part	POWERS\CosmicPowers\Nictus\NictusQuickBlast\PurpleHaze.part
		Part	POWERS\CosmicPowers\Nictus\NictusQuickBlast\PurpleHaze.part
		Part	POWERS\CosmicPowers\Nictus\NictusQuickBlast\WhiteHeadCore.part
		Part	POWERS\CosmicPowers\Nictus\NictusQuickBlast\WhiteHeadCoreHead.part
		Part	POWERS\CosmicPowers\Nictus\NictusQuickBlast\QuickBlastHeadStreaksBlue.part
		Part	POWERS\CosmicPowers\Nictus\NictusQuickBlast\QuickBlastHeadStreaksPurple.part
		Part	POWERS\CosmicPowers\Nictus\NictusQuickBlast\QuickBlastHeadStreaks.part
		Part	POWERS\CosmicPowers\Nictus\NictusQuickBlast\QuickBlastHeadGlow.part
		Part	POWERS\CosmicPowers\Nictus\NictusQuickBlast\QuickBlastHeadGlowPurple.part
		Part	POWERS\CosmicPowers\Nictus\NictusQuickBlast\QuickBlastHeadGlowBlue.part
		Part	POWERS\CosmicPowers\Nictus\NictusQuickBlast\QuickBlastHeadRing.part
		Magnet	T_Chest
		LookAt	T_Chest
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