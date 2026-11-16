#############################################################
## FallOutContinuing.fx
#############################################################

FxInfo

Lifespan 100

#########################################################

Input
	Inpname	Hips
End

#########################################################

Condition
	On	Time
	Time	7

	Event
		Type	Local
		At	Hips
		Sound goohis 33 33 0.22
	End

	Event
		Type	Posit
		At	Hips
		Part	POWERS\RadiationControl\RadiationBubbleBlastDotsHitx.part
		Part	POWERS\RadiationControl\RadiationBubblesHitx.part
		Part	POWERS\RadiationControl\RadiationBubblesSlowx.part
		Sound goo_loop 66 66 0.25
		PMagnet	Hips
	End
End

#############################################################

End