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
		Type	start 
		At	Root
		
		#Part1	World/Nature/MuckBubbleRings.part
		#Part2	World/Nature/MuckBubbleRings1.part
		Part3	World/Nature/MuckBubbleRingsOverlap.part
		#Part4	World/Nature/MuckBubbleRings3.part
		#Part5	World/Nature/MuckBubbleRingsLarge.part
	End
End

	Condition
	On 	time
	Time 	0
	
	
	Event
		EName 	bubbles
		Type	Local 
		At	Root
		#Part1	World/Nature/MuckBubbleRingFlat1.part
		
	End
End


	Condition
	On 	time
	Time 	3
		
	Event
		EName 	bubbles
		Type	Local 
		At	Root
		
		#Part1	World/Nature/MuckBubbleRingFlat3.part
		
	End
End


	Condition
	On 	time
	Time 	7
	
	
	Event
		EName 	bubbles
		Type	Local 
		At	Root
		
		#Part1	World/Nature/MuckBubbleRingFlat3.part
		
	End


End

End			