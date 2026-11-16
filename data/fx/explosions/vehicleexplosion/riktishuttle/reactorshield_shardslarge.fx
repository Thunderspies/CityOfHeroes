#########################################################
## Rikti Transport Shuttle - Persistent FX
#########################################################

FxInfo

Lifespan	200

#########################################################

Condition
	On 	Time
	Time 	0
	
	Event
		Type	start
		At	Origin
		LifeSpan 	150
		LifeSpanJitter 	50
		Bhvr	:ReactorShield_Shards.bhvr
		BhvrOverride
			InitialVelocity		0.0 -0.25 0.0
			InitialVelocityJitter	0.5 0.5 0.5
			Spin			0.0 0.0 0.0
			SpinJitter		0.125 0.125 0.125
			Scale			2.5 2.5 2.5
			Drag			0.01
		End
		geom	FX_GlassPanel_01
		geom	FX_GlassPanel_02
		geom	FX_GlassPanel_03
		geom	FX_GlassPanel_04
		#part1	:ReactorShield_ShardsGlow.part
	End
End

#########################################################

End