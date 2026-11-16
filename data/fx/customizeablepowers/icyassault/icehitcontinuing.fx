#############################################################
## IceHitContinuing.fx
#############################################################

FxInfo

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
		Part	:iceScatter.part
		LifeSpan 53
	End

	Event
		Ename   IceShards
		Type	local
		At	chest
		Part1	:IceBoltMistA.part
		Part2	:ChillTouchSnowA.part
		Part4	:ChillTouchSnow.part
	End
End

#############################################################

End