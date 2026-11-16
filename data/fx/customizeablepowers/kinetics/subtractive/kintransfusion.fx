#############################################################
## KinTransfusion.fx
#############################################################

FxInfo
Lifespan 300

#########################################################
####################### AUDIO ###########################
#########################################################


Condition
	On	Time
	Time	0

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
		At	WepL
		part	:KineticHandGlows01.part
		Lifespan 35

	End

End

Condition
	On	Time
	Time	15

	Event
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		part	:KineticBodyGlows01.part
		part	:KineticBodyGlows01_Alpha.part

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

End

#########################################################
####################     CAST  2   ######################
#########################################################

Condition
	On	Time
	Time	20


	Event
		ENAME	LeftUarmGlow
		Type	Local
		At	Target
		part	:KineticCastRaysBlue01.part
		PMagnet LarmL
		Lifespan 40

	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	LarmL
		part	:KineticIDBodySwirls01.part
		PMagnet Target
		Lifespan 40

	End

End

#########################################################
####################   Flyaways   #######################
#########################################################


Condition
	On	Time
	Time	15

	Event
		ENAME	FlyawayNode
		Type	Local
		At	WepL
		Bhvr	CustomizeablePowers\Kinetics\KineticScaleSmall.bhvr
		LifeSpan 50
	End

	Event
		ENAME	Flyaways1
		Type	Local
		At	FlyawayNode
		part	:KineticSwirlFlyAway03.part
		Bhvr	CustomizeablePowers\Kinetics\KineticSpin3.bhvr
		LifeSpan 15
	End

End

Condition
	On	Time
	Time	20

	Event
		ENAME	Flyaways2
		Type	Local
		At	FlyawayNode
		part	:KineticSwirlFlyAway03.part
		Bhvr	CustomizeablePowers\Kinetics\KineticSpin3.bhvr
		LifeSpan 15
	End

End

Condition
	On	Time
	Time	25

	Event
		ENAME	Flyaways3
		Type	Local
		At	FlyawayNode
		part	:KineticSwirlFlyAway03.part
		Bhvr	CustomizeablePowers\Kinetics\KineticSpin3.bhvr
		LifeSpan 15
	End

End

End