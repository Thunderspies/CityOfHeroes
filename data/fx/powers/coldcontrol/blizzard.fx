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
		Sound blizzard2 100 100 .9
	End

	Event
		Type	local
		At	Hookup
		altpiv	1
		part1	POWERS\ColdControl\BlizzardFreezingRain.part
		Part2	POWERS\ColdControl\BlizzardFreezingRains.part
		Part3	POWERS\ColdControl\BlizzardSnow.part
		part4	POWERS\ColdControl\BlizzardRain.part
		sound	icicle_loop 80 70 .6
	End

	Event
		EName	LastRainFalling
		Type	local
		At	Hookup
		altpiv	1
		part1	POWERS\ColdControl\BlizzardFreezingRain.part
		Part2	POWERS\ColdControl\BlizzardFreezingRains.part
		Part3	POWERS\ColdControl\BlizzardSnow.part
		part4	POWERS\ColdControl\BlizzardRain.part
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
		ChildFx POWERS\ColdControl\FreezingIceBolt2.fx
		LifeSpan 21
	End
End

#############################################################

End