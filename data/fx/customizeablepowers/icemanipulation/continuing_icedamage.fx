#############################################################
## Continuing_IceDamage.fx
#############################################################

FxInfo

ClampMinScale 0.75 0.75 0.75
ClampMaxScale 1.25 1.25 1.25

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	local
		At	chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part1	:IceChunks.part
		Part2	:IceChunksLarge.part
		Part	:iceScatter.part
		LifeSpan 53
	End

	Event
		Type	local
		At	chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part1	:IceBoltMistA.part
		Part2	:ChillTouchSnowA.part
		Part4	:ChillTouchSnow.part
	End
End

#############################################################

End