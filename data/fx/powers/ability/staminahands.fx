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
		At	WepL
		
		Part1	:Stamina.part
		Part2	:StaminaSmall.part
		Part3	:AbilityDots.part
		Part4	:AbilitySparklingDots.part
	End

	Event
		EName	Right
		Type	Local 
		At	WepR
		
		Part1	:Stamina.part
		Part2	:StaminaSmall.part
		Part3	:AbilityDots.part
		Part4	:AbilitySparklingDots.part
	End
End

End
##################################

