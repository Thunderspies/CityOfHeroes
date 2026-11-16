
FxInfo

Lifespan	120

Condition
	On 	Time
	Time 	0

	Event
		Type	Start 
		At	Origin
		LifeSpan 	75
		LifeSpanJitter 	30
		Bhvr	:GlassShardExplode.bhvr
		Part2	:GlassShard.part
		CEvent 	:GlassShardShatter.fx
		CThresh 	0.000001
		CDestroy 	1
	End

End
