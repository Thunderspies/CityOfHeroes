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
		
		
		Part	:HealFire.part

		Part	:BrightCore.part
		Part1	:BlueRings.part
		Part2	:BlueHealing02.part
		sound	dimensiondrop3 80 30 .7
	End

End

Condition
	On 	Time
	Time 	45
	
	Event
		
		Type	start 
		At	Chest
		Part1	:BrightCoreFinalBlue.part
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
		Part1	:BlueRing1.part
		LifeSpan	14
	End
	
	
End

Condition

	Event
		EName	SoulSwirls
		Type	Local
		At	chest
		
		childFx	:BoostSwirlsBlue.fx

		LifeSpan 45
	End
	
	Event
		EName	SoulSwirls
		Type	Local
		At	chest
		
		childFx	:BoostSwirlsBlue2.fx

		LifeSpan 45
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
		Part1	:BlueRing1.part
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

