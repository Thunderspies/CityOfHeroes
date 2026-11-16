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
		part2	POWERS\RadiationControl\RadiationBubbleBlastDotsHit.part
		Part4	POWERS\RadiationControl\RadiationBubblesSlow.part
		PMagnet	Hips
	End

	Event
		Type	Posit
		At	Hips
		part2	POWERS\RadiationControl\RadiationBubbleBlastDotsHitx.part
		Part4	POWERS\RadiationControl\RadiationBubblesSlowx.part
		PMagnet	Hips
	End

	Event
		Type	Posit
		At	Hips
		part4	POWERS\RadiationControl\RadiationTealCloud.part
		PMagnet	Hips
	End
End

#############################################################

End