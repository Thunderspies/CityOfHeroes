FXinfo
Lifespan 180

	Condition
		On Time
		time 0
#
		Event
			Type Local
			At Hips
			Bhvr	behaviors\Splat_RuneCircle.bhvr
			BhvrOverride
				FadeInLength	20
				PositionOffset	0 0.3 0
				Alpha		255
				StartColor	80 180 75
				PrimaryTint	80
				SecondaryTint	20
				Scale		1 1 1
				#Spin		0 0.02 0
				#Tintgeom 1
				PyrRotate	0 0 45
			End
			Geom FX_Ivy_Ring03
			#Lifespan 60
		End

		Event
			Type Local
			At Hips
			Bhvr	behaviors\Splat_RuneCircle.bhvr
			BhvrOverride
				FadeInLength	20
				PositionOffset	0 0 0
				Alpha		255
				StartColor	80 180 75
				PrimaryTint	80
				SecondaryTint	20
				Scale		1 1 1
				#Spin		0 0.02 0
				#Tintgeom 1
				PyrRotate	0 0 -45
			End
			Geom FX_Ivy_Ring03
			#Lifespan 60
		End

		Event
			Type Local
			At Hips
			Bhvr	behaviors\Splat_RuneCircle.bhvr
			BhvrOverride
				FadeInLength	20
				PositionOffset	-0.3 0.3 0
				Alpha		255
				StartColor	80 180 75
				PrimaryTint	80
				SecondaryTint	20
				Scale		1 1 1.3
				#Spin		0 0.02 0
				#Tintgeom 1
				PyrRotate	0 0 -90
			End
			Geom FX_Ivy_Ring03
			#Lifespan 60
		End

		Event
			Type Local
			At Hips
			Bhvr	behaviors\Splat_RuneCircle.bhvr
			BhvrOverride
				FadeInLength	20
				PositionOffset	-0.3 0.3 0
				Alpha		255
				StartColor	80 180 75
				PrimaryTint	80
				SecondaryTint	20
				Scale		.8 .8 1
				#Spin		0 0.02 0
				#Tintgeom 1
				PyrRotate	0 0 -90
			End
			Geom FX_Ivy_Ring03
			#Lifespan 60
		End

		Event
			Type Local
			At ULegL
			Bhvr	behaviors\Splat_RuneCircle.bhvr
			BhvrOverride
				FadeInLength	20
				PositionOffset	-0 -1 0
				Alpha		255
				StartColor	80 180 75
				PrimaryTint	80
				SecondaryTint	20
				Scale		.8 .8 .8
				#Spin		0 0.02 0
				#Tintgeom 1
				PyrRotate	0 0 -90
			End
			Geom FX_Ivy_Ring03
			#Lifespan 60
		End

		Event
			Type Local
			At LLegL
			Bhvr	behaviors\Splat_RuneCircle.bhvr
			BhvrOverride
				FadeInLength	20
				PositionOffset	0.1 0 -0.07
				Alpha		255
				StartColor	80 180 75
				PrimaryTint	80
				SecondaryTint	20
				Scale		.6 1 .6
				#Spin		0 0.02 0
				#Tintgeom 1
				PyrRotate	-30 0 -70
			End
			Geom FX_Ivy_Ring03
			#Lifespan 60
		End

########################################

		Event
			Ename Anchor
			Type PositOnly
			At Hips
		End

		Event
			EName AnchorX
			At Root
			Type Local
		End
	End

	Condition
		On time
		Time 1

		Event
			Ename PRIME
			At AnchorX
			Type Start
			LookAt Anchor
			Magnet Anchor
			PMagnet Anchor
			Bhvr CustomizeablePowers\WaterBlast\WaterTube.bhvr
			BhvrOverride
				#PositionOffset 0 3 5
				PyrRotateJitter 0 180 0
				Scale 		.1 .1 .1
				EndScale 	2 2 .17
				FadeinLength	45
				FadeOutLength	30
				#TintGeom 1
				Alpha 255
				Spin 0 0 -0.0
			End
			Geom FX_Ivy_Beam
			LifeSpan 160
		End

		Event
			Ename PRIME3
			At Root
			Type Start
			BhvrOverride
				Scale 		2.5 2 2.5
				FadeinLength	45
				FadeOutLength	30
				#TintGeom 1
				Alpha 255
			End
			Geom FX_Ivy_Beam_Base4
			LifeSpan 160
		End

		Event
			Ename PRIME3
			At Root
			Type Start
			BhvrOverride
				PositionOffset	0 1 0
				Scale 		1.7 1.2 1.7
				FadeinLength	45
				FadeOutLength	30
				#TintGeom 1
				Spin		0 -0.01 0
				SpinJitter	0 0.02 0
				Alpha 255
			End
			Geom FX_Ivy_Beam_Base1
			LifeSpan 160
		End
	End

End