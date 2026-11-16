#########################################################
##	template

FxInfo

Input
	Inpname Origin
End

Input
	Inpname Root
End

Input
	Inpname Mystic
End

Input
	Inpname Chest
End

Input
	Inpname FootR
End

Input
	Inpname FootL
End

Input
	Inpname HandR
End

Input
	Inpname HandL
End


#Condition
#	On	Time
#	Time	5
#
#	Event
#		ENAME	Vortex
#		Type	Posit
#		At	Root
#		part	:GCVortexGroundSmall.part
#		part	:GCGlowGroundSmall.part
#		Bhvr	Behaviors\GenericParticleFade.bhvr
#	End
#
#End

#Condition
#	On	Time
#	Time	0
#
##	Event
##		ENAME	Vortex
##		Type	Posit
##		At	Root
##		part1	:GCVortexGlowsUp2.part
##		#Pmagnet Chest
##		Bhvr	Behaviors\GenericParticleFade.bhvr
##		#Lookat	Camera
##		Sound	glow6_loop 80 30 .85
##	End
#
#	Event
#		Ename	GravitationalForce
#		Type	Local
#		At	Root
#
#		BhvrOverride
#			PositionOffset		0 3 0
#			PhysForceType		In
#			PhysForceRadius		20
#			PhysForcePower		75
#			PhysForcePowerJitter	10
#		End
#	End
#
#	Event
#		Ename	LiftForce
#		Type	Posit
#		At	Root
#
#		BhvrOverride
#			PhysForceType		Up
#			PhysForceRadius		6
#			PhysForcePower		100
#			PhysForcePowerJitter	10
#		End
#
#	End
#End


Condition
	On	Time
	Time	0

	Event
		ENAME	Vortex
		Type	Local
		At	Chest
		part	CustomizeablePowers\GravityControl\GCLiftingElement1out.part
		Pmagnet Chest
		BHVR	Behaviors\GenericParticleFade.bhvr
		#Lookat	Camera
		Lifespan 60
	End

End



Condition
	On	Time
	Time	0

	Event
		ENAME	Vortex
		Type	Local
		At	HandL
		part	CustomizeablePowers\GravityControl\GCLiftingElement1outSmall.part
		#part	:GCVortexSteamXSmall.part
		BHVR	Behaviors\GenericParticleFade.bhvr
		#Lookat	Camera
		Lifespan 60
	End

End

Condition
	On	Time
	Time	0

	Event
		ENAME	Vortex
		Type	Local
		At	HandR
		part	CustomizeablePowers\GravityControl\GCLiftingElement1outSmall.part
		#part	:GCVortexSteamXSmall.part
		BHVR	Behaviors\GenericParticleFade.bhvr
		#Lookat	Camera
		Lifespan 60
	End

End

Condition
	On	Time
	Time	0

	Event
		ENAME	Vortex
		Type	Local
		At	FootL
		part	CustomizeablePowers\GravityControl\GCLiftingElement1outSmall.part
		#part	:GCVortexSteamXSmall.part
		BHVR	Behaviors\GenericParticleFade.bhvr
		#Lookat	Camera
		#Lifespan 580
	End

End

Condition
	On	Time
	Time	0

	Event
		ENAME	Vortex
		Type	Local
		At	FootR
		part	CustomizeablePowers\GravityControl\GCLiftingElement1outSmall.part
		#part	:GCVortexSteamXSmall.part
		BHVR	Behaviors\GenericParticleFade.bhvr
		#Lookat	Camera
		#Lifespan 580
	End

End


End

