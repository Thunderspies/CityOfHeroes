#########################################################
##	FireRing
##
FxInfo
	

Input  
	InpName	Root
End

#########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	bubbles
		Type	Local 
		At	Root
		
		Part1	World/Nature/MuckBubbleRings3.part
		Part2	World/Nature/MuckBubbleRising3.part
	End
End

	Condition
	On 	time
	Time 	0
	
	
	Event
		EName 	bubbles
		Type	Local 
		At	Root
		Part1	World/Nature/MuckBubbleRingFlat1.part
		Part2	World/Nature/MuckBubbleRising3.part
	End
End


	Condition
	On 	time
	Time 	3
		
	Event
		EName 	bubbles
		Type	Local 
		At	Root
		
		Part1	World/Nature/MuckBubbleRingFlat3.part
		Part2	World/Nature/MuckBubbleRising3.part
	End
End


	Condition
	On 	time
	Time 	7
	
	
	Event
		EName 	bubbles
		Type	Local 
		At	Root
		
		Part1	World/Nature/MuckBubbleRingFlat3.part
		Part2	World/Nature/MuckBubbleRising3.part
	End


End

End			