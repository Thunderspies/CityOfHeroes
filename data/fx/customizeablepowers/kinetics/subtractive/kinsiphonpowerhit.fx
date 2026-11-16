#############################################################
## KinSiphonPowerHit.fx
#############################################################

FxInfo
Lifespan 200

#########################################################

Input
	Inpname	Hips
End

#########################################################

#########################################################
####################### AUDIO ###########################
#########################################################


Condition
	On	Time
	Time	0


	Event
		Type	Local
		At 	Origin
		Sound kinetic4 80 80 .84
	End
End


#########################################################
#################### BODY GLOW 1 ########################
#########################################################


Condition
	On	Time
	Time	0

	Event
		ENAME	SpinNode
		Type	Local
		At	Hips
		Bhvr	CustomizeablePowers\Kinetics\KineticScale.bhvr

	End

	Event
		ENAME	Swirl1
		Type	Local
		At	SpinNode
		part	:KinecTestSwirl02.part
		part	:KinecTestSwirl01.part
		Bhvr	CustomizeablePowers\Kinetics\KineticSpin1.bhvr
		Lifespan 20

	End

	Event
		ENAME	Swirl2
		Type	Local
		At	SpinNode
		part	:KinecTestSwirl02.part
		part	:KinecTestSwirl01.part
		Bhvr	CustomizeablePowers\Kinetics\KineticSpin2.bhvr
		Lifespan 20

	End

	Event
		ENAME	Swirl3
		Type	Local
		At	SpinNode
		part	:KinecTestSwirl02.part
		part	:KinecTestSwirl01.part
		Bhvr	CustomizeablePowers\Kinetics\KineticSpin3.bhvr
		Lifespan 20

	End

	Event
		ENAME	Glows1
		Type	Local
		At	SpinNode
		part	:KineticExpandingGlows01.part
		#part	:KineticExpandingGlows01_Alpha.part

		part	:KineticExpandingRings01.part
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Lifespan 20

	End

	Event
		ENAME	Flyaways1
		Type	Start
		At	Swirl1
		part	:KineticSwirlFlyAway01.part
		Lifespan 20

	End

	Event
		ENAME	Flyaways2
		Type	Start
		At	Swirl2
		part	:KineticSwirlFlyAway02.part
		Lifespan 20

	End


End

#########################################################
#################### BODY GLOW 2 ########################
#########################################################



Condition
	On	Time
	Time	0


	Event
		ENAME	ChestGlow
		Type	Local
		At	Hips
		part1	:KineticBodyGlowsRed01.part
		POther Neck
		Lifespan 45

	End

	Event
		ENAME	HeadGlow
		Type	Local
		At	Hair
		part1	:KineticBodyGlowsRed01.part
		POther Chest
		Lifespan 40

	End


	Event
		ENAME	RightUarmGlow
		Type	Local
		At	SpadR
		part1	:KineticBodyGlowsRed01.part
		POther LarmR
		Lifespan 40

	End


	Event
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		part1	:KineticBodyGlowsRed01.part
		POther LarmR
		Lifespan 35

	End


	Event
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		part1	:KineticBodyGlowsRed01.part
		POther LarmL
		Lifespan 40

	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part1	:KineticBodyGlowsRed01.part
		POther LarmL
		Lifespan 35

	End


	Event
		ENAME	LeftULegGlow
		Type	Local
		At	LlegL
		part1	:KineticBodyGlowsRed01.part
		POther Ulegl
		Lifespan 50

	End

	Event
		ENAME	LeftLLegGlow
		Type	Local
		At	FootL
		part1	:KineticBodyGlowsRed01.part
		POther Llegl
		Lifespan 55

	End

	Event
		ENAME	RightULegGlow
		Type	Local
		At	LlegR
		part1	:KineticBodyGlowsRed01.part
		POther UlegR
		Lifespan 50

	End

	Event
		ENAME	RightLLegGlow
		Type	Local
		At	FootR
		part1	:KineticBodyGlowsRed01.part
		POther LlegR
		Lifespan 55

	End


End


End