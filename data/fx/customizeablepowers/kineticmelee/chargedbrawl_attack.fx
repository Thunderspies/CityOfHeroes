#############################################################
## ChargedBrawl_Attack.fx
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
		Part	:Flash_Core.part
		Part	:Flash_Glow.part

		Lifespan 	15
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
		Lifespan 	15
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
		Lifespan 	15
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
		Part	:Rings_Shockwave.part
		Lifespan 	5
	End
End
#############################################################

End