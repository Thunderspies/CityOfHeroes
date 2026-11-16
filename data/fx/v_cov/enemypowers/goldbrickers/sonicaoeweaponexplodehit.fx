#########################################################
##	Fire_Ball
##
FxInfo

LifeSpan	60

Condition

	Event
		Type	Start 
		At	Chest
		Part	:Mirage.part	
		Part	:MirageSteam.part
		Bhvr	behaviors/genericparticlefade.bhvr
		Sound sonichit2 70 70 .75

	End

	Event
		Type	Start 
		At	Chest
		Part	:SonicHitflash.part
		Part	:SonicHitflashStar.part

		Part	:SonicAOEHitRing.part
		Part	:SonicAOEHitRingThin.part

	End

End

End	


			