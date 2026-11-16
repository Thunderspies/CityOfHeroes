#
FxInfo

	Flags InheritAlpha InheritAnimScale DontSuppress


###########################################################

Condition
		Event
			EName 2
			Type Local
			At Origin
			Flags OneAtATime
			BhvrOverride
				PositionOffset 0 0 1
			End
			Part WEAPONS\Custom_TitanWeapon\TitanWeapon_Purgatory_Sword_Fire.part
		End

		Event
			EName 222
			Type Local
			At Origin
			Flags OneAtATime
			BhvrOverride
				PositionOffset 0 0.21 0.5
			End
			Part WEAPONS\Custom_TitanWeapon\TitanWeapon_Purgatory_Sword_Glow.part
			Part WEAPONS\Custom_TitanWeapon\TitanWeapon_Purgatory_Sword_GlowA.part
		End

		Event
			EName 444
			Type Local
			At Origin
			Flags OneAtATime
			BhvrOverride
				PositionOffset 0 -0.205 0.5
				PyrRotate 0 90 0
			End
			Part WEAPONS\Custom_TitanWeapon\TitanWeapon_Purgatory_Sword_Glow.part
		End

	End

End
