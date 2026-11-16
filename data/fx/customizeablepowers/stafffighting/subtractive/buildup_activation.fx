#                                                          
FxInfo

	LifeSpan 150


###########################################################

	Condition
		On Time
		Time 0

		Event
			At C_Chest
			Type Local
			EName TipFlashAnchor
			BhvrOverride
				PositionOffset 0 0 6
			End
		End

		Event
			At TipFlashAnchor
			Type Local
			Bhvr Behaviors/GenericPArticleFade.bhvr
			BhvrOverride
				PositionOffset 0 0 3
			End
			Part :BuildUp_WeaponFlashes.part
			#Part :BuildUp_WeaponSmoke_In.part
			LifeSpan 5
		End

		Event
			At Root
			Type Start
			ChildFx POWERS\Child_BuildUp.fx
		End

	End

	Condition
		On Time
		Time 12

		Event
			At Root
			Type Start
			Bhvr CustomizeablePowers\Brawling\Dust_Expanding.bhvr
			BhvrOverride
				PositionOffset 0 -1 0
			End
			Part :Hit_Smoke_Radial_Big.part
			Part :Hit_Smoke_Radial_Lingering_Big.part
			LifeSpan 90
		End

		Event
			At Root
			Type Start
			BhvrOverride
				PositionOffset 0.0 0.25 0.0
			End
			Part :Buildup_Shockwave.part
		End

	End

End
