#############################################################
## GCDistortionFieldContinuing.fx
#############################################################

FxInfo

#############################################################

Input
	Inpname Root
End

#############################################################

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
		part1	:GCVortexGlowsUp2.part
		#Pmagnet Chest
		Bhvr	Behaviors/GenericParticleFade.bhvr
		#Lookat	Camera
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

#Condition
#	On	Time
#	Time	0
#
#	Event
#		ENAME	Vortex
#		Type	Local
#		At	Chest
#		part	:GCLiftingElement1out.part
#		Pmagnet Chest
#		Bhvr	Behaviors/GenericParticleFade.bhvr
#		#Lookat	Camera
#		Lifespan 60
#	End
#
#End
#
#
#
#Condition
#	On	Time
#	Time	0
#
#	Event
#		ENAME	Vortex
#		Type	Local
#		At	HandL
#		part	:GCLiftingElement1outSmall.part
#		#part	:GCVortexSteamXSmall.part
#		Bhvr	Behaviors/GenericParticleFade.bhvr
#		#Lookat	Camera
#		Lifespan 60
#	End
#
#End
#
#Condition
#	On	Time
#	Time	0
#
#	Event
#		ENAME	Vortex
#		Type	Local
#		At	HandR
#		part	:GCLiftingElement1outSmall.part
#		#part	:GCVortexSteamXSmall.part
#		Bhvr	Behaviors/GenericParticleFade.bhvr
#		#Lookat	Camera
#		Lifespan 60
#	End
#
#End


End

