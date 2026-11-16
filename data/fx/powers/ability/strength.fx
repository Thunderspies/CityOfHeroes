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
		
		Part1	:Strength.part
		Part2	:StrengthSmall.part
		Part3	:AbilityDots.part
		Part4	:AbilitySparklingDots.part
		Sound Strength 100 100 .8
	End

End

End
##################################

