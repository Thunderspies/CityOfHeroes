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
		At	Root

		Geom	FX_BlurBall

		Bhvr	:DistortionFade.bhvr

		Lifespan	75
		LifespanJitter	30
	End

End

Condition
	On 	Cycle
	Time 	1
	Chance	.6

	Repeat	1

	Event
		Ename	Heat1
		Type	Start
		At	Root

		Geom	FX_BlurBall

		Bhvr	:DistortionFade.bhvr
		BhvrOverride
			Scale			.75 .75 .75
		End

		Lifespan	75
		LifespanJitter	30
	End

End

Condition
	On 	Cycle
	Time 	1
	Chance	.7

	Repeat	1

	Event
		Ename	Heat2
		Type	Start
		At	Root

		Geom	FX_BlurBall

		Bhvr	:DistortionFade.bhvr
		BhvrOverride
			Scale			.5 .5 .5
		End

		Lifespan	75
		LifespanJitter	30
	End

End



End
