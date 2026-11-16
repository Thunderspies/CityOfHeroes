#########################################################
##	Fireweapon
########################################################
FxInfo

Input  
	Inpname	Head
End

lifespan	60
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
		Sound regen3 100.0 100.0 .75
	End

End

End
##################################

