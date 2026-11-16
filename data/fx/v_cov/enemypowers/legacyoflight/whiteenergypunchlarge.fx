#########################################################
##	cot_waterfall_med.fx
##
FxInfo

LifeSpan	40

Input  
	InpName	Chest
End


###########################################################

Condition
	On 	Time
	Time 	26
	
	Event
		Type	local
		At	WepR
		BHVR	Behaviors/GenericParticleFade.bhvr
		Part	:WhitePunchHandRing.part
		

	End
	
	Event
		Type	local
		At	WepL
		BHVR	Behaviors/GenericParticleFade.bhvr
		Part	:WhitePunchHandRing.part

	End
End
	
Condition
	On 	Time
	Time 	3
	
	Event
		EName 	HandGlowRight1
		Type	local
		At	WepR
		Part1	:CometHandBubbles.part
		Part2	:CometHandBubblesWhite.part
		Sound LegacyHands2 70.0 70.0 .88
		PMagnet	WepR
		LifeSpan	30
	End

	Event
		EName 	HandGlowLeft1
		Type	local
		At	WepL
		Part1	:CometHandBubbles.part
		Part2	:CometHandBubblesWhite.part
		
		PMagnet	WepL
		LifeSpan	30
	End

End
	
Condition
	On 	Time
	Time 	0
		
	Event
		EName 	HandGlowRight22
		Type	local
		At	WepR
		Part1	:HandGlowWhite.part
		Part2	:HandGlowEmber.part
		PMagnet	WepR
	End

	Event
		EName 	HandGlowLeft22
		Type	local
		At	WepL
		Part1	:HandGlowWhite.part
		Part2	:HandGlowEmber.part
		PMagnet	WepL
	End
	
End


Condition
	On 	Time
	Time 	0

	Event
		EName 	HandGlowRight2
		Type	local
		At	WepR
		Part1	:HandGlowWhite.part
		Part2	:HandGlowEmber.part
		PMagnet	WepR
	End

	Event
		EName 	HandGlowLeft2
		Type	local
		At	WepL
		Part1	:HandGlowWhite.part
		Part2	:HandGlowEmber.part
		PMagnet	WepL
	End

End

End