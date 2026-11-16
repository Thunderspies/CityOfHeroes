#########################################################
##	Fire_Ball
##
FxInfo

LifeSpan	20

Condition

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

	
	Event
		Type	local 
		At	Chest
		Part	:SonicHitRingThinBlack.part
		LifeSpan	10
	End


End

End	


			