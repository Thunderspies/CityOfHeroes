#############################################################
## ChokingCloudContinuing.fx
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
		At	Head
		part2	:ChockingCloudDotsHitx.part
		Part3	:ChockingCloudBubblesHitx.part
		Part4	:ChockingCloudBubblesSlow.part
		part5	:ChockingCloudSmoke.part
		part1	:ChockingCloudMist.part
		PMagnet	Head
	End

	Event
		Type	Posit
		At	Head
		part2	:ChockingCloudDotsTrail.part
		Part3	:ChockingCloudBubblesTrail.part
		Part4	:ChockingCloudBubblesSlowTrail.part
		part5	:ChockingCloudSmokeTrail.part
		part1	:ChockingCloudMistTrail.part
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