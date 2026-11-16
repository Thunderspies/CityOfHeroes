#########################################################
## Dark Nova Bolt
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
#		At	origin
#		Sound	warshadeeyes3 60 50 .72
#	End
#End
#
#Condition
#	On 	Time
#	Time 	16
#	Event
#		EName	SwordFlash2
#		Type	Local
#		At	head
#		Sound	WarshadeBlast1 80 70 .89
#	End
#End

## ATTACK FX #######################################################

Condition
	On 	Time
	Time 	16
	Event
		EName	SwordFlash2
		Type	Local
		At	Origin
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part1	POWERS\CosmicPowers\Nictus\NictusQuickBlast\PeaceHandsFlash.part
		Part	POWERS\CosmicPowers\Nictus\NictusQuickBlast\QuickBlastHandStreaks.part
		Part	POWERS\CosmicPowers\Nictus\NictusQuickBlast\QuickBlastHandStreaksBlue.part
		Part	POWERS\CosmicPowers\Nictus\NictusQuickBlast\QuickBlastHandGlow.part
		Part	POWERS\CosmicPowers\Nictus\NictusQuickBlast\QuickBlastHandRing.part
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
		At	Origin
		Bhvr	POWERS\CosmicPowers\Nictus\NictusQuickBlast\SmallBlastProjectile.bhvr
		Part	POWERS\CosmicPowers\Nictus\NictusQuickBlast\SmallBlastHeadCore.part
		Part	POWERS\CosmicPowers\Nictus\NictusQuickBlast\SmallBlastHeadCore.part
		Part	POWERS\CosmicPowers\Nictus\NictusQuickBlast\SmallBlastHeadCore.part
		Part	POWERS\CosmicPowers\Nictus\NictusQuickBlast\SmallBlastHeadGlow.part
		Part	POWERS\CosmicPowers\Nictus\NictusQuickBlast\SmallBlastSparks.part
		Part	POWERS\CosmicPowers\Nictus\NictusQuickBlast\SmallBlastHeadStreaks.part
		Part	POWERS\CosmicPowers\Nictus\NictusQuickBlast\WhiteHeadCore.part
		Part	POWERS\CosmicPowers\Nictus\NictusQuickBlast\WhiteHeadCoreHead.part
		Part	POWERS\CosmicPowers\Nictus\NictusQuickBlast\QuickBlastHeadStreaksBlue.part
		Part	POWERS\CosmicPowers\Nictus\NictusQuickBlast\QuickBlastHeadStreaksPurple.part
		Part	POWERS\CosmicPowers\Nictus\NictusQuickBlast\QuickBlastHeadStreaks.part
		Part	POWERS\CosmicPowers\Nictus\NictusQuickBlast\QuickBlastHeadGlow.part
		Part	POWERS\CosmicPowers\Nictus\NictusQuickBlast\QuickBlastHeadGlowPurple1.part
		Part	POWERS\CosmicPowers\Nictus\NictusQuickBlast\QuickBlastHeadGlowBlue1.part
		Part	POWERS\CosmicPowers\Nictus\NictusQuickBlast\QuickBlastHeadRing2.part
		Magnet	T_Chest
		LookAt	T_Chest
	End
End

Condition
	On 	Time
	Time 	18
	Event
		EName 	Prime1
		Type	Start
		At	Origin
		Bhvr	POWERS\CosmicPowers\Nictus\NictusQuickBlast\SmallBlastProjectile.bhvr
		Part	POWERS\CosmicPowers\Nictus\NictusQuickBlast\SmallBlastHeadBlackCore.part
		Part	POWERS\CosmicPowers\Nictus\NictusQuickBlast\SmallBlastHeadBlackCore.part
		Part	POWERS\CosmicPowers\Nictus\NictusQuickBlast\SmallBlastHeadBlackCore.part
		Part	POWERS\CosmicPowers\Nictus\NictusQuickBlast\SmallBlastHeadBlackCore.part
		Part	POWERS\CosmicPowers\Nictus\NictusQuickBlast\SmallNictusEnergyBlack.part
		Part	POWERS\CosmicPowers\Nictus\NictusQuickBlast\SmallNictusEnergy2.part
		Part	POWERS\CosmicPowers\Nictus\NictusQuickBlast\SmallNictusEnergyPurple2.part
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
	On	Prime1Hit

	Event
		Ename	Prime1
		Type	Destroy
	End

End

#############################################################

End