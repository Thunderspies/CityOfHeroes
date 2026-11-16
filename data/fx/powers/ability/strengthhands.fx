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
		EName	Left
		Type	Local 
		At	WepR
		
		Part1	:Strength.part
		Part2	:StrengthSmall.part
		Part3	:AbilityDots.part
		Part4	:AbilitySparklingDots.part
	End

	Event
		EName	Right
		Type	Local 
		At	WepL
		
		Part1	:Strength.part
		Part2	:StrengthSmall.part
		Part3	:AbilityDots.part
		Part4	:AbilitySparklingDots.part
	End
End

End
##################################

