#########################################################
##	Fire_Ball
##
FxInfo

LifeSpan	100

Input  
	InpName	Hair
End    

Condition
	On 	Time
	Time 	0

		
	Event
		Type	local
		At	Hair

		Part1	:DoublesLight.part
		Part2	:DoublesLightRing.part
		Part5	:DoublesStreaks.part
		
	End

End    

Condition
	On 	Time
	Time 	0

		
	Event
		ENAME	Sparkles
		Type	local
		At	Hair

		Part4	:DoublesLight3.part
			
	End

End	

Condition
	On 	Time
	Time 	20

		
	Event
		ENAME	Sparkles
		Type	Destroy
		
	End

End

End			