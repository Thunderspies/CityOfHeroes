#
FxInfo

	Flags DontSuppress


###########################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	DC_Activation_Loop 30 30 .05
		bhvr	behaviors/Sound/SoundIn1.bhvr
		lifespan 10000
	End
End


	Condition
		On Cycle
		Time 15
		Random 1
		Chance 0.8

		Event
			At WepL
			Type Local
			EName HandSwirlL
			Bhvr CustomizeablePowers\DarknessControl\Shade_Scale_Small.bhvr
			BhvrOverride
				PositionOffset 0 0 0
				PyrRotate 0 0 0
				PyrRotateJitter 180 180 180
				EndScale 0.15 0.2 0.15
				StartJitter 0 0 0
				TintGeom	1
			End
			Geom Tintable_DarkGrasp
			LifeSpan 15
		End

		Event
			At WepR
			Type Local
			EName HandSwirlR
			Bhvr CustomizeablePowers\DarknessControl\Shade_Scale_Small.bhvr
			BhvrOverride
				PyrRotate 0 0 0
				PyrRotateJitter 180 180 180
				PositionOffset 0.0 0.0 0.0
				StartJitter 0.0 0.0 0.0
				EndScale 0.15 0.2 0.15
				TintGeom	1
			End
			Geom Tintable_DarkGrasp
			LifeSpan 15
		End

	End

	Condition
		On Time
		Time 15

		Event
			At WepL
			Type Local
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :Hand_Core_Solid.part
			Part :Hand_Glow.part
			Part :Hand_Glow_Faint.part
			Part CustomizeablePowers\DarknessControl\Hand_Highlights.part
			LifeSpan 0
			LifeSpanJitter 0.000000
		End

		Event
			At WepR
			Type Local
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :Hand_Core_Solid.part
			Part :Hand_Glow.part
			Part :Hand_Glow_Faint.part
			Part CustomizeablePowers\DarknessControl\Hand_Highlights.part
			LifeSpan 0
			LifeSpanJitter 0.000000
		End

	End

	Condition
		On Cycle
		Time 3
		Random 1
		Chance 0.5

		Event
			At WepR
			Type Local
			EName DripsR
			Part :Hand_Drips.part
			LifeSpan 10
			LifeSpanJitter 5
		End

		Event
			At WepL
			Type Local
			EName DripsL
			Part :Hand_Drips.part
			LifeSpan 10
			LifeSpanJitter 5
		End

		Event
			At WepR
			Type Local
			EName SlimeR
			Part :Hand_Slime.part
			LifeSpan 10
			LifeSpanJitter 5
		End

		Event
			At WepL
			Type Local
			EName SlimeL
			Part :Hand_Slime.part
			LifeSpan 10
			LifeSpanJitter 5
		End

	End

End
