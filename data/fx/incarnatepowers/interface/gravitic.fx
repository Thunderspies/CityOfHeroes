#############################################################
## GCCrushContinuing.fx
#############################################################

FxInfo

#############################################################

Input
	Inpname Hips
End

#############################################################

Condition
	On	Time
	Time	3

	Event
		Type	Local
		At	Chest
		part	:Flash.part
		Lifespan 5
	End

End

Condition

	On	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	Incarnate_Gravitic_Loop 100 100 .3
		bhvr	behaviors/Sound/SoundIn05Out10.bhvr
		lifespan 150

	End

	Event
		ENAME	Vortex
		Type	Posit
		At	Chest
		part1	:GravSteam.part
		part2	:GoldenCenter.part
		#Sound glow6_loop 80 80 .85
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
		ENAME	Vortex
		Type	Local
		At	Chest
		part	:GravSwirl.part
	End


End


#############################################################

End

