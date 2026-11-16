#########################################################
## Kinetics - Siphon Speed
#########################################################

FxInfo
Lifespan 200

## SOUND FX #######################################################

Condition
	On	Time
	Time	15


	Event
		Type	Local
		At 	Origin
		Sound kinetic2 80 80 .8
	End
End


## CAST FX #######################################################

Condition
	On	Time
	Time	25

	Event
		ENAME	WepLGlow
		Type	Local
		At	WepL
		part	:KineticHandGlows01.part
		Lifespan	20
	End
End

Condition
	On	Time
	Time	20

	Event
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		part	:KineticBodyGlows01.part
		part	:KineticBodyGlows01_Alpha.part

		POther	LarmL
		Lifespan	30
	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part	:KineticBodyGlows01.part
		part	:KineticBodyGlows01_Alpha.part

		POther	LarmL
		Lifespan 30
	End
End


Condition
	On	Time
	Time	25

	Event
		ENAME	LeftUarmGlow
		Type	Local
		At	Target
		part	:KineticIDBodySwirls01.part
		PMagnet LarmL
		Lifespan 20
	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	LarmL
		part	:KineticIDBodySwirls01.part
		PMagnet Target
		Lifespan 20
	End
End

## FLYAWAYS #######################################################

Condition
	On	Time
	Time	20

	Event
		ENAME	FlyawayNode
		Type	Local
		At	WepL
		Bhvr	CustomizeablePowers\Kinetics\KineticScaleSmall.bhvr
		LifeSpan	40
	End
End

Condition
	On	Time
	Time	20

	Event
		ENAME	Flyaways1
		Type	Start
		At	FlyawayNode
		part	:KineticSwirlFlyAway03.part
		Bhvr	CustomizeablePowers\Kinetics\KineticSpin3.bhvr
		LifeSpan	20
	End
End

Condition
	On	Time
	Time	25

	Event
		ENAME	Flyaways2
		Type	Start
		At	FlyawayNode
		part	:KineticSwirlFlyAway03.part
		Bhvr	CustomizeablePowers\Kinetics\KineticSpin3.bhvr
		LifeSpan	20
	End
End

Condition
	On	Time
	Time	30

	Event
		ENAME	Flyaways3
		Type	Start
		At	FlyawayNode
		part	:KineticSwirlFlyAway03.part
		Bhvr	CustomizeablePowers\Kinetics\KineticSpin3.bhvr
		LifeSpan	20
	End

End

#########################################################

End