#############################################################
## ColdMistContinuingWithOutCrystals.fx
#############################################################

FxInfo

LifeSpan 60

#############################################################

Condition
	On	Time
	Time	0

	Event
		Ename   Ice
		Type	local
		At	chest
		Part1	:IceChunks.part
		Part2	:IceChunksLarge.part
		LifeSpan 53
	End

	Event
		Ename   IceShards
		Type	local
		At	chest
		Flags	PowerLoopingSound
		Bhvr	Behaviors\soundFade3.bhvr
		Part1	:IceBoltMistA.part
		Part2	:ChillTouchSnowA.part
		Part4	:ChillTouchSnow.part
		Sound Cold_loop 50.0 50.0 .42
		Lifespan 110
	End
End

#############################################################

End