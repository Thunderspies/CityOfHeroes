#                                                          
FxInfo

	Flags InheritAlpha InheritAnimScale DontSuppress


###########################################################

	Condition
		On Time
		Time 0

		Event
			At WepR
			Type Local
			EName GlowyBits
			Flags OneAtATime
			BhvrOverride
				PyrRotate 180 0 0
				#Scale 5 5 5
				TintGeom 1
				BeColor1 255 0 0
				ByTime1 1
				PrimaryTint 100
				SecondaryTint 0
			End
			Geom Mecha_Claw01_GlowFX
		End

		Event
			At GlowyBits
			Type Local
			EName GlowFX
			Flags OneAtATime
			BhvrOverride
				PositionOffset 0.0 -0.2 0
				PyrRotate 0 90 0
				Scale .25 1 1
			End
			Part WEAPONS\Custom_Claws\Claw_Glow_Shaft.part
			Part WEAPONS\Custom_Claws\Claw_Glow_Outer.part
		End

		Event
			At GlowyBits
			Type Local
			EName GlowFX2
			Flags OneAtATime
			BhvrOverride
				PositionOffset 0.0 -0.2 0
				PyrRotate 0 90 180
				#Scale .25 1 1
			End
			Part WEAPONS\Custom_Claws\Claw_Glows_Straight.part
		End

	End

End
