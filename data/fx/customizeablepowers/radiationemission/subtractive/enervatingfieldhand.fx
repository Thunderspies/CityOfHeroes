#############################################################
## enervatingfieldHand.fx
#############################################################

FxInfo

#########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Hand1
		Type	Local
		At	WepR
		Part1	:RadiationInfectOthersHands.part
		Part2	:RadiationHandElectrons.part
		Part3	:InfectBubbleDotsHands.part
		Part4	:InfectBubblesHands.part
		PMagnet	WepR
		Sound mutationthrow 66 66 .75
		LifeSpan	50
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


		LifeSpan	50
	End

End

Condition
	On 	Time
	Time	0

	Event
		Type	Local
		At 	wepL
		Sound enervating2_loop 66 66 .33
		bhvr	CustomizeablePowers\RadiationEmission\soundFade2.bhvr
		Flags	PowerLoopingSound
		Lifespan 380
	End
End

Condition

	On 	Time
	Time 	9

	Event
		Type	Local
		At 	wepL
		Sound Spewhiss_loop 66 66 .27
		bhvr	CustomizeablePowers\RadiationEmission\soundFade2.bhvr
		Flags	PowerLoopingSound
		Lifespan 380
	End
End



Condition
	On 	Time
	Time	25

	Event
		Type	Local
		At 	wepL
		Sound radiation5_loop 66 66 .45
		bhvr	CustomizeablePowers\RadiationEmission\soundFade2.bhvr
		Flags	PowerLoopingSound
		Lifespan 380
	End
End

#########################################################

End