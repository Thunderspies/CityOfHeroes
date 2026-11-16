#
FxInfo

	Flags DontSuppress
	Lifespan 300

###########################################################

Condition
	On 	Time
	Time	10
	Event
		Type	Local
		At	Root
		Sound 	DD_Singularity_Attack_01 300 100 .8
	End
End


	Condition
		On Time
		Time 0

		Event
			At T_Root
			Type PositOnly
			Part Events\EndGame\DilemmaDiabolique\Diabolique_Attacks\CrushingHate_Fill_Flat.part
			LifeSpan 230
		End

		Event
			EName SingularityAnchor
			At T_Head
			Type PositOnly
			Part :CrushingHate_Swirls.part
			LifeSpan 215
		End
	End

	Condition
		On Time
		Time 40

		Event
			At SingularityAnchor
			Type PositOnly
			Bhvr :ForceBubble_Disintegration.bhvr
			BhvrOverride
				PyrRotate 30 20 0
				Spin 0 -0.5 0
				Alpha 150
				FadeInLength 50
				FadeOutLength 20
				StartColor 10 1 20
			End
			Part :CrushingHate_Border.part
			Geom Tintable_ShadowField
			LifeSpan 175
		End
	End

	Condition
		On Time
		Time 60

		Event
			At SingularityAnchor
			Type PositOnly
			EName SingularityAnchor_Stg2
			Part :CrushingHate_Swirls_Additive.part
			Part :CrushingHate_Fill_Alpha.part
			LifeSpan 155
		End

		Event
			EName SingularityAnchor_Stg2
			At Head
			Type PositOnly
			LifeSpan 50
		End
	End

	Condition
		On Cycle
		Time 50

		Event
			At SingularityAnchor_Stg2
			Type Local
			EName CrushingHateGeo
			Bhvr :ForceBubble_Disintegration_Small.bhvr
			BhvrOverride
				PyrRotate -60 30 0
				PyrRotateJitter 10 10 0
				Alpha 255
				FadeInLength 10
				FadeOutLength 30
				StartColor 50 0 90
			End
			Geom FX_Singularity_Highlights
			LifeSpan 40
		End
	End

	Condition
		On Time
		Time 80

		Event
			At T_Head
			Type PositOnly
			ChildFx :CHILD_Singularity_20ft.fx
			EName Ring20
			LifeSpan 90
		End

		Event
			At T_Head
			Type PositOnly
			EName Lightrays
			Part Events\EndGame\DilemmaDiabolique\Diabolique_Attacks\CrushingHate_Lightrays_Expanding.part
			LifeSpan 50
		End
	End

	Condition
		On Time
		Time 110
		Event
			At T_Head
			Type StartPositOnly
			EName Explosion
			Part Events\EndGame\DilemmaDiabolique\Diabolique_Attacks\Buildup_Rays.part
			LifeSpan 65
		End
	End

	Condition
		On Time
		Time 140

		Event
			At T_Head
			Type PositOnly
			ChildFx :CHILD_Singularity_40ft.fx
			EName Ring40
			LifeSpan 120
		End
	End

	Condition
		On Time
		Time 170

		Event
			At T_Head
			Type PositOnly
			Bhvr	behaviors/CameraShakeFault.bhvr
			LifeSpan 120
		End
	End

	Condition
		On Time
		Time 200

		Event
			At T_Head
			Type PositOnly
			ChildFx :CHILD_Singularity_60ft.fx
			EName Ring60
			LifeSpan 60
		End
	End

	Condition
		On Time
		Time 220

		Event
			At T_Head
			Type PositOnly
			EName Ring60
			Part :CrushingHate_Fill_Alpha_Shrink.part
			Part :CrushingHate_Swirls_Shrink.part
			Part :CrushingHate_Core_Additive_Shrink.part
			Part :CrushingHate_Shockwave_Shrink.part
			LifeSpan 5
		End

		Event
			At T_Head
			Type PositOnly
			Bhvr :ForceBubble_Shrink.bhvr
			BhvrOverride
				PyrRotate 30 20 0
				Spin 0 -0.5 0
				Alpha 150
				FadeInLength 5
				FadeOutLength 20
				StartColor 10 1 20
			End
			Geom Tintable_ShadowField
			LifeSpan 20
		End
	End

	Condition
		On Time
		Time 240

		Event
			At T_Head
			Type StartPositOnly
			EName Explosion
			Bhvr behaviors\CameraShake_Strong30.bhvr
			Part Events\EndGame\DilemmaDiabolique\Diabolique_Attacks\CrushingHate_Streak.part
			Part Events\EndGame\DilemmaDiabolique\Diabolique_Attacks\Explosion_Flash_Streaks.part
			Part Events\EndGame\DilemmaDiabolique\Diabolique_Attacks\CrushingHate_Star.part
			Part Events\EndGame\DilemmaDiabolique\Diabolique_Attacks\CrushingHate_Star_Subtractive.part
			Part Events\EndGame\DilemmaDiabolique\Diabolique_Attacks\Explosion_Shockwave.part
			Part Events\EndGame\DilemmaDiabolique\Diabolique_Attacks\Explosion_Flash_Core.part
			#Part Events\EndGame\DilemmaDiabolique\Diabolique_Attacks\CrushingHate_Lightrays_Expanding.part
			LifeSpan 10
		End
	End
End
