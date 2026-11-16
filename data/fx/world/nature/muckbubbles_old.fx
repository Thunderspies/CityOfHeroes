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
		Part1	World/Nature/MuckBubblePopping_Old.part
		Part2	World/Nature/MuckBubblePoppingFlat_Old.part	
		Part3	World/Nature/MuckBubbleRings2a_Old.part
		Part4	World/Nature/MuckBubbleRising_Old.part
		Part5	World/Nature/MuckBubbleRingRising_Old.part
		Sound Sewer1_loop 5.0 5.0 .4
	End
End

Condition
	On 	Time
	Time 	3
	Event
		EName 	bubbles
		Type	Local 
		At	Root
		Part1	World/Nature/MuckBubbleRising_Old.part
		
	End



End

End			