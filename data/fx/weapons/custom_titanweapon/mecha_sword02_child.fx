#
FxInfo

	Flags InheritAlpha InheritAnimScale DontSuppress


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Origin
			Type Local
			EName GlowyBits
			Flags OneAtATime
			BhvrOverride
				TintGeom 1
				BeColor1 255 0 0
				ByTime1 1
				PrimaryTint 100
				SecondaryTint 0
			End
			Geom Mecha_Sword02_GlowFX
		End

		Event
			At Origin
			Type Local
			EName GlowFX
			Flags OneAtATime
			BhvrOverride
				PositionOffset 0 0 0.4
			End
			Part WEAPONS\Custom_TitanWeapon\EH_Glow_Outer.part
			Part WEAPONS\Custom_TitanWeapon\EH_Glow_Shaft.part
		End

		Event
			At Origin
			Type Local
			EName GlowFX2
			Flags OneAtATime
			BhvrOverride
				PositionOffset 0 0 1
				PyrRotate 0 10 90
			End
			Part WEAPONS\Custom_TitanWeapon\EH_Glows_SM.part
		End

		Event
			At Origin
			Type Local
			EName GlowFX3
			Flags OneAtATime
			BhvrOverride
				PositionOffset 0 0 0.6
				PyrRotate 0 -10 -90
			End
			Part WEAPONS\Custom_TitanWeapon\EH_Glows.part
		End

	End

End
