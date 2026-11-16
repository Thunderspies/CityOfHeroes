#########################################################
##	chill touch hit
FxInfo

Input  
	InpName	chest
End

##################################

Condition
	On	Time
	Time	0
		

	Event	
		Ename   Ice
		Type	local
		At	chest
		Part1	CustomizeablePowers\IceBlast\IceChunks.part
		Part2	CustomizeablePowers\IceBlast\IceChunksLarge.part
		Part	CustomizeablePowers\IceBlast\iceScatter.part
		LifeSpan	53
		
	End

	Event	
		Ename   IceShards
		Type	local
		At	chest
		Part1	CustomizeablePowers\IceBlast\IceBoltMistA.part
		Part2	CustomizeablePowers\IceBlast\ChillTouchSnowA.part
		Part4	CustomizeablePowers\IceBlast\ChillTouchSnow.part
		Sound Ice7 100.0 100.0 1.0
	End


End


End				