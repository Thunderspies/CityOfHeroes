#########################################################
##	Inferno FX
##
FxInfo
	
LifeSpan 40

Input  
	InpName	Root
End

Input  
	Inpname	hips
End

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime1
		Type	Local 
		At	hips
		
		Part1	:InfernoCare.part
		Part2	:Inferno.part

	End

	Event
		EName 	Prime2
		Type	Local 
		At	hips
		Part1	:InfernoExplosion.part
		

	End

End

Condition
	On 	Time
	Time 	3

	Event
		EName 	Prime
		Type	Local 
		At	hips
		Part1	:InfernorockBits.part
		Part2	:InfernorockBitz.part
	End


End

Condition
	On 	Time
	Time	9

	Event
		EName 	Prime1
		Type	Destroy
	End

End

Condition
	On 	Time
	Time	3

	Event
		EName 	Prime2
		Type	Destroy
	End

End


Condition
	On 	Time
	Time	7

	Event
		EName 	Prime
		Type	Destroy
	End

End	

End			