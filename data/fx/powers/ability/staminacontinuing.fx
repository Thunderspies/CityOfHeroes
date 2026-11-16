#########################################################
##	Fireweapon
########################################################
FxInfo

Input  
	Inpname	Head
End

########################################################

Condition
	On 	Time
	Time 	0


	Event
		Type	Local 
		At	Head
		
		Part1	:Stamina.part
		Part2	:StaminaSmall.part
		Part3	:AbilityDots.part
		Part4	:AbilitySparklingDots.part
		Sound mabuildup2 100.0 100.0 .75
	End

End

End
##################################

