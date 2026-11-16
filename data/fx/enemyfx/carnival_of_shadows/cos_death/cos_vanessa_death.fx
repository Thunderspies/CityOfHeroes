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
	Time 	30

	Event
		EName 	Prime1
		Type	Local 
		At	Root
		bhvr    :Infernobubble.bhvr
		Part1	:Glow01.part	
	End		

	Event
		EName 	Prime8
		Type	Local 
		At	root
		Part1   :DeathRing01.part
	End
End

Condition

	On 	Time
	Time 	40

	Event
		EName 	Prime2
		Type	Local 
		At	Root
		Part1	:Glow02.part	
		Lifespan	15
	End		
End

Condition

	On 	Time
	Time 	65

	Event
		EName 	Prime2
		Type	Local 
		At	Head
		Part1	:Glow03.part	
		Lifespan	15
	End		
End

Condition

	On 	Time
	Time 	70

	Event
		EName 	Prime2
		Type	Local 
		At	Head
		Part1	:CoS_Death_Ghost.part	
		Part1	:CoS_Death_Ghost_Cloud.part
		Lifespan	15
	End		
End

End
