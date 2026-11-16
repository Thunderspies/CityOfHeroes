#
FxInfo

	LifeSpan 180


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Hips
			Type Local
			Bhvr behaviors\Splat_RuneCircle.bhvr
			BhvrOverride
				TintGeom 1
				PositionOffset 0 0.3 0
				PyrRotate 0 0 45
				Scale 1.3 1.3 1.3
				#Spin 0 0.02 0
				#TintGeom 1
				Alpha 255
				FadeInLength 20
				PrimaryTint 80
				SecondaryTint 20
				#StartColor 80 180 75
			End
			Geom FX_Tintable_Ivy_Ring05
			#LifeSpan 60
		End

		Event
			At Hips
			Type Local
			Bhvr behaviors\Splat_RuneCircle.bhvr
			BhvrOverride
				TintGeom 1
				FadeInLength 20
				PositionOffset 0 0 0
				Alpha 255
				#StartColor 80 180 75
				PrimaryTint 80
				SecondaryTint 20
				Scale 1.15 1.15 1.15
				#Spin 0 0.02 0
				#TintGeom 1
				PyrRotate 0 0 -45
			End
			Geom FX_Tintable_Ivy_Ring05
			#LifeSpan 60
		End

		Event
			At Hips
			Type Local
			Bhvr behaviors\Splat_RuneCircle.bhvr
			BhvrOverride
				TintGeom 1
				FadeInLength 20
				PositionOffset -0.3 0.3 0
				Alpha 255
				#StartColor 80 180 75
				PrimaryTint 80
				SecondaryTint 20
				Scale 1.5 1.5 1.5
				#Spin 0 0.02 0
				#TintGeom 1
				PyrRotate 0 0 -90
			End
			Geom FX_Tintable_Ivy_Ring05
			#LifeSpan 60
		End

		Event
			At Hips
			Type Local
			Bhvr behaviors\Splat_RuneCircle.bhvr
			BhvrOverride
				TintGeom 1
				FadeInLength 20
				PositionOffset -0.3 0.3 0
				Alpha 255
				#StartColor 80 180 75
				PrimaryTint 80
				SecondaryTint 20
				Scale .8 .8 1
				#Spin 0 0.02 0
				#TintGeom 1
				PyrRotate 0 0 -90
			End
			Geom FX_Tintable_Ivy_Ring05
			#LifeSpan 60
		End

		Event
			At ULegL
			Type Local
			Bhvr behaviors\Splat_RuneCircle.bhvr
			BhvrOverride
				TintGeom 1
				FadeInLength 20
				PositionOffset -0 -1 0
				Alpha 255
				#StartColor 80 180 75
				PrimaryTint 80
				SecondaryTint 20
				Scale 1 1 1
				#Spin 0 0.02 0
				#TintGeom 1
				PyrRotate 0 0 -90
			End
			Geom FX_Tintable_Ivy_Ring05
			#LifeSpan 60
		End

		Event
			At LLegL
			Type Local
			Bhvr behaviors\Splat_RuneCircle.bhvr
			BhvrOverride
				TintGeom 1
				FadeInLength 20
				PositionOffset 0.1 0 -0.07
				Alpha 255
				#StartColor 80 180 75
				PrimaryTint 80
				SecondaryTint 20
				Scale .6 1 .6
				#Spin 0 0.02 0
				#TintGeom 1
				PyrRotate -30 0 -70
			End
			Geom FX_Tintable_Ivy_Ring05
			#LifeSpan 60
		End

		Event
			At LLegR
			Type Local
			Bhvr behaviors\Splat_RuneCircle.bhvr
			BhvrOverride
				TintGeom 1
				FadeInLength 20
				PositionOffset -0.3 -0.5 -0.07
				Alpha 255
				#StartColor 80 180 75
				PrimaryTint 80
				SecondaryTint 20
				Scale 1.3 1.3 1.3
				#Spin 0 0.02 0
				#TintGeom 1
				PyrRotate 30 0 70
			End
			Geom FX_Tintable_Ivy_Ring05
			#LifeSpan 60
		End

	End

	#Condition
		#On Time
		#Time 1

		#Event
			#At Chest
			#Type PositOnly
			#EName Anchor
		#End

		#Event
			#At Root
			#Type Local
			#EName AnchorX
			#BhvrOverride
				#PositionOffset 0 -0.5 0
			#End
		#End

		#Event
			#At AnchorX
			#Type StartPositOnly
			#EName PRIME
			#LookAt Anchor
			#Magnet Anchor
			#PMagnet Anchor
			#Bhvr CustomizeablePowers\WaterBlast\WaterTube.bhvr
			#BhvrOverride
				#PyrRotateJitter 0 180 0
				#EndScale 2 2 0.7
				#FadeInLength 45
				#FadeOutLength 30
				#Alpha 255
				#Spin 0 0 -0.0
			#End
			#LifeSpan 160
		#End

		#Event
			#At Root
			#Type Start
			#EName PRIME3
			#BhvrOverride
				#Scale 2.5 2 2.5
				#FadeInLength 45
				#FadeOutLength 30
				#Alpha 255
			#End
			#LifeSpan 160
		#End

		#Event
			#At Root
			#Type Start
			#EName PRIME3
			#BhvrOverride
				#PositionOffset 0 1 0
				#Scale 1.7 1.6 1.7
				#FadeInLength 45
				#FadeOutLength 30
				#Spin 0 -0.03 0
				#Alpha 255
			#End
			#LifeSpan 160
		#End

	#End

End
