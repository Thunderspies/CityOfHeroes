#############################################################
## GCDistortionFieldContinuing.fx
#############################################################

FxInfo

#############################################################

Input
	Inpname	Hips
End

#########################################################



Condition
	On	Time
	Time	5

	Event
		ENAME	Vortex
		Type	Posit
		At	Root
		part	:GCVortexGroundSmall.part
		part	:GCGlowGroundSmall.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
	End

End

Condition
	On	Time
	Time	0

	Event
		ENAME	Vortex
		Type	Posit
		At	Root
		part	:GCVortexGlowsUp2.part
		part	:GCVortexGlowsUp2_Alpha.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Sound glow6_loop 80 80 .85
	End

	Event
		Ename	GravitationalForce
		Type	Local
		At	Root

		BhvrOverride
			PositionOffset		0 3 0
			PhysForceType		In
			PhysForceRadius		20
			PhysForcePower		75
			PhysForcePowerJitter	10
		End
	End

	Event
		Ename	LiftForce
		Type	Posit
		At	Root

		BhvrOverride
			PhysForceType		Up
			PhysForceRadius		6
			PhysForcePower		100
			PhysForcePowerJitter	10
		End

	End
End

#############################################################

End

