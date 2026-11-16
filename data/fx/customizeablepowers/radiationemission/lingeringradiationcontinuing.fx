#############################################################
## LingeringRadiationContinuing.fx
#############################################################

FxInfo

#########################################################

Input
	Inpname	Hips
End

#########################################################

Condition
	On	Time
	Time	7

	Event
		Type	Posit
		At	Hips
		part2	:RadiationBubbleBlastDotsHit.part
		Part4	:RadiationBubblesSlow.part
		PMagnet	Hips
	End

	Event
		Type	Posit
		At	Hips
		part4	:RadiationTealCloud.part
		PMagnet	Hips
	End
End

#############################################################

End