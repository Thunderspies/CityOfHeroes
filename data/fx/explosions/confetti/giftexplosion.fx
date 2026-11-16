
FxInfo

Lifespan	30

Condition
	On 	Time
	Time 	0

	Event
		Ename	Center
		Type	Local
		At	Origin
		LifeSpan 	5
		# Sound confettiBlast 88 88 .9
		Part1	:ConfettiBurst.part
		Part1	:ConfettiBurst.part
		Part1	:ConfettiSparkles.part
		Part1	:ExplosiveFlash.part
	End

	Event
		Type	SetLight
		At	Center
		LifeSpan 	120

		Bhvr	:ConfettiPulse.bhvr
	End

End

#############  Large Red

Condition
	On 	Time
	Time 	0
	Repeat 		50
	RepeatJitter	2

	Event
		Type	start
		At	Center
		LifeSpan 	150
		LifeSpanJitter 	50
		Bhvr	:XmasConfetti.bhvr
		BhvrOverride
			StartColor	255 0 0
			Scale	.1 .1 .1
		End
		geom	FX_Confetti
	End

End

#############  Small Red

Condition
	On 	Time
	Time 	3
	Repeat 		35
	RepeatJitter	5

	Event
		Type	start
		At	Center
		LifeSpan 	150
		LifeSpanJitter 	50
		Bhvr	:XmasConfetti.bhvr
		BhvrOverride
			StartColor	255 0 0
		End
		geom	FX_Confetti
	End

End

###########  Large Green

Condition
	On 	Time
	Time 	0
	Repeat 		25
	RepeatJitter	2

	Event
		Type	start
		At	Center
		LifeSpan 	150
		LifeSpanJitter 	50
		Bhvr	:XmasConfetti.bhvr
		BhvrOverride
			StartColor	0 255 0
			Scale	.1 .1 .1
		End
		geom	FX_Confetti
	End

End

###########  Small Green

Condition
	On 	Time
	Time 	3
	Repeat 		25
	RepeatJitter	2

	Event
		Type	start
		At	Center
		LifeSpan 	150
		LifeSpanJitter 	50
		Bhvr	:XmasConfetti.bhvr
		BhvrOverride
			StartColor	0 255 0
		End
		geom	FX_Confetti
	End

End

###########  Large Gold

Condition
	On 	Time
	Time 	0
	Repeat 		20
	RepeatJitter	2

	Event
		Type	start
		At	Center
		LifeSpan 	150
		LifeSpanJitter 	50
		Bhvr	:XmasConfetti.bhvr
		BhvrOverride
			StartColor	225 255 0
			Scale	.1 .1 .1
		End
		geom	FX_Confetti
	End

End

###########  Small Gold

Condition
	On 	Time
	Time 	3
	Repeat 		20
	RepeatJitter	2

	Event
		Type	start
		At	Center
		LifeSpan 	150
		LifeSpanJitter 	50
		Bhvr	:XmasConfetti.bhvr
		BhvrOverride
			StartColor	225 255 0
		End
		geom	FX_Confetti
	End

End

End