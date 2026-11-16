#############################################################
## StandardBlastHit.fx
#############################################################

FxInfo

Input
	Inpname	Hips
End

LifeSpan 20

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	local
		At	Chest
		Part	CustomizeablePowers\SonicAttack\Mirage.part
		Part	CustomizeablePowers\SonicAttack\Mirage2.part
		Part	CustomizeablePowers\SonicAttack\MirageSteam.part
		#Sound sonichit2 70 70 .75
	End

	Event
		Type	local
		At	Chest
		Part	CustomizeablePowers\SonicAttack\SonicHitflash.part
		Part	CustomizeablePowers\SonicAttack\SonicHitflashStar.part
		Part	CustomizeablePowers\SonicAttack\SonicHitRing.part
		Part	CustomizeablePowers\SonicAttack\SonicHitRingThin.part
	End
End

#############################################################

End