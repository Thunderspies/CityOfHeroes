#                                                          
FxInfo

	Flags DontSuppress


###########################################################

	Condition
		On Time
		Time 5

		Event
			At Root
			Type Local
			Bhvr behaviors/Sound/SoundIn1.bhvr
			Sound DC_ShadowField_Hit_Loop 100 100 0.5
			LifeSpan 10000
		End

	End

	Condition
		On Time
		Time 0

		Event
			At Root
			Type Local
			BhvrOverride
				Spin 0 0.05 0
				#FadeInLength 1
			End
			#Part CustomizeablePowers\DarknessControl\Explosion_Shockwave_Small.part
		End

	End

	Condition
		On Time
		Time 5

		Event
			At Hips
			Type Local
			EName CrushingHateGeo
			Bhvr CustomizeablePowers\DarknessControl\ShadowField_Expand_Large.bhvr
			BhvrOverride
				PyrRotate 0 0 10
				Scale 0.01 0.01 0.01
				EndScale 1 1 1
				Spin 0 0.2 0
				#TintGeom 1
				Alpha 80
				FadeInLength 40
				FadeOutLength 50
				StartColor 1 1 1
				PositionOffset 0 -0.6 0
			End
			Geom Tintable_ShadowField
			#LifeSpan 1
		End

		Event
			At Hips
			Type Local
			EName CrushingHateSwirls
			Bhvr :ShadowField_Shrink.bhvr
			BhvrOverride
				PositionOffset 0 -0.6 0
				PyrRotate 0 0 20
				EndScale 1.030 1.030 1.030
				Spin 0 0.2 0
				#TintGeom 1
				Alpha 120
				#FadeInLength 40
				FadeOutLength 70
				StartColor 1 1 1
			End
			Geom Tintable_ShadowField_Highlights
			#LifeSpan 1
		End

	End

	Condition
		On Time
		Time 20

		Event
			At Hips
			Type Local
			EName CrushingHateSwirls
			BhvrOverride
				PositionOffset 0 -0.6 0
			End
			Part ENEMYFX\BlackKnights\ShadowField_Border.part
			#Part CustomizeablePowers\DarknessControl\ShadowField_Swarm.part
		End

	End

	Condition
		On Death

		Event
			Type Destroy
			EName CrushingHateGeo
		End

		Event
			Type Destroy
			EName CrushingHateSwirls
		End

	End

End
