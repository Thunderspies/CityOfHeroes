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
		Part	:Mirage.part
		Part	:Mirage2.part
		Part	:MirageSteam.part
		Sound sonichit2 70 70 .75
	End

	Event
		Type	local
		At	Chest
		Part	:SonicHitflash.part
		Part	:SonicHitflashStar.part
		Part	:SonicHitRing.part
		Part	:SonicHitRingThin.part
	End
End

#############################################################

End