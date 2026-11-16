#########################################################
## Freezing Rain - Continuing FX
#########################################################

FXInfo

ClampMinScale 0.75 0.75 0.75
ClampMaxScale 1.25 1.25 1.25

Lifespan 900

#########################################################

Condition
	On	Time
	Time	0

	Event
		Type	local
		At	chest
		Sound Cold_loop 50.0 50.0 .42
		bhvr	behaviors\soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 110
	End

	Event
		Type	local
		At	chest
		Part1	CustomizeablePowers\IceArmor\IceBoltMistA.part
		Part2	CustomizeablePowers\IceArmor\ChillTouchSnowA.part
	End
End

#########################################################

End