#############################################################
## HeavyHands_Activation.fx
#############################################################

FxInfo

LifeSpan 45

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Local
		At	WepL
		Part1	:BluePowerpunch1.part
		Part2	:BluePowerpunch2.part
		Part1	:BluePowerpunch3.part
		Part3   :BluePowerpunchGlow.part
		Part3   :BluePowerpunchRings.part
		sound HeavyHands_PowerPunch_Attack 100 100 0.8
	End
End

#############################################################

End