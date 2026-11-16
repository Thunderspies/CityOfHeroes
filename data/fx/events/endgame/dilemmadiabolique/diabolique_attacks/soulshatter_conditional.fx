###########################################################

FxInfo

###########################################################

	Flags DontSuppress

###########################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	DD_SoulshatterConditional_Loop 50 50 .2
		bhvr	behaviors/Sound/SoundIn2Out10.bhvr
		lifespan 500
	End
End


	Condition
		On Cycle
		Time 25

		Event
			At Hips
			Type Local
			EName Swirls
			Bhvr CustomizeablePowers\DarknessControl\DarkGrasp_Scale.bhvr
			BhvrOverride
				Spin 0 0.2 0
				TintGeom 1
				StartColor 1 1 1
			End
			Geom Tintable_DarkGrasp
			LifeSpan 35
		End

		Event
			At Root
			Type Local
			BhvrOverride
				Spin 0 0.1000 0
			End
			LifeSpan 50
		End

		Event
			At Hips
			Type Local
			EName SwirlsSmall
			Bhvr CustomizeablePowers\DarknessControl\DarkGrasp_Scale_Small.bhvr
			BhvrOverride
				TintGeom 1
			End
			Geom Tintable_DarkGrasp
			LifeSpan 35
		End

	End

###########################################################

End