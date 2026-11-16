#############################################################
## ChockingCloud.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time 	5

	Event
		EName	Hand1
		Type	Local
		At	WepR
		Part1	:RadiationInfectOthersHands.part
		Part2	:RadiationHandElectrons.part
		Part3	:InfectBubbleDotsHands.part
		Part4	:InfectBubblesHands.part
		PMagnet	WepR
		Sound radinfect1 66 66 .75
		Lifespan 100
	End

	Event
		EName	Hand2
		Type	Local
		At	WepL
		Part1	:RadiationInfectOthersHands.part
		Part2	:RadiationHandElectrons.part
		Part3	:InfectBubbleDotsHands.part
		Part4	:InfectBubblesHands.part
		PMagnet	WepL
		Lifespan 100
	End
End

#############################################################

Condition
	On 	Time
	Time 	60

	Event
		Type	Local
		At	Root
		bhvr	behaviors\soundFade2.bhvr
		Sound Tar_loop 66 66 .64
		Flags	PowerLoopingSound
		Lifespan 240
	End

	Event
		EName 	Ring
		Type	Posit
		At	Root
		Part1	:Yellow1.part
		Part2	:Yellow2.part
		Part5	:PoisonBubblesYellow.part
	End

	Event
		EName 	Ring
		Type	Posit
		At	Root
		Part1	:PoisonOutsideRings.part
		Part2	:PoisonOutsideGlows.part
		Sound rumble_loop 66 66 .36
	End
End

#############################################################

Condition
	On 	Time
	Time 	60

	Event
		EName 	InnerCloudStuff
		Type	Local
		At	Root
		ChildFX	:Child_ChokingCloud.fx
	End
End

#############################################################

End