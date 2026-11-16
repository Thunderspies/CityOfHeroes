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
		
		Part1	CustomizeablePowers\Empathy\Strength.part
		Part2	CustomizeablePowers\Empathy\StrengthSmall.part
		Part3	CustomizeablePowers\Empathy\AbilityDots.part
		Part4	CustomizeablePowers\Empathy\AbilitySparklingDots.part
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

