#########################################################
##  Temporary Invulnerability Continuing FX
#########################################################

FxInfo

#########################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Hips
		Sound regen5b 60 60 .5
	End

	Event
		Type	Local
		At	Hips
		Sound glow5_loop 46 46 .11
		Bhvr	Behaviors\soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan	400
	End
End

Condition
	On	Time
	Time	31

	Event
		Ename	Ring
		Type	Local
		At	Hips
		Part3	CustomizeablePowers\Willpower\ERing.part
		Part5	CustomizeablePowers\Willpower\ERing2.part
		Part1	CustomizeablePowers\Willpower\Cloud01.part
		Part2	CustomizeablePowers\Willpower\Cloud02.part
		Part4	CustomizeablePowers\Willpower\Cloud03.part
	End
End

Condition
	On	Time
	Time	44

	Event
		Ename	Ring
		Type	Local
		At	Hips
		Part3	CustomizeablePowers\Willpower\ERing.part
		Part5	CustomizeablePowers\Willpower\ERing2.part
		Part1	CustomizeablePowers\Willpower\Cloud01.part
		Part2	CustomizeablePowers\Willpower\Cloud02.part
		Part4	CustomizeablePowers\Willpower\Cloud03.part
	End
End

#########################################################

Condition
	On	Cycle
	Time	120
	Chance	0.125

	Event
		Ename	BodyAura
		Type	Local
		At	Origin
		ChildFX	:WillPower_Continuing_Pulse.fx
		Lifespan	60
	End
End

#########################################################

End