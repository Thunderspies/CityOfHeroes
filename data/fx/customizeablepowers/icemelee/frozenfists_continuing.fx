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
		Part1	CustomizeablePowers\IceMelee\IceChunks.part
		Part2	CustomizeablePowers\IceMelee\IceChunksLarge.part
		Part	CustomizeablePowers\IceMelee\iceScatter.part
		LifeSpan 53
	End

	Event
		Ename   IceShards
		Type	local
		At	chest
		Part1	CustomizeablePowers\IceMelee\IceBoltMistA.part
		Part2	CustomizeablePowers\IceMelee\ChillTouchSnowA.part
		Part4	CustomizeablePowers\IceMelee\ChillTouchSnow.part
		#Sound 	Ice7 100.0 30.0 1.0
	End
End

#############################################################

End