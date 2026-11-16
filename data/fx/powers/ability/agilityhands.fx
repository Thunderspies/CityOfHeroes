#########################################################
##	Fireweapon
########################################################
FxInfo

Input  
	Inpname	WepR
End

Input  
	Inpname	WepL
End

LifeSpan	50

########################################################

Condition
	On 	Time
	Time 	10


	Event
		EName	Left
		Type	Local 
		At	WepL
		
		Part1	:Agility.part
		Part2	:AgilitySmall2.part
		Part3	:AbilityDots.part
		Part4	:AbilitySparklingDots.part

	End

	Event
		EName	Right
		Type	Local 
		At	WepR
		
		Part1	:Agility.part
		Part2	:AgilitySmall2.part
		Part3	:AbilityDots.part
		Part4	:AbilitySparklingDots.part

	End
End

Condition
	On 	Time
	Time 	35

	Event
	
	EName	Left
	Type	Destroy

	End

	Event
	
	EName	Right
	Type	Destroy

	End

End

End
##################################

