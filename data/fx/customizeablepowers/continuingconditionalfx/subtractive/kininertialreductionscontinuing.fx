#############################################################
## KinInertialReductionsContinuing.fx
#############################################################

FxInfo

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
		Sound kin2_loop 80 80 .22
	End
End


#########################################################
#################### BODY GLOW 1 ########################
#########################################################



Condition
	On	Time
	Time	0


	Event
		ENAME	ChestGlow
		Type	Local
		At	Hips
		part1	:KinIDBodyStreaks01.part
		POther Neck
	End

	Event
		ENAME	HeadGlow
		Type	Local
		At	Head
		part1	:KinIDBodyStreaks01.part
		POther Hair
	End


	Event
		ENAME	RightUarmGlow
		Type	Local
		At	SpadR
		part1	:KinIDBodyStreaks01.part
		POther LarmR
	End

	Event
		ENAME	RightWepGlow
		Type	Local
		At	WepR
		part1	:KinIDBodyStreaks01.part
		POther LarmR
	End

	Event
		ENAME	LeftWepGlow
		Type	Local
		At	WepL
		part1	:KinIDBodyStreaks01.part
		POther LarmL
	End

	Event
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		part1	:KinIDBodyStreaks01.part
		POther LarmL
	End


	Event
		ENAME	LeftULegGlow
		Type	Local
		At	LlegL
		part1	:KinIDBodyStreaks01.part
		POther Ulegl
	End


	Event
		ENAME	RightULegGlow
		Type	Local
		At	LlegR
		part1	:KinIDBodyStreaks01.part
		POther UlegR
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
		At	Chest
		Bhvr	POWERS/Kinetics/KineticScaleSmall.bhvr

	End

	Event
		ENAME	Swirl1
		Type	Local
		At	SpinNode
		Bhvr	POWERS/Kinetics/KineticSpin1.bhvr

	End

	Event
		ENAME	Swirl2
		Type	Local
		At	SpinNode
		Bhvr	POWERS/Kinetics/KineticSpin2.bhvr

	End

	Event
		ENAME	Swirl3
		Type	Local
		At	SpinNode
		Bhvr	POWERS/Kinetics/KineticSpin3.bhvr

	End

End

Condition
	On	Cycle
	Time	30

	Event
		ENAME	Flyaways1
		Type	Posit
		At	Swirl1
		part	:KineticIDSwirlFlyAway01.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 50

	End


End

Condition
	On	Cycle
	Time	25


	Event
		ENAME	Flyaways2
		Type	Posit
		At	Swirl2
		part	:KineticIDSwirlFlyAway01.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 50

	End


End

End