#############################################################
## KinSiphonSpeedHit.fx
#############################################################

FxInfo
Lifespan 300

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
		Sound kinetic5 80 80 .7
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
		Lifespan 50

	End

	Event
		ENAME	Swirl2
		Type	Local
		At	SpinNode
		part	:KinecTestSwirl02.part
		part	:KinecTestSwirl01.part
		Bhvr	CustomizeablePowers\Kinetics\KineticSpin2.bhvr
		Lifespan 50

	End

	Event
		ENAME	Swirl3
		Type	Local
		At	SpinNode
		part	:KinecTestSwirl02.part
		part	:KinecTestSwirl01.part
		Bhvr	CustomizeablePowers\Kinetics\KineticSpin3.bhvr
		Lifespan 50

	End

	Event
		ENAME	Glows1
		Type	Local
		At	SpinNode
		part	:KineticExpandingGlows01.part
		part	:KineticExpandingRings01.part
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Lifespan 50

	End

	Event
		ENAME	Flyaways1
		Type	Start
		At	Swirl1
		part	:KineticSwirlFlyAway01.part
		Lifespan 50

	End

	Event
		ENAME	Flyaways2
		Type	Start
		At	Swirl2
		part	:KineticSwirlFlyAway02.part
		Lifespan 50
	End
End

#############################################################

End