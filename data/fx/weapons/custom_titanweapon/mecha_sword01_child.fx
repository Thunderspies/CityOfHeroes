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
			Geom Mecha_Sword01_GlowFX
		End

		Event
			At Origin
			Type Local
			EName GlowFX
			Flags OneAtATime
			BhvrOverride
				PositionOffset 0.25 0 1
			End
			Part WEAPONS\Custom_TitanWeapon\EH_Glow_Outer.part
			Part WEAPONS\Custom_TitanWeapon\EH_Glow_Shaft3.part
		End

		Event
			At Origin
			Type Local
			EName GlowFX2
			Flags OneAtATime
			BhvrOverride
				PositionOffset -0.3 0 0.65
				PyrRotate 0 0 -90
			End
			Part WEAPONS\Custom_TitanWeapon\EH_Glow_Outer.part
			Part WEAPONS\Custom_TitanWeapon\EH_Glow_Shaft2.part
		End

		Event
			At Origin
			Type Local
			EName GlowFX3
			Flags OneAtATime
			BhvrOverride
				PositionOffset -0.1 0 1.5
				PyrRotate 0 0 90
			End
			Part WEAPONS\Custom_TitanWeapon\EH_Flare_Big_Straight.part
		End

	End

End
