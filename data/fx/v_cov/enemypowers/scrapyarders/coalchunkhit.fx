#########################################################
##	iceweapon
########################################################
FxInfo

Input  
	Inpname	Target
End

LifeSpan	100

Condition
	On 	Time
	Time 	0

	Event
		EName 	peltedwithcoal
		Type	Local 
		At	Chest
		Part1	:CoalHitDust.part
		Part	:CoalHitFlash.part
		Part	:CoalHitRocks.part
		Part4	:CoalChunkBreak.part
		Sound coalhit 50 50 .99

	End

End


Condition
	On 	Time
	Time 	4

	Event
		EName 	peltedwithcoal2
		Type	Local 
		At	peltedwithcoal
		Part1	:CoalHitDust.part
		Part	:CoalHitRocks.part

	End

End


End
