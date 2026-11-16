#############################################################
## Defensive_Sweep_Slow.fx
#############################################################

FxInfo
Lifespan 100
################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	TW_CrushingBlow_AttackFast_01 100 100 .8
	End
End

Condition
	On Time
	Time 14

	Event
		At C_Chest
		Type Local
		Bhvr Behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset -1.5 0 -2
			PyrRotate 0 -130 0
		End
		Part :Swoosh_Trail_Short.part
		Lifespan 3
	End
End

Condition
	On Time
	Time 30

	Event
		At C_Chest
		Type Local
		Bhvr Behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset 1.5 0 -2
			PyrRotate 180 -90 0
		End
		Part :Swoosh_Trail.part
		Lifespan 3
	End
End

Condition
	On	Time
	Time	35

	Event
		At 	Root
		Type 	Local
		EName 	Prime
		BhvrOverride
			PositionOffset 1.5 0 9
		End
		Part 	:Dust_Circle_Small.part
		Part 	:Dust_Shockwave_Small.part
		LifeSpan 8
	End
End

End
