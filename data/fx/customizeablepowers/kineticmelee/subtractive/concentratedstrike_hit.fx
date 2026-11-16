#############################################################
## KineticMelee_Hit.fx
#############################################################

FxInfo
Lifespan 40

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	ShockwaveAnchor
		Type	Local
		At	Chest
		Sound KM_ConcentratedStrike_Hit_01 200 50 1
		Part	:Rings_Shockwave_Small.part
		Part	:Rings_Shockwave_Smokey.part
		Part	:Flash_Streaks.part
		Lifespan	5
	End

	Event
		Type	Local
		At	Chest
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PyrRotate	60 30 30
			PyrRotateJitter 10 0 10
		End
		Part	:Rings_Burst_LocalFacing.part
		Part	:Rings_Burst_LocalFacing_Small.part
		Part	:Flash_Core.part
		Part	:Flash_Glow.part
		Sound PP11 200 100 0.7
		Lifespan 	25
		LifespanJitter	5
	End

	Event
		Type	PositOnly
		At	Chest
		BhvrOverride
			PyrRotate	100 0 -30
			PyrRotateJitter 10 0 10

		End
		Part	:Rings_Burst_LocalFacing.part
		Part	:Rings_Burst_LocalFacing_Small.part
		Lifespan 	27
		LifespanJitter	5
	End

	Event
		Type	PositOnly
		At	Chest
		BhvrOverride
			PyrRotate	0 120 0
			PyrRotateJitter 20 0 20

		End
		Part	:Rings_Burst_LocalFacing.part
		Part	:Rings_Burst_LocalFacing_Small.part
		Lifespan 	29
		LifespanJitter	5
	End
End

Condition
	On	Cycle
	Time	2

	Event
		Type	Local
		At	ShockwaveAnchor
		BhvrOverride
			PyrRotateJitter 180 180 180
		End
		Part	:Rings_Burst_LocalFacing_Rapid.part
		Lifespan 	5
	End
End

#############################################################

End