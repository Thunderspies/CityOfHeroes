#
FxInfo

	LifeSpan 100


###########################################################

	Condition
		On Time
		Time 0

		Event
			EName 	Prime
			Type	Local
			At	Origin
			BhvrOverride
				Alpha 1
			End
			Flags	AdoptParentEntity
			Lifespan 5
		End

		Event
			At Root
			Type Local
			EName Anchor
			BhvrOverride
				PositionOffset 0 0 0
			End
		End

		Event
			At Root
			Type Local
			EName Anchor4
			BhvrOverride
				PositionOffset 0 0 0
			End
		End

		Event
			At Anchor
			Type Local
			EName Anchor2
			BhvrOverride
				Spin 0 -0.35 0
			End
		End

		Event
			At Anchor
			Type Local
			EName Anchor3
			LifeSpan 40
		End

		Event
			At Anchor
			Type Local
			BhvrOverride
				PositionOffset 0 0 0
			End
			Part NPCFX\Pets\Leprechaun\SummonRing_Poof01.part
			Part NPCFX\Pets\Leprechaun\SummonRing_Dust.part
			Part NPCFX\Pets\Leprechaun\SummonRing_Dust2.part
			Part NPCFX\Pets\Liger\SummonRing_PoofsIn.part
			Part NPCFX\Pets\Leprechaun\SummonRing_LightStreaks_Small.part
			LifeSpan 40
		End

	End

	Condition
		On Cycle
		Time 5
		Repeat 5

		Event
			At Anchor3
			Type Local
			EName CrushingHateGeo
			Bhvr CustomizeablePowers\DarknessControl\ShadowyBinds_Scale.bhvr
			BhvrOverride
				InitialVelocity 0 0.1 0
				Scale 1 1 1
				TintGeom 0
				Alpha 180
				StartColor 255 252 255
			End
			Geom Tintable_ShadowyBinds
			LifeSpan 10
		End

	End

	Condition
		On Time
		Time 15

		Event
			EName 	Prime
			Type	Local
			At	Origin
			Bhvr	behaviors/fadein.bhvr
			Flags	AdoptParentEntity
		End

	End

	Condition
		On Time
		Time 36

		Event
			At anchor
			Type Local
			EName Flash
			Part :Flash_Small.part
			LifeSpan 5
		End

	End

	Condition
		On Time
		Time 40

		Event
			At anchor
			Type Start
			EName Flash
			Part :Flash.part
			Part :Flash_Add.part
			Part NPCFX\Pets\Leprechaun\Spawn_Flash.part
			Part :SummonRing_GoldDust.part
			Part :SummonRing_GoldDust_Add.part
			Part :Glow_Streaks.part
			LifeSpan 5
		End

	End

	Condition
		On Time
		Time 40
		Repeat 5

		Event
			At Anchor4
			Type Start
			EName Flash
			BhvrOverride
				Drag 0.01
				Gravity 0.0100
				InitialVelocity 0 0.25 0
				InitialVelocityJitter 0.2 0.10 0.2
				PyrRotateJitter 180 0 180
				SpinJitter 0.10 0 0.10
			End
			Part WEAPONS\Bow\Archery\Heart_Twinkle.part
			Part WEAPONS\Bow\Archery\Heart_TwinkleClump.part
		End

	End

End
