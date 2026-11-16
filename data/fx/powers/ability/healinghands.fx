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

########################################################

Condition
	On 	Time
	Time 	0


	Event
		EName	Left
		Type	Local 
		At	WepL
		
		Part1	:Stamina2.part
		Part2	:StaminaSmall2.part
		Part3	:AbilityDots.part
		Part4	:AbilitySparklingDots.part
		Sound heal4 70 70 .55
	End

	Event
		EName	Right
		Type	Local 
		At	WepR
		
		Part1	:Stamina2.part
		Part2	:StaminaSmall2.part
		Part3	:AbilityDots.part
		Part4	:AbilitySparklingDots.part
	End
End


Condition
	On 	Time
	Time 	55

	Event
		EName	all
		Type	Destroy
		
	End

End


End
##################################

