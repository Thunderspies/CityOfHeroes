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
		Part1	CustomizeablePowers\IcyAssault\IceChunks.part
		Part2	CustomizeablePowers\IcyAssault\IceChunksLarge.part
		Part	CustomizeablePowers\IcyAssault\iceScatter.part
		LifeSpan 53
	End

	Event
		Ename   IceShards
		Type	local
		At	chest
		Part1	CustomizeablePowers\IcyAssault\IceBoltMistA.part
		Part2	CustomizeablePowers\IcyAssault\ChillTouchSnowA.part
		Part4	CustomizeablePowers\IcyAssault\ChillTouchSnow.part
		#Sound 	Ice7 100.0 30.0 1.0
	End
End

#############################################################

End