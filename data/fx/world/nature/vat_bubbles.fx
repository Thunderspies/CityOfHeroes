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
		Type	posit 
		At	Root
		Part1	World/Nature/MuckBubblePoppingLarge.part
		##Part3	World/Nature/MuckBubbleRingsLarge.part
		Part4	World/Nature/MuckBubbleRisingLarge.part
		Part5	World/Nature/MuckBubbleRingRisingLarge.part
		Part	:MuckBubbleMistLarge.part
		Sound BubHiss1_loop 20.0 20.0 .2
		Sound BubHiss2_loop 25.0 25.0 .3
	End
	
	Event
		EName 	bubbles
		Type	posit 
		At	Root
		#Part2	World/Nature/MuckBubblePopFlatLarge.part	
		
	End

End

End			