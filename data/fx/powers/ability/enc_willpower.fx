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
		At	Hips
		
		Part1	:WillPower.part
		Part2	:WillPowerSmall.part
		Part3	:AbilityDots.part
		Part4	:AbilitySparklingDots.part
		Sound enrage 60 60 .88
	End

	Event
		Type	Local 
		At	Chest
		
		Part1	:WillPower.part
		Part2	:WillPowerSmall.part
		Part3	:AbilityDots.part
		Part4	:AbilitySparklingDots.part
	End

	Event
		Type	Local 
		At	LlegL
		
		Part1	:WillPower.part
		Part2	:WillPowerSmall.part
		Part3	:AbilityDots.part
		Part4	:AbilitySparklingDots.part
	End

End

End
##################################

