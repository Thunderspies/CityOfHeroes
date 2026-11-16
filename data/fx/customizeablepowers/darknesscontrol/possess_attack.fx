#
FxInfo

	LifeSpan 300


###########################################################

	Condition
		On Time
		Time 20

		Event
			At Root
			Type Local
			Sound DC_Possess_Attack_01 100 100 0.8
		End

		Event
			At WepR
			Type Start
			EName Prime
			LookAt T_Head
			Magnet T_Head
			Bhvr Behaviors\projectile3.bhvr
			BhvrOverride
				TrackRate 1.5
			End
			LifeSpan 80
		End

		Event
			At Prime
			Type Local
			EName Spinner1
			BhvrOverride
				Spin 0.25 0.1 0.0
			End
			LifeSpan 80
		End

		Event
			At Spinner1
			Type Local
			BhvrOverride
				PositionOffset 0.0 0.0 2.0
			End
			Part :Soul_Trail_Core.part
			Part :Soul_Trail_Large.part
			Part :Soul_Head.part
			Part :Soul_Head_Light.part
			Part :Soul_Trail_Large_Solid.part
			LifeSpan 80
		End

	End

	Condition
		On Time
		Time 24

		Event
			At WepL
			Type Start
			EName Prime3
			LookAt T_Head
			Magnet T_Head
			Bhvr Behaviors\projectile3.bhvr
			BhvrOverride
				TrackRate 1.5
			End
			LifeSpan 80
		End

		Event
			At Prime3
			Type Local
			EName Spinner3
			BhvrOverride
				Spin 0.0 -0.25 -0.1
			End
			Sound 	DC_Possess_Hit_Loop 100 100 .5
			LifeSpan 80

		End

		Event
			At Spinner3
			Type Local
			BhvrOverride
				PositionOffset 2.0 0.0 0.0
			End
			Part :Soul_Trail_Core.part
			Part :Soul_Trail_Large.part
			Part :Soul_Head.part
			Part :Soul_Head_Light.part
			Part :Soul_Trail_Large_Solid.part
			LifeSpan 80
		End

	End

	Condition
		On Time
		Time 26

		Event
			At WepR
			Type Start
			EName Prime2
			LookAt T_Head
			Magnet T_Head
			Bhvr Behaviors\projectile3.bhvr
			BhvrOverride
				TrackRate 1.5
			End
			LifeSpan 80
		End

		Event
			At Prime2
			Type Local
			EName Spinner2
			BhvrOverride
				Spin -0.25 -0.1 0.0
			End
			LifeSpan 80
		End

		Event
			At Spinner2
			Type Local
			BhvrOverride
				PositionOffset 0.0 0.0 2.0
			End
			Part :Soul_Trail_Core.part
			Part :Soul_Trail_Large.part
			Part :Soul_Head.part
			Part :Soul_Head_Light.part
			Part :Soul_Trail_Large_Solid.part
			LifeSpan 80
		End

	End

End
