#############################################################
## ColdMistContinuingWithOutCrystals.fx
#############################################################

FxInfo

Input
	InpName	Hips
End

LifeSpan	60

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
		#Part	:iceScatter.part
		LifeSpan	53

	End

	Event
		Ename   IceShards
		Type	local
		At	chest
		Part1	CustomizeablePowers\IceBlast\IceBoltMistA.part
		Part2	CustomizeablePowers\IceBlast\ChillTouchSnowA.part
		Part4	CustomizeablePowers\IceBlast\ChillTouchSnow.part
		Sound Cold_loop 50.0 50.0 .42
		bhvr	behaviors/soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 110

	End


End


End