FXinfo
Lifespan 180

	Condition
		On Time
		time 0

		Event
			Ename ConeAnchor
			Type Local
			At Root
			BhvrOverride
				#PositionOffset 0 -2.5 0
				#Scale 15 15 15
			End
		End

		Event
			At ConeAnchor
			Type Local
			EName AnchorY
			BhvrOverride
				PositionOffset 0 0 0
			End
		End

		Event
			At AnchorY
			Type PositOnly
			EName AnchorG
		End

		Event
			At ConeAnchor
			Type Local
			EName AnchorX
			BhvrOverride
				PositionOffset 0 0 0
			End
		End

		Event
			At AnchorX
			Type PositOnly
			EName ConeTarget
		End

#		Event
#			Type Local
#			At Root
#			Bhvr	behaviors\Splat_RuneCircle.bhvr
#			BhvrOverride
#				FadeInLength	120
#				PositionOffset	0 0 25
#				Alpha		255
#				StartColor	120 150 100
#				#PrimaryTint	80
#				#SecondaryTint	20
#				Scale		.1 .1 .1
#				#SplatFlags	ADDITIVE
#			End
#			Splat	Tintable_PlantGrowth_Cone
#			#Lifespan 60
#		End
#	#
#	#	Event
#	#		Type Local
#	#		At Root
#	#		Bhvr	behaviors\Splat_RuneCircle.bhvr
#	#		BhvrOverride
#	#			FadeInLength	120
#	#			#PositionOffset	0 0 25
#	#			Alpha		255
#	#			StartColor	80 180 75
#	#			#PrimaryTint	80
#	#			#SecondaryTint	20
#	#			Scale		45 20 45
#				#SplatFlags	ADDITIVE
#			End
#		#	Splat	Tintable_PlantGrowth_Round
#			#Lifespan 60
#		End

#		Event
#			Type Local
#			At AnchorY
#			Bhvr	behaviors\Splat_RuneCircle.bhvr
#			BhvrOverride
#				FadeInLength	20
#				#PositionOffset	0 0 25
#				Alpha		255
#				StartColor	80 180 75
#				PrimaryTint	80
#				SecondaryTint	20
#				Scale		5.5 5.5 5.5
#				Spin		0 0.02 0
#				#Tintgeom 1
#	#			#SplatFlags	ADDITIVE
#			End
#			Geom FX_Ivy_Ring_Big
#			#Lifespan 60
#		End
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
				#Spin		0 0.02 0-
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


	End

########################################

	Condition
		On Time
		time 0

		Event
			Ename Anchor2
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
			LookAt Anchor2
			Magnet Anchor2
			PMagnet Anchor2
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

#	Condition
#		On time
#		time 15
#
#		Event
#			Ename PRIME3
#			At ConeTarget2
#			Type PositOnly
#			LookAt AnchorG2
#			Magnet AnchorG2
#			PMagnet AnchorG2
#			Bhvr CustomizeablePowers\BioArmor\Siphon_Geo.bhvr
#			BhvrOverride
#				#PositionOffset 0 3 5
#				#PyrRotateJitter 0 0 90
#				Scale 		2 1.7 2
#				EndScale 	.1 .1 .1
#				FadeOutLength	30
#				#TintGeom 1
#				Alpha 255
#				Spin 0 0 -0.15
#			End
#	#		Geom FX_Ivy_Beam
#		#	LifeSpan 15
#		End
#
#	End


End