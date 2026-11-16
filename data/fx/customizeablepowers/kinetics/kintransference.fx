#############################################################
## KinTransference.fx
#############################################################

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
	Time	25

	Event
		ENAME	WepLGlow
		Type	Local
		At	WepL
		part	CustomizeablePowers\Kinetics\KineticHandGlows01.part
		Lifespan 35

	End

	Event
		ENAME	WepRGlow
		Type	Local
		At	WepR
		part	CustomizeablePowers\Kinetics\KineticHandGlows01.part
		Lifespan 35

	End
End

Condition
	On	Time
	Time	20

	Event
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		part	CustomizeablePowers\Kinetics\KineticBodyGlows01.part
		POther LarmL
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 40

	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		POther  LarmL
		part	CustomizeablePowers\Kinetics\KineticBodyGlows01.part
		POther LarmL
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 40

	End

	Event
		ENAME	RightUarmGlow
		Type	Local
		At	SpadR
		part	CustomizeablePowers\Kinetics\KineticBodyGlows01.part
		POther LarmR
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 40

	End

	Event
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		POther  LarmL
		part	CustomizeablePowers\Kinetics\KineticBodyGlows01.part
		POther LarmR
		#Bhvr	Behaviors/TelportationFade.bhvr
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
		part	CustomizeablePowers\Kinetics\KineticCastRaysBlue01.part
		PMagnet WepL
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 40

	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part	CustomizeablePowers\Kinetics\KineticIDBodySwirls01.part
		PMagnet Target
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 40

	End

	Event
		ENAME	LeftUarmGlow
		Type	Local
		At	Target
		part	CustomizeablePowers\Kinetics\KineticCastRaysBlue01.part
		PMagnet WepR
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 40

	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepR
		part	CustomizeablePowers\Kinetics\KineticIDBodySwirls01.part
		PMagnet Target
		#Bhvr	Behaviors/TelportationFade.bhvr
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
		part	CustomizeablePowers\Kinetics\KineticSwirlFlyAway03.part
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
		part	CustomizeablePowers\Kinetics\KineticSwirlFlyAway03.part
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
		At	FlyawayNodeL
		part	CustomizeablePowers\Kinetics\KineticSwirlFlyAway03.part
		Bhvr	CustomizeablePowers\Kinetics\KineticSpin3.bhvr
		LifeSpan 15
	End

	Event
		ENAME	FlyawaysR2
		Type	Local
		At	FlyawayNodeR
		part	CustomizeablePowers\Kinetics\KineticSwirlFlyAway03.part
		Bhvr	CustomizeablePowers\Kinetics\KineticSpin3.bhvr
		LifeSpan 15
	End

End

Condition
	On	Time
	Time	25

	Event
		ENAME	FlyawaysL3
		Type	Local
		At	FlyawayNodeL
		part	CustomizeablePowers\Kinetics\KineticSwirlFlyAway03.part
		Bhvr	CustomizeablePowers\Kinetics\KineticSpin3.bhvr
		LifeSpan 15
	End

	Event
		ENAME	FlyawaysR3
		Type	Local
		At	FlyawayNodeR
		part	CustomizeablePowers\Kinetics\KineticSwirlFlyAway03.part
		Bhvr	CustomizeablePowers\Kinetics\KineticSpin3.bhvr
		LifeSpan 15
	End

End


Condition
	On	Time
	Time	70

	Event
		ENAME	All
		Type	Destroy
	End

End

End
