#############################################################
## BuildUp_Activation.fx
#############################################################

FxInfo
Lifespan 150

#############################################################

	Condition
		On Time
		Time 0

		Event
			Ename TipFlashAnchor
			At C_Chest
			Type Local
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
			part 	:BuildUp_WeaponFlashes.part
			#part 	:BuildUp_WeaponSmoke_In.part
			Lifespan 5
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
