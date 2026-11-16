#########################################################
##	Fireweapon
########################################################
FxInfo

Input  
	Inpname	wepR
End

Input  
	Inpname	wepL
End

LifeSpan	50

########################################################

Condition
	On 	Time
	Time 	0


	Event
		EName	Left
		Type	Local 
		At	wepL
		
		Part1	:WillPower.part
		Part2	:WillPowerSmall.part
		Part3	:AbilityDots.part
		Part4	:AbilitySparklingDots.part
	End

	Event
		EName	Right
		Type	Local 
		At	wepR
		
		Part1	:WillPower.part
		Part2	:WillPowerSmall.part
		Part3	:AbilityDots.part
		Part4	:AbilitySparklingDots.part
	End

End

End
##################################

