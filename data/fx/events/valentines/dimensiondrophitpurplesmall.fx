#########################################################
##	Fireweapon
########################################################
FxInfo
########################################################

LifeSpan	20

Condition
	On 	Time
	Time 	0
	
	Event
		EName	dummy
		Type	Local 
		At	chest
		
		Part	:BrightCore.part
		Part1	:PurpleRings.part
		Part2	:PurpleHealing02.part
		sound	dimensiondrop2 80 30 .7
	End

End

Condition
	On 	cycle
	Time 	14
	Chance	1


	Event
		EName	dummy3
		Type	Local 
		At	chest
		Part1	:PurpleRing.part
		LifeSpan	14
	End
	
	
End

Condition
	On 	cycle
	Time 	28
	Chance	1

	Event
		EName	dummy4
		Type	Local 
		At	chest
		Part1	:PurpleRing.part
		LifeSpan	14
	End


End

End
##################################

