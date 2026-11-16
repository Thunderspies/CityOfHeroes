#############################################################
## KinIncreaseDensityContinuing.fx
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
		Sound kin2_loop 40 40 .24
	End
End


#########################################################
#################### BODY GLOW 1 ########################
#########################################################



Condition
	On	Time
	Time	30

	Event
		Ename	GravitationalForce
		Type	Local
		At	Hips

		BhvrOverride
			PhysForceType		In
			PhysForceRadius		10
			PhysForcePower		25
			PhysForcePowerJitter	5
		End

	End

	Event
		Ename	LevitationForce
		Type	Local
		At	Root

		BhvrOverride
			PositionOffset		0 3 0
			PhysForceType		Up
			PhysForceRadius		10
			PhysForcePower		-50
			PhysForcePowerJitter	5
		End

	End
End



#########################################################
#################### BODY GLOW 2 ########################
#########################################################



Condition
	On	Cycle
	Time	45


	Event
		ENAME	ChestGlow
		Type	Local
		At	Hips
		part1	:KineticBodyGlows02.part
		POther Neck
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 20

	End

	Event
		ENAME	HeadGlow
		Type	Local
		At	Hair
		part1	:KineticBodyGlows02.part
		POther Chest
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 20

	End


	Event
		ENAME	RightUarmGlow
		Type	Local
		At	SpadR
		part1	:KineticBodyGlows02.part
		POther LarmR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 20

	End


	Event
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		part1	:KineticBodyGlows02.part
		POther LarmR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 20

	End


	Event
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		part1	:KineticBodyGlows02.part
		POther LarmL
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 20

	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part1	:KineticBodyGlows02.part
		POther LarmL
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 20

	End


	Event
		ENAME	LeftULegGlow
		Type	Local
		At	LlegL
		part1	:KineticBodyGlows02.part
		POther Ulegl
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 20

	End

	Event
		ENAME	LeftLLegGlow
		Type	Local
		At	FootL
		part1	:KineticBodyGlows02.part
		POther Llegl
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 20

	End

	Event
		ENAME	RightULegGlow
		Type	Local
		At	LlegR
		part1	:KineticBodyGlows02.part
		POther UlegR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 20

	End

	Event
		ENAME	RightLLegGlow
		Type	Local
		At	FootR
		part1	:KineticBodyGlows02.part
		POther LlegR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 20

	End

End

Condition
	On	Cycle
	Time	30

	Event
		ENAME	FlyawaysNode1
		Type	Local
		At	Hips
		Bhvr	POWERS/Kinetics/KineticSpin2.bhvr
		Lifespan 50

	End

	Event
		ENAME	Flyaways1
		Type	Posit
		At	FlyawaysNode1
		part	:KineticSwirlOrbitIn01.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 50

	End

End

Condition
	On	Cycle
	Time	45

	Event
		ENAME	FlyawaysNode2
		Type	Local
		At	Hips
		Bhvr	POWERS/Kinetics/KineticSpin1.bhvr
		Lifespan 50

	End

	Event
		ENAME	Flyaways2
		Type	Posit
		At	FlyawaysNode2
		part	:KineticSwirlOrbitIn02.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 50

	End


End


End