#########################################################
##	template

FxInfo


Input
	Inpname Chest
End

LifeSpan 60

Condition
	On Time 
	Time 5
	
	Event
		EName	Prime
		Type 	Local
		At	Chest
		Part1	:Pistolflash.part
		Part2	:PistolflashStar.part
		geom	CrossBowBolt
		bhvr	Behaviors/genericparticlefade.bhvr
	End
End


End	

	