#
FxInfo

	LifeSpan 48


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Origin
			Type StartPositOnly
			POther DustAnchor
			BhvrOverride
				PositionOffset 0 -3.5 0
			End
			Part :Water_Splatter.part
			Part :Flash_Glow_Large.part
			Part :Mist_Spread.part
			Part :Sparks_Impact.part
			Part :WaterBurst_WaterSplode.part
			Part :WaterBurst_WaterSplode02.part
			Part :WaterBurst_WaterSplode03.part
			Part :WaterBurst_WaterSplode03b.part
			Part :WaterBurst_WaterSplode04.part
			Part :WaterBurst_WaterSplode05.part
			Part :WaterBurst_WaterSplode06.part
			LifeSpan 5
		End

	End

	Condition
		On Time
		Time 0

		Event
			At Origin
			Type StartPositOnly
			Bhvr behaviors/Braz/ExpandRingFast.bhvr
			BhvrOverride
				PositionOffset 0 -3.5 0
			End
			Part :Mist_Shockwave3.part
			Part :Mist_Pillar_Large.part
			LifeSpan 2
		End

	End

	Condition
		On Time
		Time 0

		Event
			At Origin
			Type StartPositOnly
			BhvrOverride
				PositionOffset 0 -3.5 0
			End
			Part :Mist_Lingering_Cloud.part
			LifeSpan 40
		End

	End

	Condition
		On Time
		Time 3

		Event
			At Origin
			Type StartPositOnly
			POther DustAnchor
			BhvrOverride
				PositionOffset 0 -3.5 0
			End
			Part :Water_Splatter.part
			LifeSpan 5
		End

		Event
			At Origin
			Type StartPositOnly
			BhvrOverride
				PositionOffset 0 10 0
			End
			Part :Mist_Falling.part
		End

	End

	Condition
		On Time
		Time 21

		Event
			At ExplosionAnchor
			Type StartPositOnly
			POther DustAnchor
			BhvrOverride
				PositionOffset 0 -3.5 0
			End
			Part :Water_Splatter.part
			LifeSpan 5
		End

	End

End
