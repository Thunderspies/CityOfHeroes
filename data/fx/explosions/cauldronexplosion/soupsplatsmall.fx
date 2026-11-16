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
		
		Bhvr	:soupSplatSmall.bhvr

		Lifespan	150
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

			Scale			.1 .1 .1

			InitialVelocity		0 0.3 0
			InitialVelocityJitter	0.15 0.15 0.15

			SpinJitter		.2 .2 .2

			Physics			0
		End

		Lifespan	1
	End

End

End			