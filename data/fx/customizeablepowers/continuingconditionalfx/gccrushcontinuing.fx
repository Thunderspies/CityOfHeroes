#############################################################
## GCCrushContinuing.fx
#############################################################

FxInfo

#############################################################

Input
	Inpname	Hips
End

#############################################################


Condition
	On	Time
	Time	0

	Event
		ENAME	Vortex
		Type	Posit
		At	Chest
		part1	:GCVortexSteamSmall.part
		Sound glow6_loop 80 80 .85
	End


	Event
		Ename	GravitationalForce
		Type	Local
		At	Chest

		BhvrOverride
			PhysForceType		In
			PhysForceRadius		10
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
			PhysForcePower		30
			PhysForcePowerJitter	10
		End

	End

End

Condition
	On	Time
	Time	10

	Event
		ENAME	Vortex
		Type	Local
		At	Chest
		part1	:GCVortexGlowsSmall.part
		Pmagnet Chest
	End

End

Condition
	On	Time
	Time	0

	Event
		ENAME	Vortex
		Type	Local
		At	Chest
		part	:GCCrushSwirl01.part
	End

	Event
		ENAME	Vortex
		Type	Local
		At	Head
		part	:GCCrushSwirl01.part
	End

	Event
		ENAME	Vortex
		Type	Local
		At	WepL
		part	:GCCrushSwirl01.part
	End

	Event
		ENAME	Vortex
		Type	Local
		At	WepR
		part	:GCCrushSwirl01.part
	End

	Event
		ENAME	Vortex
		Type	Local
		At	LlegL
		part	:GCCrushSwirl01.part
	End

	Event
		ENAME	Vortex
		Type	Local
		At	LlegR
		part	:GCCrushSwirl01.part
	End

End


End

