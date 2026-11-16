#########################################################
##	Fireweapon
########################################################
FxInfo

Input  
	Inpname	Eyes
End

Input  
	Inpname	Chest
End

########################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local 
		At	Chest
			
		part1	:RadiationElectronsHit.part
		part2	:RadiationBubbleBlastDotsHit2.part
		Part3	:RadiationBubblesHit.part
		Part4	:RadiationBubblesSlow2.part
		part	:Flies.part
		part	:Flies2.part
		part	:Beatles.part
		Pmagnet	Chest
	
	End

End

End
##################################

