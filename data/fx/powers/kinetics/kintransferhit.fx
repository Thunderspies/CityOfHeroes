#############################################################
## KinTransferHit.fx
#############################################################

FxInfo

Lifespan 100

#########################################################

Input
	Inpname	Hips
End

#########################################################

Condition
	On	Time
	Time	0

	Event
		ENAME	HitSwirlNode
		Type	Posit
		At	Root
		Geom	RootToChestAdjustment
		Lifespan 30
	End

	Event
		ENAME	HitSwirl1
		Type	Local
		At	HitSwirlNode
		AltPiv	1
		part	:KineticTransferHitRingLrg01.part
		Lifespan 30
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		ENAME	SpinNode
		Type	Local
		At	Hips
	End

	Event
		ENAME	Swirl1
		Type	Local
		At	SpinNode
		Bhvr	:KineticSpin1.bhvr
		part	:KinecTestSwirl02.part
		part	:KinecTestSwirl01.part
		Lifespan 20
	End

	Event
		ENAME	Swirl2
		Type	Local
		At	SpinNode
		Bhvr	:KineticSpin2.bhvr
		part	:KinecTestSwirl02.part
		part	:KinecTestSwirl01.part
		Lifespan 20
	End

	Event
		ENAME	Swirl3
		Type	Local
		At	SpinNode
		Bhvr	:KineticSpin3.bhvr
		part	:KinecTestSwirl02.part
		part	:KinecTestSwirl01.part
		Lifespan 20
	End

	Event
		ENAME	Glows1
		Type	Local
		At	SpinNode
		Bhvr	Behaviors/GenericParticleFade.bhvr
		part	:KineticExpandingGlows01.part
		part	:KineticExpandingRings01.part
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

#############################################################

#Condition
#	On	Time
#	Time	0
#
#	Event
#		ENAME	ChestGlow
#		Type	Local
#		At	Hips
#		part1	:KineticBodyGlowsBlue01.part
#		POther Neck
#		Lifespan 45
#	End
#
#	Event
#		ENAME	HeadGlow
#		Type	Local
#		At	Hair
#		part1	:KineticBodyGlowsBlue01.part
#		POther Chest
#		Lifespan 40
#	End
#
#	Event
#		ENAME	RightUarmGlow
#		Type	Local
#		At	SpadR
#		part1	:KineticBodyGlowsBlue01.part
#		POther LarmR
#		Lifespan 40
#	End
#
#	Event
#		ENAME	RightLarmGlow
#		Type	Local
#		At	WepR
#		part1	:KineticBodyGlowsBlue01.part
#		POther LarmR
#		Lifespan 35
#	End
#
#	Event
#		ENAME	LeftUarmGlow
#		Type	Local
#		At	SpadL
#		part1	:KineticBodyGlowsBlue01.part
#		POther LarmL
#		Lifespan 40
#	End
#
#	Event
#		ENAME	LeftLarmGlow
#		Type	Local
#		At	WepL
#		part1	:KineticBodyGlowsBlue01.part
#		POther LarmL
#		Lifespan 35
#	End
#
#	Event
#		ENAME	LeftULegGlow
#		Type	Local
#		At	LlegL
#		part1	:KineticBodyGlowsBlue01.part
#		POther Ulegl
#		Lifespan 50
#	End
#
#	Event
#		ENAME	LeftLLegGlow
#		Type	Local
#		At	FootL
#		part1	:KineticBodyGlowsBlue01.part
#		POther Llegl
#		Lifespan 55
#	End
#
#	Event
#		ENAME	RightULegGlow
#		Type	Local
#		At	LlegR
#		part1	:KineticBodyGlowsBlue01.part
#		POther UlegR
#		Lifespan 50
#	End
#
#	Event
#		ENAME	RightLLegGlow
#		Type	Local
#		At	FootR
#		part1	:KineticBodyGlowsBlue01.part
#		POther LlegR
#		Lifespan 55
#	End
#End

## PHYSICS ###########################################################

#Condition
#	On	Time
#	Time	0
#
#	Event
#		Ename	GravitationalForce
#		Type	Local
#		At	Hips
#		BhvrOverride
#			PhysForceType		CCWSwirl
#			PhysForceRadius		12
#			PhysForceCentripetal	.25
#			PhysForcePower		100
#			PhysForcePowerJitter	5
#		End
#		Lifespan 30
#	End
#
#	Event
#		Ename	LevitationForce
#		Type	Local
#		At	Root
#		BhvrOverride
#			PhysForceType		Up
#			PhysForceRadius		10
#			PhysForcePower		25
#			PhysForcePowerJitter	5
#		End
#		Lifespan	30
#	End
#End

#############################################################

End