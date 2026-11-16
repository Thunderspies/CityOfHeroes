#
FxInfo

	Input
		InpName Hips
	End

	LifeSpan 150


###########################################################
Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	NA_CorrosiveSap_01 100 100 .8
	End
End

	Condition
		On Time
		Time 0

		Event
			At WepR
			Type Local
			EName Anchor
			Flags OneAtATime
		End

	End

	Condition
		On Time
		Time 5

		Event
			At LArmR
			Type Local
			Bhvr CustomizeablePowers\PlantControl\NatureAffinity\QuickGrow.bhvr
			BhvrOverride
				TintGeom 1
				PositionOffset 0.7 0 0.1
				PyrRotate 0 15 -5
				Scale 0.1 0.1 0.1
				FadeInLength 20
				FadeOutLength 15
			End
			Geom FX_Tintable_Kelp_Wreath_LegL
			LifeSpan 40
		End

		Event
			At LArmR
			Type Local
			Bhvr CustomizeablePowers\PlantControl\NatureAffinity\QuickGrow.bhvr
			BhvrOverride
				TintGeom 1
				FadeInLength 20
				FadeOutLength 15
				PyrRotate 180 -0 -15
				PositionOffset 0.9 0 0.0
				Scale .1 .1 .1
			End
			Geom FX_Tintable_Kelp_Wreath_LegL
			LifeSpan 40
		End

	End

	Condition
		On Time
		Time 10

		Event
			At LArmR
			Type Local
			Bhvr CustomizeablePowers\PlantControl\NatureAffinity\QuickGrow.bhvr
			BhvrOverride
				TintGeom 1
				FadeInLength 20
				FadeOutLength 0
				#PyrRotate 180 -0 -15
				PositionOffset 1.5 0 0.0
				Scale .1 .1 .1
			End
			Geom FX_Tintable_NatureBlast_Flower_Head
			LifeSpan 10
		End

	End

	Condition
		On Time
		Time 20

		Event
			At LArmR
			Type Local
			Bhvr CustomizeablePowers\PlantControl\NatureAffinity\QuickGrow.bhvr
			BhvrOverride
				TintGeom 1
				FadeInLength 0
				FadeOutLength 45
				#PyrRotate 180 -0 -15
				PositionOffset 1.5 0 0.0
				Scale 2 .75 .75
			End
			Geom FX_Tintable_NatureBlast_Flower_Head
			LifeSpan 5
		End

	End

	Condition
		On Time
		Time 25

		Event
			At LArmR
			Type Local
			Bhvr CustomizeablePowers\PlantControl\NatureAffinity\QuickGrow.bhvr
			BhvrOverride
				TintGeom 1
				FadeInLength 0
				#FadeOutLength 0
				#PyrRotate 180 -0 -15
				PositionOffset 1.5 0 0.0
				#Scale 2 .75 .75
			End
			Geom FX_Tintable_NatureBlast_Flower_Head
			LifeSpan 15
		End

	End

	Condition
		On Time
		Time 20

		Event
			At Anchor
			Type Start
			EName Prime
			LookAt T_Chest
			Magnet T_Chest
			Bhvr CustomizeablePowers\SonicAttack\SMLProjectile.bhvr
			BhvrOverride
				InitialVelocity 0 0.1 0
				TrackRate 2
			End
			Part CustomizeablePowers\PlantControl\NatureAffinity\Tintable_SapShot_Bullet_SM.part
			Part CustomizeablePowers\PlantControl\NatureAffinity\Tintable_SapShot_Trail_01.part
			Part CustomizeablePowers\PlantControl\NatureAffinity\Tintable_SapShot_Trail_02.part
		End

	End

	Condition
		On PrimeHit

		Event
			Type Destroy
			EName Prime
		End

	End

End
