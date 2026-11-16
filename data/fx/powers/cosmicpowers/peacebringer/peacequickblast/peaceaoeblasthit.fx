#########################################################
##	Fire_Ball
##
FxInfo

LifeSpan	200

Condition
	On 	Time
	Time 	0
	Event
		EName	SwordFlash2
		Type	Local
		At	root
		BHVR	Behaviors/RootToChestAdjustment.bhvr

		Part1	:BlastRing.part
		Part1	:BlastThinRing.part
		LifeSpan 4
	End

	Event
		EName	SwordFlash2
		Type	Local
		At	Chest
		Part1	:BlastFlash.part
		Part	:BlastFlashTendrils.part
		Part	:QuickBlastHitStreaks.part
		Part	:PeaceBlastHitSparks.part
		LifeSpan 10
	End

End


End	


			