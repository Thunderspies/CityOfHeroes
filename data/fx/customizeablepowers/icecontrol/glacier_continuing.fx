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
		Part1	CustomizeablePowers\IceControl\IceChunks.part
		Part2	CustomizeablePowers\IceControl\IceChunksLarge.part
		Part	CustomizeablePowers\IceControl\iceScatter.part
		LifeSpan 53
	End

	Event
		Ename   IceShards
		Type	local
		At	chest
		Part1	CustomizeablePowers\IceControl\IceBoltMistA.part
		Part2	CustomizeablePowers\IceControl\ChillTouchSnowA.part
		Part4	CustomizeablePowers\IceControl\ChillTouchSnow.part
		#Sound 	Ice7 100.0 30.0 1.0
	End
End

#############################################################

End