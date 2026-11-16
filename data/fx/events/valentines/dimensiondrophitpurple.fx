#########################################################
##	Fireweapon
########################################################
FxInfo

Input  
	Inpname	chest
End

Input  
	Inpname	root
End

########################################################

LifeSpan	50

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

	Event
		EName	SoulSwirls
		Type	Local
		At	chest
		
		childFx	:BoostSwirlsPurple.fx

		LifeSpan 45
	End
	
	Event
		EName	SoulSwirls
		Type	Local
		At	chest
		
		childFx	:BoostSwirlsPurple2.fx

		LifeSpan 45
	End

End

Condition
	On 	Time
	Time 	45
	
	Event
		
		Type	start 
		At	Chest
		Part1	:BrightCoreFinalPurple.part
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
		Part1	:PurpleRing1.part
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
		Part1	:PurpleRing1.part
		LifeSpan	14
	End


End

Condition
	On 	Time
	Time 	45
	
	Event
		EName	dummy
		Type	Destroy
		
	End

	Event
		EName	dummy3
		Type	Destroy
		
	End

	Event
		EName	dummy4
		Type	Destroy
		
	End
	
End



End
##################################

