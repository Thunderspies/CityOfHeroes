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
			Geom MechaBroadSword02_BladeFX
		End

		Event
			At Origin
			Type Local
			EName GlowyBits2
			Flags OneAtATime
			BhvrOverride
				TintGeom 1
				BeColor1 255 0 0
				ByTime1 1
				PrimaryTint 100
				SecondaryTint 0
			End
			Geom MechaKatana02_GlowFX
		End

		Event
			At Origin
			Type Local
			EName GlowFX
			Flags OneAtATime
			BhvrOverride
				PositionOffset 0 0 0.4
				Scale .52 .52 .52
			End
			Part WEAPONS\Custom_Broadsword\SMEH_Glow_Outer.part
			Part WEAPONS\Custom_Broadsword\SMEH_Glow_Shaft.part
		End

		Event
			At Origin
			Type Local
			EName GlowFX2
			Flags OneAtATime
			BhvrOverride
				PositionOffset -.08 0 0.7
				PyrRotate 0 15 -90
				Scale .6 .6 .6
			End
			Part WEAPONS\Custom_Broadsword\SMEH_Glows_SM.part
		End

		Event
			At Origin
			Type Local
			EName GlowFX3
			Flags OneAtATime
			BhvrOverride
				PositionOffset .08 0 0.5
				PyrRotate 0 -15 90
				Scale .6 .6 .6
			End
			Part WEAPONS\Custom_Broadsword\SMEH_Glows.part
		End

	End

End
