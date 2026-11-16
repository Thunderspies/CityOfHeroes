#
FxInfo

	LifeSpan 70


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Origin
			Type Start
			EName Anchor
		End

		Event
			At Anchor
			Type Start
			BhvrOverride
				PositionOffset 0 -2.5 0
				Scale 1.25 1.25 1.25
			End
			Part :Geyser_ViolentSplash_Initial.part
			#Sound Sewer_SewerGeiser_Start_01 100 100 0.8
		End

		Event
			At Anchor
			Type Start
			Bhvr behaviors\CameraShake_Strong30.bhvr
			Part :Geyser_InitialFoam_Flat_Hilite.part
		End

		Event
			At Anchor
			Type Start
			EName Steam
			BhvrOverride
				PositionOffset 0 -0.5 0
				Scale 1.25 1.25 1.25
			End
			Part :Geyser_SteamAlpha.part
			Part :Geyser_ViolentSteamCore.part
			Part :Geyser_ViolentSteamCore2.part
			Part :Geyser_ViolentSteamCoreAdd.part
			Part :Geyser_Top_Splash_Spray.part
			Part :Geyser_Top_Splash.part
			Part :Geyser_Splash_Base.part
			Part :Geyser_Splash_Base02.part
			Part :Geyser_Splash_Secondary.part
			Part :Geyser_Splash_Secondary_Add.part
			#Sound Sewer_SewerGeiser_Loop 100 100 0.8
			LifeSpan 65
		End

	End

	Condition
		On Time
		Time 3

		Event
			At Anchor
			Type Start
			ChildFx :Geyser_Hit.fx
			BhvrOverride
				PositionOffset 12 0 12
				StartJitter 8 0 8
			End
		End

	End

	Condition
		On Time
		Time 5

		Event
			At Anchor
			Type Start
			ChildFx :Geyser_Hit.fx
			BhvrOverride
				PositionOffset -12 0 -12
				StartJitter 8 0 8
			End
		End

	End

	Condition
		On Time
		Time 8

		Event
			At Anchor
			Type Start
			ChildFx :Geyser_Hit.fx
			BhvrOverride
				PositionOffset 12 0 -12
				StartJitter 8 0 8
			End
		End

		Event
			At Anchor
			Type Start
			ChildFx :Geyser_Hit.fx
			BhvrOverride
				PositionOffset -8.5 0 8
				StartJitter 8 0 8
			End
		End

	End

	Condition
		On Time
		Time 12

		Event
			At Anchor
			Type Start
			ChildFx :Geyser_Hit.fx
			BhvrOverride
				PositionOffset -12 0 12
				StartJitter 8 0 8
			End
		End

	End

	Condition
		On Time
		Time 6
		Chance 0.6

		Event
			At Anchor
			Type Start
			ChildFx :Geyser_Hit.fx
			BhvrOverride
				PositionOffset 8 0 8
				StartJitter 10 0 10
			End
		End

	End

	Condition
		On Time
		Time 10
		Chance 0.6

		Event
			At Anchor
			Type Start
			ChildFx :Geyser_Hit.fx
			BhvrOverride
				PositionOffset -8 0 -8
				StartJitter 10 0 10
			End
		End

	End

End
