#############################################################
## GiftExplosion.fx
#############################################################

FxInfo
Lifespan	250

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Ename	Center
		Type	Start
		At	Chest
		Sound confettiBlast 25 25 .4
		Part1	:ConfettiBurst.part
		Part1	:ConfettiBurst.part
		Part1	:ConfettiSparkles.part
		Part1	:ExplosiveFlash.part
		Part	:Sparks.part
		Part	:SmokeLight.part
		Part	:SmokeLight2.part
		Part	:Blast.part
		LifeSpan 	5

	End

	Event
		Type	SetLight
		At	Center
		LifeSpan 	120
		Bhvr	:ConfettiPulse.bhvr
	End

End

#############  Box Lid

Condition
	On 	Time
	Time 	0

	Event
		Type	start
		At	Chest
		Bhvr	Explosions\MetalExplosion\MetalPanelLightFade.bhvr
		BhvrOverride

			InitialVelocity		0.0 0.8 0.0
			InitialVelocityJitter	0.2 0.1 0.2
			Scale			4.5 4.5 4.5
			SpinJitter		0.2 0.2 0.2
			Gravity			0.04
			Drag			0.006
			FadeOutLength		300

			physics 		1
			physRadius 		1
			physGravity 		.9
			physRestitution 	0.1
			physSFriction 	 	0.6
			physDFriction 	 	0.8
		End
		LifeSpan 	250
		Geom	GEO_HolidayGift1_Top
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