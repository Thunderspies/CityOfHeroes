
FxInfo

Lifespan	200

Condition
	On 	Time
	Time 	0
	
	Event
		Type	start
		At	Origin
		LifeSpan 	150
		LifeSpanJitter 	50
		Bhvr	:GlassPanelLarge.bhvr
		CEvent 	:GlassPanelShatter.fx
		CThresh 	0.000001
		CDestroy 	1
		geom	FX_GlassPanel_01
		geom	FX_GlassPanel_02
		geom	FX_GlassPanel_03
		geom	FX_GlassPanel_04
	End


End
