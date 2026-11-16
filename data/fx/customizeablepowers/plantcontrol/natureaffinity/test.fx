FXinfo
Lifespan 100

	Condition
		On Time
		time 0

#		Event
#			At Root
#			Type Local
#			Bhvr :AirStrips.bhvr
#			BhvrOverride
#				Spin	0 0 -0.08
#				PyrRotate -90 0 0
#				PositionOffset 0 2 0
#				FadeInLength 60
#				FadeOutLength 60
#				Alpha 120
#				#Scale 2 1.5 2
#				StartColor 255 255 255
#			End
#			geom FX_WindWisp
#			Lifespan 30
#		End
#
#		Event
#			At Root
#			Type Local
#			Bhvr :AirStrips.bhvr
#			BhvrOverride
#				#Scale 		2 1.5 2
#				StartColor 	255 255 255
#				Spin		0 0 -0.08
#				PyrRotate 	-90 0 180
#				PositionOffset	0 2 0
#				FadeInLength 	60
#				FadeOutLength 	60
#				Alpha 		120
#			End
#			geom FX_WindWisp
#			Lifespan 30
#		End

		Event
			Ename ConeAnchor
			Type StartPositOnly
			At Root
			BhvrOverride
				PositionOffset 0 0 0
				#Scale 15 15 15
			End
		End

		Event
			At ConeAnchor
			#Type Local
			Type CREATE
  			Inherit	Rotation Position
  			Update	Rotation Position
			EName AnchorY
			BhvrOverride
				PositionOffset 0 0 0
			End
			#Geom geo_FX_TestSphere
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
				PositionOffset 0 1 0
			End
		End

		Event
			At AnchorX
			Type PositOnly
			EName ConeTarget
		End
	End

	Condition
		On time
		Time 1

		Event
			Ename PRIME
			At ConeTarget
			Type PositOnly
			LookAt AnchorG
			Magnet AnchorG
			PMagnet AnchorG
			Bhvr CustomizeablePowers\BioArmor\Siphon_Geo.bhvr
			BhvrOverride
				#PositionOffset 0 3 5
				#PyrRotateJitter 0 0 90
				Scale 		.1 .1 .1
				EndScale 	2 1.7 2
				FadeOutLength	0
				#TintGeom 1
				Alpha 255
				#Spin 0 0 -0.15
			End
			Geom FX_Ivy_Beam
			LifeSpan 15
		End

		Event
			Ename PRIME2
			At ConeTarget
			Type PositOnly
			LookAt AnchorG
			Magnet AnchorG
			PMagnet AnchorG
			Bhvr CustomizeablePowers\BioArmor\Siphon_Geo.bhvr
			BhvrOverride
				#PositionOffset 0 3 5
				#PyrRotateJitter 0 0 90
				Scale 		.1 .1 .1
				EndScale 	2 1.7 2
				FadeOutLength	20
				#TintGeom 1
				Alpha 255
				#Spin 0 0 -0.08
			End
		#	Geom FX_Ivy_Beam_Base1
			LifeSpan 20
		End

		Event
			Ename PRIME3
			At PRIME2
			Type local
#			LookAt AnchorG
#			Magnet AnchorG
#			PMagnet AnchorG
#			Bhvr CustomizeablePowers\BioArmor\Siphon_Geo.bhvr
#			BhvrOverride
#				#PositionOffset 0 3 5
#				#PyrRotateJitter 0 0 90
#				Scale 		.1 .1 .1
#				EndScale 	2 1.7 2
#				FadeOutLength	20
#				#TintGeom 1
#				Alpha 255
#				#Spin 0 0 -0.08
#			End
		#	Geom FX_Ivy_Beam_Base2
			LifeSpan 20
		End
	End

	Condition
		On time
		time 15

		Event
			Ename PRIME3
			At ConeTarget
			Type PositOnly
			LookAt AnchorG
			Magnet AnchorG
			PMagnet AnchorG
			Bhvr CustomizeablePowers\BioArmor\Siphon_Geo.bhvr
			BhvrOverride
				#PositionOffset 0 3 5
				#PyrRotateJitter 0 0 90
				Scale 		2 1.7 2
				EndScale 	.1 .1 .1
				FadeOutLength	30
				#TintGeom 1
				Alpha 255
			#	Spin 0 0 -0.15
			End
			Geom FX_Ivy_Beam
			LifeSpan 15
		End

	End

End