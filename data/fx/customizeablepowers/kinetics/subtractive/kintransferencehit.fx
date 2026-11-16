#############################################################
## KinTransferenceHit.fx
#############################################################

FxInfo
Lifespan 100

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
		Sound kinetic4 80 80 .8
	End
End

####################### Force ###########################

Condition
	On	Time
	Time	0

	Event
		Ename	GravitationalForce
		Type	Local
		At	Hips

		BhvrOverride
			PhysForceType		CCWSwirl
			PhysForceRadius		20
			PhysForceCentripetal	.1
			PhysForcePower		75
			PhysForcePowerJitter	5
		End

		Lifespan 40

	End

	Event
		Ename	LevitationForce
		Type	Local
		At	Root

		BhvrOverride
			PhysForceType		Up
			PhysForceRadius		15
			PhysForcePower		25
			PhysForcePowerJitter	5
		End

		Lifespan	30

	End
End

#########################################################
#################### BODY GLOW 1 ########################
#########################################################


Condition
	On	Time
	Time	0

	Event
		ENAME	HitSwirlNode
		Type	Posit
		At	Root
		Geom	RootToChestAdjustment
		Sound Heal3b 80 80 .5
		Lifespan 30

	End


	Event
		ENAME	HitSwirl1
		Type	Local
		At	HitSwirlNode
		part	:KineticTransferenceHitRingLrg01.part
		AltPiv	1
		Lifespan 30

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
		#Bhvr	:KineticScaleLarge.bhvr

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
		part1	:KineticBodyGlowsBlue01.part
		POther Neck
		Lifespan 45

	End

	Event
		ENAME	HeadGlow
		Type	Local
		At	Hair
		part1	:KineticBodyGlowsBlue01.part
		POther Chest
		Lifespan 40

	End


	Event
		ENAME	RightUarmGlow
		Type	Local
		At	SpadR
		part1	:KineticBodyGlowsBlue01.part
		POther LarmR
		Lifespan 40

	End


	Event
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		part1	:KineticBodyGlowsBlue01.part
		POther LarmR
		Lifespan 35

	End


	Event
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		part1	:KineticBodyGlowsBlue01.part
		POther LarmL
		Lifespan 40

	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part1	:KineticBodyGlowsBlue01.part
		POther LarmL
		Lifespan 35

	End


	Event
		ENAME	LeftULegGlow
		Type	Local
		At	LlegL
		part1	:KineticBodyGlowsBlue01.part
		POther Ulegl
		Lifespan 50

	End

	Event
		ENAME	LeftLLegGlow
		Type	Local
		At	FootL
		part1	:KineticBodyGlowsBlue01.part
		POther Llegl
		Lifespan 55

	End

	Event
		ENAME	RightULegGlow
		Type	Local
		At	LlegR
		part1	:KineticBodyGlowsBlue01.part
		POther UlegR
		Lifespan 50

	End

	Event
		ENAME	RightLLegGlow
		Type	Local
		At	FootR
		part1	:KineticBodyGlowsBlue01.part
		POther LlegR
		Lifespan 55

	End


End

#############################################################

End