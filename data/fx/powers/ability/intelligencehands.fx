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
	Time 	0


	Event
		EName	left
		Type	Local 
		At	WepL
		
		Part1	:Intelligence.part
		Part2	:IntelligenceSmall.part
		Part3	:AbilityDots.part
		Part4	:AbilitySparklingDots.part
	End

	Event
		EName	Right
		Type	Local 
		At	WepR
		
		Part1	:Intelligence.part
		Part2	:IntelligenceSmall.part
		Part3	:AbilityDots.part
		Part4	:AbilitySparklingDots.part
	End
End

End
##################################

