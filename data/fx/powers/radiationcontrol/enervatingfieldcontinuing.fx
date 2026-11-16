#############################################################
## EnervatingFieldContinuing.fx
#############################################################

FxInfo
ClampMaxScale 3.5 3.5 3.5

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
		At	hips
		Sound fry_loop 55 55 .4
		bhvr	behaviors\soundFade2.bhvr
		Flags	PowerLoopingSound
		Lifespan 380
	End
End

Condition
	On	Time
	Time	7

	Event
		Type	Posit
		At	Hips
		part2	POWERS\RadiationControl\RadiationBubbleBlastDotsHit.part
		Part3	POWERS\RadiationControl\RadiationBubblesHit.part
		Part4	POWERS\RadiationControl\RadiationBubblesSlow.part
		part5	POWERS\RadiationControl\RadiationAuraYellowHit2.part
		PMagnet	Hips
		#Sound	fry_loop 55 22 .4
	End

	Event
		Type	Posit
		At	Hips
		part4	POWERS\RadiationControl\RadiationRedCloud3.part
		Sound goohiss 60 60 .18
		PMagnet	Hips
	End

	Event
		ENAME	LocalSystems
		Type	Posit
		At	Hips
		part2	POWERS\RadiationControl\RadiationBubbleBlastDotsHitx.part
		Part3	POWERS\RadiationControl\RadiationBubblesHitx.part
		Part4	POWERS\RadiationControl\RadiationBubblesSlowx.part
		PMagnet	Hips
	End

	Event
		ENAME	LocalSystems
		Type	Posit
		At	Hips
		part4	POWERS\RadiationControl\RadiationRedCloud.part
		PMagnet	Hips
	End
End

#############################################################

End