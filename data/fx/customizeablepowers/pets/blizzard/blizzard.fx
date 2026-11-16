#############################################################
## Blizzard.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	Hookup
		Type	local
		At	root
		Geom	OverHeadDummy01
		Sound blizzard2 100 100 0.9
	End

	Event
		Type	local
		At	Hookup
		altpiv	1
		part1	CustomizeablePowers\IceBlast\BlizzardFreezingRain.part
		Part2	CustomizeablePowers\IceBlast\BlizzardFreezingRains.part
		Part3	CustomizeablePowers\IceBlast\BlizzardSnow.part
		part4	CustomizeablePowers\IceBlast\BlizzardRain.part
		sound	icicle_loop 80 70 0.6
	End
End

#############################################################

Condition
	On 	cycle
	Time 	3
	Chance	0.95

	Event
		Type	start
		At	Hookup
		altpiv	1
		ChildFx CustomizeablePowers\IceBlast\Child_FreezingIceBolt2.fx
		LifeSpan 21
	End
End

#############################################################

End