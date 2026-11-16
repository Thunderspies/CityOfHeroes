#########################################################
##	template

FxInfo

Input
	Inpname Origin
End

Input
	Inpname Mystic
End

Input
	Inpname Chest
End

Lifespan 600

Condition
	On	Time
	Time	0
		
	Event	
		ENAME	Vortex
		Type	Local
		At	Chest
		part1	:GCVortexSteam.part
	End

	Event
		Ename	GravitationalImpulse
		Type	Local
		At	Chest
		
		BhvrOverride
			PhysForceType		In
			PhysForceRadius		30
			PhysForcePower		150
			PhysForcePowerJitter	25
		End

		Lifespan	3

	End

	Event
		Ename	GravitationalForce
		Type	Local
		At	Chest
		
		BhvrOverride
			PhysForceType		In
			PhysForceRadius		20
			PhysForcePower		60
			PhysForcePowerJitter	10
		End
	End

	Event
		Ename	LiftForce
		Type	Local
		At	Root
		
		BhvrOverride
			PhysForceType		Up
			PhysForceRadius		6
			PhysForcePower		40
			PhysForcePowerJitter	10
		End

	End

End

Condition
	On	Time
	Time	5
		
	Event	
		ENAME	Vortex
		Type	Local
		At	Chest
		part	:GCVortex.part
		#Bhvr	Behaviors/GCVortexSpin.bhvr
		Lifespan 560
	End

End

Condition
	On	Time
	Time	10
		
	Event	
		ENAME	Vortex
		Type	Local
		At	Chest
		part1	:GCVortexGlows.part
		Pmagnet Chest
		#Bhvr	Behaviors/GCVortexSpin.bhvr
		#Lookat	Camera
		Lifespan 580
	End

End

	
End	

	