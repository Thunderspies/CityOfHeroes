#############################################################
## ChokingCloudContinuing.fx
#############################################################

FxInfo

#############################################################

Input
	Inpname	Hips
End

#############################################################

Condition
	On	Time
	Time	7

	Event
		Type	Posit
		At	Head
		part2	POWERS\RadiationControl\ChockingCloudDotsHitx.part
		Part3	POWERS\RadiationControl\ChockingCloudBubblesHitx.part
		Part4	POWERS\RadiationControl\ChockingCloudBubblesSlow.part
		part5	POWERS\RadiationControl\ChockingCloudSmoke.part
		part1	POWERS\RadiationControl\ChockingCloudMist.part
		PMagnet	Head
	End

	Event
		Type	Posit
		At	Head
		part2	POWERS\RadiationControl\ChockingCloudDotsTrail.part
		Part3	POWERS\RadiationControl\ChockingCloudBubblesTrail.part
		Part4	POWERS\RadiationControl\ChockingCloudBubblesSlowTrail.part
		part5	POWERS\RadiationControl\ChockingCloudSmokeTrail.part
		part1	POWERS\RadiationControl\ChockingCloudMistTrail.part
		Sound goohiss 60 60 .22
		PMagnet	Head
	End
End

#############################################################

Condition
	On	Time
	Time	12

	Event
		Type	Local
		At	head
		Sound goo_loop 60 60 .27
		bhvr	behaviors\soundFade2.bhvr
		Flags	PowerLoopingSound
		Lifespan 340
	End
End

#############################################################

End