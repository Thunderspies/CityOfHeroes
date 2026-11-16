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
		Part1	World/Nature/MuckBubblePopping.part
		Part2	World/Nature/MuckBubblePoppingFlat.part	
		Part3	World/Nature/MuckBubbleRings2a.part
		Part4	World/Nature/MuckBubbleRising.part
		Part5	World/Nature/MuckBubbleRingRising.part
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
		Part1	World/Nature/MuckBubbleRising.part
		
	End



End

End			