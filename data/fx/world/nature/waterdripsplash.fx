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
		Part1	World/Nature/Splash.part
		#Part2	World/Nature/MuckBubblePoppingFlat.part	
		#Part3	World/Nature/MuckBubbleRings.part
		#Part4	World/Nature/MuckBubbleRising.part
		#Part5	World/Nature/MuckBubbleRingRising.part
#		Sound	drip1 50 40 .5
#		Sound	drip2 50 40 .5
#		Sound	drip3 50 40 .5
#		Sound	drip4 50 40 .5
#		Sound	drip5 50 40 .5
#		Sound	drip6 50 40 .5
#		Sound	drip7 50 40 .5

	End


End

End			