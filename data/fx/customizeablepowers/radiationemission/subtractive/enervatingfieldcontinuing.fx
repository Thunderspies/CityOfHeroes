#############################################################
## HEADER
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
		part2	:RadiationBubbleBlastDotsHit.part
		Part3	:RadiationBubblesHit.part
		Part4	:RadiationBubblesSlow.part
		part5	:RadiationAuraYellowHit2.part
		PMagnet	Hips
		Sound fry_loop 55 55 .4
	End
End

Condition
	On	Time
	Time	7


	Event
		Type	Posit
		At	Hips
		part4	:RadiationRedCloud3.part
		Sound goohiss 60 60 .18
		PMagnet	Hips
	End

	Event
		ENAME	LocalSystems
		Type	Posit
		At	Hips
		part4	:RadiationRedCloud.part
		PMagnet	Hips
	End
End

#############################################################

End