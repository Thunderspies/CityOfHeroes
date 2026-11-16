#########################################################
##	template

FxInfo
Lifespan 300

#########################################################
####################### AUDIO ###########################
#########################################################


Condition
	On	Time
	Time	15


	Event
		Type	Local
		At 	Origin
		Sound kinetic2 80 80 .8
	End
End


#########################################################
####################     CAST  1   ######################
#########################################################

Condition
	On	Time
	Time	20

	Event
		ENAME	WepLGlow
		Type	Local
		At	WepR
		part	CustomizeablePowers\Kinetics\KineticHandGlowsCyan01.part
		Lifespan 10

	End

End

Condition
	On	Time
	Time	15

	Event
		ENAME	RightUarmGlow
		Type	Local
		At	SpadR
		part	CustomizeablePowers\Kinetics\KineticBodyGlowsCyan01.part
		POther LarmR
		Lifespan 20

	End

	Event
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		POther  LarmR
		part	CustomizeablePowers\Kinetics\KineticBodyGlowsCyan01.part
		POther LarmR
		Lifespan 20

	End

End

#########################################################
####################     CAST  2   ######################
#########################################################

Condition
	On	Time
	Time	15


	Event
		ENAME	RightUarmGlow
		Type	Local
		At	Target
		part	CustomizeablePowers\Kinetics\KineticBodySwirlsCyan01.part
		PMagnet LarmR
		Lifespan 10

	End

	Event
		ENAME	RightLarmGlow
		Type	Local
		At	LarmR
		part	CustomizeablePowers\Kinetics\KineticBodySwirlsCyan01.part
		PMagnet Target
		Lifespan 10

	End

End

#########################################################
####################   Flyaways   #######################
#########################################################


Condition
	On	Time
	Time	5

	Event
		ENAME	FlyawayNode
		Type	Local
		At	WepR
		Bhvr	CustomizeablePowers\Kinetics\KineticScaleSmall.bhvr
		LifeSpan 50
	End

	Event
		ENAME	Flyaways1
		Type	Local
		At	FlyawayNode
		part	CustomizeablePowers\Kinetics\KineticSwirlFlyAway03.part
		Bhvr	CustomizeablePowers\Kinetics\KineticSpin3.bhvr
		LifeSpan 5
	End

End

Condition
	On	Time
	Time	10

	Event
		ENAME	Flyaways2
		Type	Local
		At	FlyawayNode
		part	CustomizeablePowers\Kinetics\KineticSwirlFlyAway03.part
		Bhvr	CustomizeablePowers\Kinetics\KineticSpin3.bhvr
		LifeSpan 5
	End

End

Condition
	On	Time
	Time	15

	Event
		ENAME	Flyaways3
		Type	Local
		At	FlyawayNode
		part	CustomizeablePowers\Kinetics\KineticSwirlFlyAway03.part
		Bhvr	CustomizeablePowers\Kinetics\KineticSpin3.bhvr
		LifeSpan 5
	End

End

End