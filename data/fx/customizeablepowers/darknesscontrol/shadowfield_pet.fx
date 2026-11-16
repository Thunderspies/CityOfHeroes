#
FxInfo

	Flags DontSuppress


###########################################################

Condition
	On 	Time
	Time	5
	Event
		Type	Local
		At	Root
		Sound 	DC_ShadowField_Hit_Loop 100 100 .5
		bhvr	behaviors/Sound/SoundIn1.bhvr
		lifespan 10000
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
			Part :Explosion_Shockwave_Small.part
		End

	End

	Condition
		On Time
		Time 5

		Event
			At Root
			Type PositOnly
			EName CrushingHateGeo
			Bhvr :ShadowField_Expand_Large.bhvr
			BhvrOverride
				PyrRotate 0 0 10
				Spin 0 0.15 0
				TintGeom 1
				Alpha 80
				#FadeInLength 40
				FadeOutLength 50
				#StartColor 1 1 1
			End
			Geom Tintable_ShadowField
			#LifeSpan 1
		End

		Event
			At Root
			Type PositOnly
			EName CrushingHateSwirls
			Bhvr :ShadowField_Expand.bhvr
			BhvrOverride
				PyrRotate 0 0 20
				Spin 0 0.15 0
				TintGeom 1
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
			At Root
			Type PositOnly
			EName CrushingHateSwirls
			Part :ShadowField_Swarm.part
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
