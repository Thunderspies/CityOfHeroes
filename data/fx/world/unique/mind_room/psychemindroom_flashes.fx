#
FxInfo


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Root
			Type Local
			EName FlashAnchor
			BhvrOverride
				PositionOffset 0 40 0
			End
		End

		Event
			At Root
			Type Start
			BhvrOverride
				Spin 0 0.0005000 0
			End
			Part :PsycheFlash_Clouds.part
			Part :PsycheFlash_Cloud_Ring.part
			Part :PsycheFlash_Cloud_Lingering.part
			Part :PsycheFlashes_Cloud_Highlights.part
		End

	End

	Condition
		On Cycle
		Time 6

		Event
			At FlashAnchor
			Type Local
			ChildFx :Child_Lightning.fx
			EName Lightning
			LifeSpan 1
		End

	End

End
