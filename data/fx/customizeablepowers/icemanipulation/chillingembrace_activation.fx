#############################################################
## ChillingEmbrace_Activation.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	local
		At	Chest
		Part1	CustomizeablePowers\IceManipulation\IceBoltMistB.part
		Part1	CustomizeablePowers\IceManipulation\ColdStars.part
		Part2	CustomizeablePowers\IceManipulation\ChillTouchSnowA.part
		Sound icebuild2 80 80 .7
	End
End

Condition
	On	Time
	Time	30

	Event
		Type	local
		At	Chest
		bhvr	Behaviors\soundFade3.bhvr
		Sound icearmor_loop 80 80 .7
		Flags	PowerLoopingSound
		Lifespan 250
	End
End

#############################################################

Condition
	On 	Cycle
	Time 	45

	Event
		Type	Local
		At	Hips
		Part3	CustomizeablePowers\IceManipulation\ChillingEmbrace_Ellipse.part
		LifeSpan 45
	End
End

#############################################################

End