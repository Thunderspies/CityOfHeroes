#############################################################
## KinTransfer.fx
#############################################################

FxInfo
Lifespan 300

#############################################################

Condition
	On	Time
	Time	15

	Event
		Type	Local
		At 	Origin
		Sound fulcrumshift1 90 90 .95
	End

	Event
		Type	Local
		At 	Origin
		Sound blind 80 80 .6
	End

	Event
		Type	Local
		At 	Origin
		Sound fulcrumshift2 80 80 .55
	End
End

#############################################################

Condition
	On	Time
	Time	25

	Event
		ENAME	WepLGlow
		Type	Local
		At	WepL
		part	:KineticHandGlows01.part
		Lifespan 35
	End

	Event
		ENAME	WepRGlow
		Type	Local
		At	WepR
		part	:KineticHandGlows01.part
		Lifespan 35
	End
End

Condition
	On	Time
	Time	25

	Event
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		part	:KineticBodyGlows01.part
		POther LarmL
		Lifespan 40
	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		POther  LarmL
		part	:KineticBodyGlows01.part
		part	:KineticBodyGlows01_Alpha.part

		POther LarmL
		Lifespan 40
	End

	Event
		ENAME	RightUarmGlow
		Type	Local
		At	SpadR
		part	:KineticBodyGlows01.part
		part	:KineticBodyGlows01_Alpha.part

		POther LarmR
		Lifespan 40
	End

	Event
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		POther  LarmL
		part	:KineticBodyGlows01.part
		part	:KineticBodyGlows01_Alpha.part

		POther LarmR
		Lifespan 40
	End
End

############################################################

Condition
	On	Time
	Time	25

	Event
		ENAME	LeftUarmGlow
		Type	Local
		At	Target
		part	:KineticCastRaysBlue01.part
		PMagnet WepL
		Lifespan 40
	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part	:KineticIDBodySwirls01.part
		PMagnet Target
		Lifespan 40
	End

	Event
		ENAME	LeftUarmGlow
		Type	Local
		At	Target
		part	:KineticCastRaysBlue01.part
		PMagnet WepR
		Lifespan 40
	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepR
		part	:KineticIDBodySwirls01.part
		PMagnet Target
		Lifespan 40
	End

End

############################################################

Condition
	On	Time
	Time	20

	Event
		ENAME	FlyawayNodeL
		Type	Local
		At	WepL
		Bhvr	CustomizeablePowers\Kinetics\KineticScaleSmall.bhvr
		LifeSpan 50
	End

	Event
		ENAME	FlyawaysL1
		Type	Local
		At	FlyawayNodeL
		part	:KineticSwirlFlyAway03.part
		Bhvr	CustomizeablePowers\Kinetics\KineticSpin3.bhvr
		LifeSpan 15
	End

	Event
		ENAME	FlyawayNodeR
		Type	Local
		At	WepR
		Bhvr	CustomizeablePowers\Kinetics\KineticScaleSmall.bhvr
		LifeSpan 50
	End

	Event
		ENAME	FlyawaysR1
		Type	Local
		At	FlyawayNodeR
		part	:KineticSwirlFlyAway03.part
		Bhvr	CustomizeablePowers\Kinetics\KineticSpin3.bhvr
		LifeSpan 15
	End
End

Condition
	On	Time
	Time	25

	Event
		ENAME	Flyaways2
		Type	Local
		At	FlyawayNodeL
		part	:KineticSwirlFlyAway03.part
		Bhvr	CustomizeablePowers\Kinetics\KineticSpin3.bhvr
		LifeSpan 15
	End

	Event
		ENAME	FlyawaysR2
		Type	Local
		At	FlyawayNodeR
		part	:KineticSwirlFlyAway03.part
		Bhvr	CustomizeablePowers\Kinetics\KineticSpin3.bhvr
		LifeSpan 15
	End

End

Condition
	On	Time
	Time	30

	Event
		ENAME	FlyawaysL3
		Type	Local
		At	FlyawayNodeL
		part	:KineticSwirlFlyAway03.part
		Bhvr	CustomizeablePowers\Kinetics\KineticSpin3.bhvr
		LifeSpan 15
	End

	Event
		ENAME	FlyawaysR3
		Type	Local
		At	FlyawayNodeR
		part	:KineticSwirlFlyAway03.part
		Bhvr	CustomizeablePowers\Kinetics\KineticSpin3.bhvr
		LifeSpan 15
	End
End

#############################################################

End
