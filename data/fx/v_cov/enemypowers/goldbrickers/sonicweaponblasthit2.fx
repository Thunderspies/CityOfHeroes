#########################################################
##	Fire_Ball
##
FxInfo

LifeSpan	30

Condition

	Event
		Type	local 
		At	Chest
		Part	:Mirage.part	
		Part	:MirageSteam.part
		Bhvr	behaviors/genericparticlefade.bhvr
		Sound grexplo2 70 70 .9

	End

	Event
		Type	local 
		At	Chest
		Part	:SonicHitflash.part
		Part	:SonicHitflashStar.part

		Part	:SonicHitRing.part
		Part	:SonicHitRingThin.part
		Sound ionblast1 70 70 .5

	End

End

End	


			