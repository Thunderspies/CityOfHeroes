#########################################################
##	SStreetSteeam01
##
FxInfo
	
LifeSpan 0

Input  
	InpName	root
End

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	start 
		At	root
		Part1	:fire_flame1.part
		#Part2	:Phlanxspark1.part
		#Part3	:fire_flame2.part
		#Part4	:PhalanxFlameGround.part
		
	End

End

End			