#############################################################
## KinInertialReductions.fx
#############################################################

FxInfo
Lifespan 50

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


	Event
		Ename	LevitationForce
		Type	Local
		At	Root

		BhvrOverride
			pyrRotate		180 0 0
			PhysForceType		Up
			PhysForceRadius		25
			PhysForcePower		75
			PhysForcePowerJitter	15
		End

		Lifespan	75
	End


	Event
		Ename	radiatingForce
		Type	Local
		At	Head

		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		20
			PhysForcePower		30
			PhysForcePowerJitter	15
		End

		Lifespan	60

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
		Bhvr	POWERS\Kinetics\KineticScaleSmall.bhvr
		LifeSpan 50
	End

	Event
		ENAME	FlyawaysL1
		Type	Local
		At	FlyawayNodeL
		part	:KineticSwirlFlyAway03.part
		Bhvr	POWERS\Kinetics\KineticSpin3.bhvr
		LifeSpan 15
	End

	Event
		ENAME	FlyawayNodeR
		Type	Local
		At	WepR
		Bhvr	POWERS\Kinetics\KineticScaleSmall.bhvr
		LifeSpan 50
	End

	Event
		ENAME	FlyawaysR1
		Type	Local
		At	FlyawayNodeR
		part	:KineticSwirlFlyAway03.part
		Bhvr	POWERS\Kinetics\KineticSpin3.bhvr
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
		part	:KineticSwirlFlyAway03.part
		Bhvr	POWERS\Kinetics\KineticSpin3.bhvr
		LifeSpan 15
	End

	Event
		ENAME	FlyawaysR2
		Type	Local
		At	FlyawayNodeR
		part	:KineticSwirlFlyAway03.part
		Bhvr	POWERS\Kinetics\KineticSpin3.bhvr
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
		part	:KineticSwirlFlyAway03.part
		Bhvr	POWERS\Kinetics\KineticSpin3.bhvr
		LifeSpan 15
	End

	Event
		ENAME	FlyawaysR3
		Type	Local
		At	FlyawayNodeR
		part	:KineticSwirlFlyAway03.part
		Bhvr	POWERS\Kinetics\KineticSpin3.bhvr
		LifeSpan 15
	End

End

#########################################################
####################    GYRO 1   ########################
#########################################################


Condition
	On	Time
	Time	20

	Event
		ENAME	SpinNode
		Type	Local
		At	Chest
		Bhvr	POWERS\Kinetics\KineticScaleLarge.bhvr

	End

	Event
		ENAME	Swirl1
		Type	Local
		At	SpinNode
		part	:KinecTestSwirl02.part
		part	:KinecTestSwirl01.part
		Bhvr	POWERS\Kinetics\KineticSpin1.bhvr
		Lifespan 15

	End

	Event
		ENAME	Swirl2
		Type	Local
		At	SpinNode
		part	:KinecTestSwirl02.part
		part	:KinecTestSwirl01.part
		Bhvr	POWERS\Kinetics\KineticSpin2.bhvr
		Lifespan 15

	End

	Event
		ENAME	Swirl3
		Type	Local
		At	SpinNode
		part	:KinecTestSwirl02.part
		part	:KinecTestSwirl01.part
		Bhvr	POWERS\Kinetics\KineticSpin3.bhvr
		Lifespan 15

	End

	Event
		ENAME	Glows1
		Type	Local
		At	SpinNode
		part	:KineticExpandingGlows01.part
		part	:KineticExpandingRings01.part
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Lifespan 15

	End

	Event
		ENAME	Flyaways1
		Type	Start
		At	Swirl1
		part	:KineticSwirlFlyAway01.part
		Lifespan 15

	End

	Event
		ENAME	Flyaways2
		Type	Start
		At	Swirl2
		part	:KineticSwirlFlyAway02.part
		Lifespan 15

	End


End

#########################################################
####################    SWIRLS 1   ######################
#########################################################

Condition
	On	Time
	Time	30

	Event
		ENAME	BigSwirlNode
		Type	Posit
		At	Root
		Geom	RootToChestAdjustment
	End

	Event
		ENAME	BigSwirl
		Type	Local
		At	BigSwirlNode
		AltPiv	1
		Part	:KineticIDCastSwirlLrg01.part
	End

End

#############################################################

End
