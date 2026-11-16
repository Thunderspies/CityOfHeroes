#############################################################
## SweepingStrike_Fast.fx
#############################################################

FxInfo
Lifespan 100
#########################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	TW_SweepingStrike_AttackFast_01 100 100 .8
	End
End

Condition
	On Time
	Time 10

	Event
		At C_Chest
		Type Local
		Bhvr Behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset 1.5 0 -2
			PyrRotate 180 -60 0
		End
		Part :Swoosh_Trail.part
		Lifespan 3
	End
End

Condition
	On	Time
	Time	15

	Event
		At T_Chest
		Type Start
		EName ConeTarget
		BhvrOverride
			PositionOffset 0 -1 0
		End
	End

	Event
		At Chest
		Type Start
		EName Prime
		LookAt ConeTarget
		Magnet ConeTarget
		Bhvr :Swoosh_Projectile.bhvr
		Part :Titan_Swoosh_Horizontal.part
		LifeSpan 3
	End

	Event
		At 	Root
		Type 	Local
		EName 	AirRush1
		Bhvr 	behaviors\Braz\ExpandRingFast.bhvr
		BhvrOverride
			Scale 0.05 0.05 0.05
		End
		ChildFx :SweepingStrike_Fast_Child.fx
	End

End

Condition
	On Time
	Time 13

	Event
		At Root
		Type Local
		EName AirRushAnchor
		BhvrOverride
			PositionOffset -2 0 1
			PyrRotate 0 -20 0
		End
		Part	:TitanSwing_Dust_Traveling.part
		Lifespan 3
	End

End

Condition
	On Time
	Time 15

	Event
		At Root
		Type Local
		EName AirRushAnchor
		BhvrOverride
			PositionOffset 0 0 3
			PyrRotate 0 0 0
		End
		Part	:TitanSwing_Dust_Traveling.part
		Lifespan 3
	End

End

Condition
		On Time
		Time 17

	Event
			At Root
			Type Local
			EName AirRushAnchor
			BhvrOverride
				PositionOffset 2 0 1
				PyrRotate 0 20 0
			End
		Part	:TitanSwing_Dust_Traveling.part
		Lifespan 3
	End

End

End
