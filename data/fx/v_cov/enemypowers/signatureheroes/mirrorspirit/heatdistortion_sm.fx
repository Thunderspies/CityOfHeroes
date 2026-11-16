#########################################################
##	Mirror Cage
########################################################
FxInfo

############################################


Condition
	On 	Cycle
	Time 	1
	Chance	.8

	Repeat	1

	Event
		Ename	Heat3
		Type	Start

		Geom	FX_BlurBall

		Bhvr	:DistortionFade.bhvr
		BhvrOverride
			StartJitter		0.5 0.0 0.5
		End


		Lifespan	25
		LifespanJitter	10
	End

End

Condition
	On 	Cycle
	Time 	2
	Chance	.6

	Repeat	1

	Event
		Ename	Heat1
		Type	Start

		Geom	FX_BlurBall

		Bhvr	:DistortionFade.bhvr
		BhvrOverride
			StartJitter		0.5 0.0 0.5
			Scale			.75 .75 .75
			InitialVelocityJitter	0.15 0.15 0.15
		End

		Lifespan	20
		LifespanJitter	10
	End

End

Condition
	On 	Cycle
	Time 	3
	Chance	.7

	Repeat	1

	Event
		Ename	Heat2
		Type	Start

		Geom	FX_BlurBall

		Bhvr	:DistortionFade.bhvr
		BhvrOverride
			StartJitter		0.5 0.0 0.5
			Scale			.5 .5 .5
			InitialVelocityJitter	0.2 0.2 0.2
		End

		Lifespan	15
		LifespanJitter	10
	End

End



End
