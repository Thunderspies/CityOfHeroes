FxInfo

	Flags InheritAlpha InheritAnimScale DontSuppress


###########################################################

Condition
		Event
			EName Slime
			Type Local
			At WepL
			Flags OneAtATime
			BhvrOverride
				PositionOffset 0 0 2.5
			End
			Part WEAPONS\Custom_TitanWeapon\TitanWeapon_Demonic_Axe_SlimeB.part
			Part WEAPONS\Custom_TitanWeapon\TitanWeapon_Demonic_Axe_SlimeC.part
		End

		Event
			EName SlimeB
			Type Local
			At WepL
			Flags OneAtATime
			BhvrOverride
				PositionOffset 0 0 1
			End
			Part WEAPONS\Custom_TitanWeapon\TitanWeapon_Demonic_Axe_Slime.part
			Part WEAPONS\Custom_TitanWeapon\TitanWeapon_Demonic_Axe_SlimeB.part
		End

		Event
			EName SlimeC
			Type Local
			At WepL
			Flags OneAtATime
			BhvrOverride
				PositionOffset 0 0 1.5
			End
			Part WEAPONS\Custom_TitanWeapon\TitanWeapon_Demonic_Axe_Slime.part
			Part WEAPONS\Custom_TitanWeapon\TitanWeapon_Demonic_Axe_SlimeB.part
		End

	End

End