#
FxInfo

	Flags InheritAlpha InheritAnimScale DontSuppress


###########################################################

Condition
		Event
			At Origin
			EName Right_Front_Eye
			Flags OneAtATime
			Type Local
			BhvrOverride
				PositionOffset 0.12 0.200000 2.7
			End
			Part WEAPONS\Custom_TitanWeapon\TitanWeapon_Minotaur_Axe_Glow.part
		End

		Event
			At Origin
			EName Left_Front_Eye
			Flags OneAtATime
			Type Local
			BhvrOverride
				PositionOffset -0.10 0.1800000 2.7
			End
			Part WEAPONS\Custom_TitanWeapon\TitanWeapon_Minotaur_Axe_Glow.part

		End

		Event
			At Origin
			EName Left_Back_Eye
			Flags OneAtATime
			Type Local
			BhvrOverride
				PositionOffset -0.10 -0.16 2.7
			End
			Part WEAPONS\Custom_TitanWeapon\TitanWeapon_Minotaur_Axe_Glow.part

		End

		Event
			At Origin
			EName Right_Back_Eye
			Flags OneAtATime
			Type Local
			BhvrOverride
				PositionOffset 0.12 -0.16 2.7
			End
			Part WEAPONS\Custom_TitanWeapon\TitanWeapon_Minotaur_Axe_Glow.part

		End

	End

End
