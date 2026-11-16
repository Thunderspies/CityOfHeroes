#########################################################
##	Inferno FX
##
FxInfo
	
lifespan	90

Input  
	InpName	Root
End
Input  
	InpName	Head
End

Condition

	On 	Time
	Time 	20
		

	Event
		EName 	Prime
		Type	Start 
		At	Chest
		Part1   :Glow01.part
		PMagnet	Target
		LookAt	Target
		lifespan	30
	End
	Event
		EName 	Prime
		Type	Start 
		At	Chest
		Part2   :Glow02.part
		lifespan	50
	End
End

Condition
	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy
	End

End

End
