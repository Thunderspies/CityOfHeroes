#
FxInfo

	LifeSpan 150


###########################################################
Condition
	On 	Time
	Time	1
	Event
		Type	Local
		At	Root
		Sound 	WB_Geyser 100 100 .8
	End
End

	Condition
		On Time
		Time 0

		Event
			EName ProjectileAnchor
			At T_Root
			Type StartPositOnly
		End

		Event
			At WepR
			Type Local
			EName mainAnchor
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :WaterBurst_DrawIn_Spray.part
			Part :WaterShot_GlowStreaks.part
			LifeSpan 60
		End

	End

	Condition
		On Time
		Time 3

		Event
			At mainAnchor
			Type Local
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :WaterBurst_DrawIn_Splatter.part
			LifeSpan 58
		End

	End

	Condition
		On Time
		Time 5

		Event
			At mainAnchor
			Type Local
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :WaterShot_HandGlow.part
			LifeSpan 55
		End

	End

	Condition
		On Time
		Time 55

		Event
			At ProjectileAnchor
			Type Start
			Part :Geyser_InitialSplash_01.part
			Part :Geyser_InitialSplash_02.part
			Part :Geyser_Foam_Flat_Hilite.part
			Part :Geyser_Foam_Flat.part
			LifeSpan 0
		End

		Event
			At ProjectileAnchor
			Type Local
			ChildFx :Geyser_Child.fx
		End

	End

End
