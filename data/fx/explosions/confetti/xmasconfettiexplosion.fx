
FxInfo

Lifespan	120

Condition
	On 	Time
	Time 	0

	Event
		Ename	Center
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0 3 1.5
		End
		LifeSpan 	5
		Sound confettiBlast 25 25 .4
		Part1	:ConfettiBurst.part
		Part1	:ConfettiBurst.part
		Part1	:ConfettiSparkles.part
		Part1	:ExplosiveFlash.part
	End

	Event
		Type	SetLight
		At	Center
		LifeSpan 	200

		Bhvr	:ConfettiPulse.bhvr
	End

End

#############  Large Red

Condition
	On 	Time
	Time 	0
	Repeat 		50
	RepeatJitter	3

	Event
		Type	start
		At	Center
		LifeSpan 	150
		LifeSpanJitter 	50
		Bhvr	:XmasConfetti.bhvr
		BhvrOverride
			StartColor	255 0 0
			Scale	.2 .2 .2
		End
		geom	FX_Confetti
	End

End

#############  Small Red

Condition
	On 	Time
	Time 	3
	Repeat 		35
	RepeatJitter	6

	Event
		Type	start
		At	Center
		LifeSpan 	150
		LifeSpanJitter 	50
		Bhvr	:XmasConfettiSmall.bhvr
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
	RepeatJitter	3

	Event
		Type	start
		At	Center
		LifeSpan 	150
		LifeSpanJitter 	50
		Bhvr	:XmasConfetti.bhvr
		BhvrOverride
			StartColor	0 255 0
			Scale	.2 .2 .2
		End
		geom	FX_Confetti
	End

End

###########  Small Green

Condition
	On 	Time
	Time 	3
	Repeat 		25
	RepeatJitter	3

	Event
		Type	start
		At	Center
		LifeSpan 	150
		LifeSpanJitter 	50
		Bhvr	:XmasConfettiSmall.bhvr
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
	RepeatJitter	3

	Event
		Type	start
		At	Center
		LifeSpan 	150
		LifeSpanJitter 	50
		Bhvr	:XmasConfetti.bhvr
		BhvrOverride
			StartColor	225 255 0
			Scale	.2 .2 .2
		End
		geom	FX_Confetti
	End

End

###########  Small Gold

Condition
	On 	Time
	Time 	3
	Repeat 		20
	RepeatJitter	3

	Event
		Type	start
		At	Center
		LifeSpan 	150
		LifeSpanJitter 	50
		Bhvr	:XmasConfettiSmall.bhvr
		BhvrOverride
			StartColor	225 255 0
		End
		geom	FX_Confetti
	End

End

End