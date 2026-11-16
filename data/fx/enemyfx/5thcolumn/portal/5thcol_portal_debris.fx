#############################################################
## 5thCol_Portal_Debris.fx
#############################################################

FxInfo

Lifespan 300

########################################################
Condition
	On	Time
	Time	0

	Event
		Type	start
		At	Origin
		LifeSpan 	150
		LifeSpanJitter 	50
		Bhvr		:DebrisExplosion.bhvr
		CThresh 	0.000001
		geom	_Fifthcol_Portal_chunk_01

	End

	Event
		Type		start
		At		Origin
		LifeSpan 	150
		LifeSpanJitter 	50
		Bhvr		:DebrisExplosion.bhvr
		CThresh 	0.000001
		Part	:FireSplash.part
		Part	:FireSplashEmbers.part
		CEvent 	:FireNapalmHit.fx
		geom	_Fifthcol_Portal_chunk_02

	End

	Event
		Type		start
		At		Origin
		LifeSpan 	150
		LifeSpanJitter 	50
		Bhvr		:DebrisExplosion.bhvr
		Part1	:FireSplash.part
		Part3	:FireSplashEmbers.part
		CEvent 	:FireNapalmHit.fx
		CThresh 	0.000001
		geom	_Fifthcol_Portal_chunk_02

	End

	Event
		Type		start
		At		Origin
		LifeSpan 	150
		LifeSpanJitter 	50
		Bhvr		:DebrisExplosion.bhvr
		CThresh 	0.000001
		Part1	:FireSplash.part
		Part3	:FireSplashEmbers.part
		CEvent 	:FireNapalmHit.fx
		geom	_Fifthcol_Portal_chunk_02

	End

	Event
		Type		start
		At		Origin
		LifeSpan 	150
		LifeSpanJitter 	50
		Bhvr		:DebrisExplosion.bhvr
		CThresh 	0.000001
		geom		_Fifthcol_Portal_chunk_05

	End

	Event
		Type		start
		At		Origin
		LifeSpan 	150
		LifeSpanJitter 	50
		Bhvr		:DebrisExplosion.bhvr
		CThresh 	0.000001
		geom		_Fifthcol_Portal_chunk_06

	End

End