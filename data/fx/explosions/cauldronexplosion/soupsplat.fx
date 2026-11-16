#########################################################
##	onfire
##
FxInfo  

LifeSpan	500

Condition
	On 	Time
	Time 	0

	Event
		Ename	splat
		Type	start
		At	Origin

		Geom	exp_cauldron_glob
		
		Bhvr	:soupSplat.bhvr

		Lifespan	210
		LifeSpanJitter	60
	End

End

Condition
	On 	Time
	Time 	0

	Repeat	7

	Event
		Ename	drops
		Type	start
		At	splat

		Geom	exp_cauldron_glob
		
		Bhvr	:soupFade.bhvr
		BhvrOverride	
			PositionOffset		0 .4 0

			InitialVelocity		0 0.5 0
			InitialVelocityJitter	0.25 0.25 0.25

			SpinJitter		.2 .2 .2
		End

		CEvent 		:SoupSplatSmall.fx
		CThresh		0.0001
		Cdestroy	1

		Lifespan	1
	End

End

End			