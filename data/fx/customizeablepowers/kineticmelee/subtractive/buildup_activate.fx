#############################################################
## BuildUp_Activate.fx
#############################################################

FxInfo
Lifespan 45

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	ShockwaveAnchor
		Type	Local
		At	Chest
		Part	:Flash_Streaks.part
		Part	:Flash_Core.part
		Part	:Flash_Glow.part
		Sound KM_BuildUp_Activate_01 100 100 .8
		Lifespan	5
	End
End

Condition
	On	Time
	Time	4

	Event
		EName	ShockwaveAnchor
		Type	Local
		At	Chest
		Part	:Rings_Shockwave_Small.part
		Lifespan	5
	End
End
Condition
	On	Cycle
	Time	4

	Event
		Type	Local
		At	Chest
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PyrRotate	60 30 30
			PyrRotateJitter 20 0 20
		End
		Part	:Rings_Burst_LocalFacing.part


		Lifespan 	1
	End
End

Condition
	On	Cycle
	Time	4

	Event
		Type	PositOnly
		At	Chest
		BhvrOverride
			PyrRotate	100 0 -30
			PyrRotateJitter 20 0 20

		End
		Part	:Rings_Burst_LocalFacing.part
		Lifespan 	1
	End
End

Condition
	On	Cycle
	Time	4

	Event
		Type	PositOnly
		At	Chest
		BhvrOverride
			PyrRotate	0 120 0
			PyrRotateJitter 20 0 20

		End
		Part	:Rings_Burst_LocalFacing.part
		Lifespan 	1
	End
End

Condition
	On	Time
	Time	15

	Event
		Type 	local
		At	Root
		ChildFX	POWERS\Child_BuildUp.fx
	End
End

#############################################################

End