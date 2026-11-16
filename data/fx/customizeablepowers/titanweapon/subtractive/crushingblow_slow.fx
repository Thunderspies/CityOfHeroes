#############################################################
## CrushingBlow_Slow.fx
#############################################################

FxInfo
	LifeSpan 100

###########################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	TW_CrushingBlow_AttackSlow_01 100 100 .8
	End
End

Condition
	On Time
	Time 43

	Event
		At C_Chest
		Type Local
		Bhvr Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset -1.5 0 -2
			PyrRotate 0 -130 0
		End
		Part :Swoosh_Trail_Short.part
		Lifespan 2
	End
End

Condition
	On Time
	Time 57

	Event
		At C_Chest
		Type Local
		Bhvr Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset 1.5 0 -2
			PyrRotate 180 -60 0
		End
		Part :Swoosh_Trail.part
		Lifespan 5
	End
End

Condition
	On Time
	Time 59

	Event
		At 	Chest
		Type 	Local
		Bhvr 	:Swoosh_Projectile.bhvr
		BhvrOverride
			PyrRotate 90 90 0
		End
		Part 	:Titan_Swoosh_Vertical.part
		LifeSpan 300
	End
End

Condition
	On Time
	Time 63

	Event
		At 	Root
		Type 	Local
		EName 	Prime
		BhvrOverride
			PositionOffset 0 0 12
		End
		Part 	:Dust_Circle_Small.part
		Part 	:Dust_Shockwave_Small.part
		LifeSpan 8
	End
End
End
