#########################################################
##	RadiationAura
FxInfo


Input  
	InpName	Hips
End

Input  
	InpName	Head
End

##################################
Condition
	On	Time
	Time	21
	Event
		Type	Posit
		At	Hips
		part1	:ChockingCloud.part
		part2	:ChockingCloudLocal.part
		Part3	:ChockingCloud2.part
		part4	:ChockingCloudLocal2.part
		#part5	:RadiationAuraWhite.part
		
		PMagnet	Hips
	End

	Event
		Type	Posit
		At	Hips
		#part4	:RadiationBubbleDotsHit.part
		PMagnet Head
	End

End

End		