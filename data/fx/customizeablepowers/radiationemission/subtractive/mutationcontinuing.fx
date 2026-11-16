#############################################################
## MutationContinuing.fx
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
		part2	:RadiationBubbleBlastDotsHitx.part
		Part4	:RadiationBubblesSlowx.part
		Sound goohiss 60 60 .22
		PMagnet	Hips
	End

	Event
		Type	Posit
		At	Hips
		part4	:RadiationBlueCloud.part
		PMagnet	Hips
	End
End
#############################################################

Condition
	On	Time
	Time	8

	Event
		Type	Local
		At	hips
		Sound goo_loop 60 60 .27
		bhvr	behaviors\soundFade2.bhvr
		Flags	PowerLoopingSound
		Lifespan 340
	End
End

#############################################################

End