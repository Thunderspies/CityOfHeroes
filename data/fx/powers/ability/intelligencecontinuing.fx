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
		
		Part1	:Intelligence.part
		Part2	:IntelligenceSmall.part
		Part3	:AbilityDots.part
		Part4	:AbilitySparklingDots.part
		sound	regen3 60 20 .7
	End

End



End
##################################

