#########################################################
##	Fireweapon
########################################################
FxInfo

Input  
	Inpname	Head
End

Input  
	Inpname	Origin
End


########################################################

Condition
	On 	Time
	Time 	30


	Event
		Type	Local 
		At	Origin
		
	Sound stimulate1 40 40 .1
	End

End

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
		End

End

Condition
	On 	Time
	Time 	0


	Event
		Type	Local 
		At	Origin
		
	Sound Strength 40 40 .2
	End

End

End
##################################

