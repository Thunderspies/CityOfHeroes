#
FxInfo
Lifespan 400

############################################################

	Condition
		On Time
		Time 0

		Event
			Ename Anchor
			At Chest
			Type Local
			BhvrOverride
				PyrRotate 	15 -10 45
				PositionOffset	-0.18 0.1 -0.05
			End
		End

		Event
			Ename Anchor2
			At Hips
			Type Local
			BhvrOverride
				PyrRotate 	5 -5 -5
				PositionOffset	0 0 0
			End
		End

		Event
			Ename Anchor3
			At UlegL
			Type Local
			BhvrOverride
				PyrRotate 	15 -5 -15
				PositionOffset	-0.058 -1.5 0.2
			End
		End

		Event
			Ename Anchor4
			At LlegL
			Type Local
			BhvrOverride
				PyrRotate 	20 -185 -15
				PositionOffset	0 -1.5 -.5
			End
		End
	End

	Condition
		On time
		Time 0

		Event
			Ename WreathLegB
			At Anchor4
			Type Local
			Bhvr CustomizeablePowers\PlantControl\NatureAffinity\QuickGrow.bhvr
			BhvrOverride
				TintGeom	1
				FadeInLength	60
				Scale		0.5 0.2 0.5
				Endscale	.9 1 .9
			End
			Geom FX_Tintable_Leaf_Wreath_LegL
			LifeSpan 300
		End

	End

	Condition
		On time
		Time 5

		Event
			Ename WreathLegA
			At Anchor3
			Type Local
			Bhvr CustomizeablePowers\PlantControl\NatureAffinity\QuickGrow.bhvr
			BhvrOverride
				TintGeom	1
				FadeInLength	60
				Scale		0.5 0.2 0.5
				Endscale	1.4 1 1.4
			End
			Geom FX_Tintable_Leaf_Wreath_LegL
			LifeSpan 300
		End
	End

	Condition
		On time
		Time 5

		Event
			Ename WreathHips
			At Anchor2
			Type Local
			Bhvr CustomizeablePowers\PlantControl\NatureAffinity\MushGrow.bhvr
			BhvrOverride
				Alpha		1
				FadeInLength	60
				Scale		0.5 0.2 0.5
				Endscale	1.0 1.2 1.0
			End
			Geom FX_Leaf_Wreath
			LifeSpan 300
		End

		Event
			Ename WreathHips
			At Anchor2
			Type Local
			Bhvr CustomizeablePowers\PlantControl\NatureAffinity\MushGrow.bhvr
			BhvrOverride
				TintGeom	1
				FadeInLength	60
				Scale		0.5 0.2 0.5
				Endscale	1.0 1.2 1.0
			End
			Geom FX_Tintable_Leaf_Wreath
			LifeSpan 300
		End

	End

	Condition
		On time
		Time 15

		Event
			Ename WreathChest
			At Anchor
			Type Local
			Bhvr CustomizeablePowers\PlantControl\NatureAffinity\MushGrow.bhvr
			BhvrOverride
				Alpha 		1
				FadeInLength	60
				Scale		1 0.8 1
				Endscale	1.2 1.4 1.4
			End
			Geom FX_Leaf_Wreath
			LifeSpan 300
		End

		Event
			Ename WreathChest
			At Anchor
			Type Local
			Bhvr CustomizeablePowers\PlantControl\NatureAffinity\MushGrow.bhvr
			BhvrOverride
				TintGeom	1
				FadeInLength	60
				Scale		1 0.8 1
				Endscale	1.2 1.4 1.4
			End
			Geom FX_Tintable_Leaf_Wreath
			LifeSpan 300
		End
	End

#########################################

	Condition
		On time
		Time 15
		Chance 0.8

		Event
			Ename flowerA
			At WreathChest
			Type Local
			Altpiv 1
			Bhvr CustomizeablePowers\PlantControl\NatureAffinity\MushGrow.bhvr
			BhvrOverride
				TintGeom	1
				FadeInLength	60
#				Scale		0.8 0.5 0.8
#				Endscale	0.9 1 1
				#StartJitter	0.3 0.3 0
				PyrRotate	180 -40 90
				PyrRotateJitter	30 30 180
				#StartColor	255 255 255
				PositionOffset	0 0 0
			End
			geom FX_Flower_01
		End

	End

	Condition
		On time
		Time 15
		Chance 0.8

		Event
			Ename flowerB
			At WreathChest
			Type Local
			Altpiv 2
			Bhvr CustomizeablePowers\PlantControl\NatureAffinity\MushGrow.bhvr
			BhvrOverride
				TintGeom	1
				FadeInLength	60
#				Scale		0.8 0.5 0.8
#				Endscale	0.9 1 1
				#StartJitter	0.3 0.3 0
				PyrRotate	180 40 90
				PyrRotateJitter	30 30 180
				#StartColor	255 255 255
				PositionOffset	0 0 0
			End
			geom FX_Flower_01
		End

	End

	Condition
		On time
		Time 15
		Chance 0.8

		Event
			Ename flowerC
			At WreathChest
			Type Local
			Altpiv 3
			Bhvr CustomizeablePowers\PlantControl\NatureAffinity\MushGrow.bhvr
			BhvrOverride
				TintGeom	1
				FadeInLength	60
				#Scale		0.8 0.5 0.8
				Endscale	0.8 0.8 0.8
				#StartJitter	0.3 0.3 0
				PyrRotate	15 -15 100
				PyrRotateJitter	30 30 180
				#StartColor	255 255 255
				PositionOffset	0 -0.25 0
			End
			geom FX_Flower_01
		End

	End

	Condition
		On time
		Time 15
		Chance 0.8

		Event
			Ename flowerD
			At WreathChest
			Type Local
			Altpiv 4
			Bhvr CustomizeablePowers\PlantControl\NatureAffinity\MushGrow.bhvr
			BhvrOverride
				TintGeom	1
				FadeInLength	60
#				Scale		0.8 0.5 0.8
				Endscale	0.9 0.9 0.9
				#StartJitter	0.3 0.3 0
				PyrRotate	0 20 50
				PyrRotateJitter	30 30 180
				#StartColor	255 255 255
				PositionOffset	0 -0.02 0
			End
			geom FX_Flower_01
		End

	End
#
#	Condition
#		On time
#		Time 45
#	#	Chance 0.8
#
#		Event
#			Ename flowerF
#			At WreathChest
#			Type Local
#			Altpiv 5
#			Bhvr CustomizeablePowers\PlantControl\NatureAffinity\MushGrow.bhvr
#			BhvrOverride
#				FadeInLength	60
##				Scale		0.8 0.5 0.8
##				Endscale	0.9 1 1
#				#StartJitter	0.3 0.3 0
#				PyrRotate	180 40 90
#				PyrRotateJitter	15 15 15
#				StartColor	255 255 255
#				PositionOffset	0 0 -0.2
#			End
#			geom FX_Flower_01
#		End
#
#	End
#
	Condition
		On time
		Time 15
		Chance 0.8

		Event
			Ename flowerE
			At WreathChest
			Type Local
			Altpiv 6
			Bhvr CustomizeablePowers\PlantControl\NatureAffinity\MushGrow.bhvr
			BhvrOverride
				TintGeom	1
				FadeInLength	60
#				Scale		0.8 0.5 0.8
#				Endscale	0.9 1 1
				#StartJitter	0.3 0.3 0
				PyrRotate	0 10 90
				PyrRotateJitter	15 15 15
				#StartColor	255 255 255
				PositionOffset	0 0 0.015
			End
			geom FX_Flower_01
		End

	End

	Condition
		On time
		Time 15
		Chance 0.6

		Event
			Ename flowerG
			At WreathChest
			Type Local
			Altpiv 7
			Bhvr CustomizeablePowers\PlantControl\NatureAffinity\MushGrow.bhvr
			BhvrOverride
				TintGeom	1
				FadeInLength	60
#				Scale		0.8 0.5 0.8
				Endscale	0.8 0.8 0.8
				#StartJitter	0.3 0.3 0
				PyrRotate	0 40 90
				PyrRotateJitter	15 15 15
				#StartColor	255 255 255
				PositionOffset	0 0.2 -0.05
			End
			geom FX_Flower_01
		End

	End

#	Condition
#		On time
#		Time 45
#	#	Chance 0.8
#
#		Event
#			Ename flowerH
#			At WreathChest
#			Type Local
#			Altpiv 8
#			Bhvr CustomizeablePowers\PlantControl\NatureAffinity\MushGrow.bhvr
#			BhvrOverride
#				FadeInLength	60
##				Scale		0.8 0.5 0.8
##				Endscale	0.9 1 1
#				#StartJitter	0.3 0.3 0
#				PyrRotate	180 40 90
#				PyrRotateJitter	15 15 15
#				StartColor	255 255 255
#				PositionOffset	0 0 -0.2
#			End
#			geom FX_Flower_01
#		End
#
#	End
#
	Condition
		On time
		Time 15
		Chance 0.6

		Event
			Ename flowerI
			At WreathChest
			Type Local
			Altpiv 9
			Bhvr CustomizeablePowers\PlantControl\NatureAffinity\MushGrow.bhvr
			BhvrOverride
				TintGeom	1
				FadeInLength	60
#				Scale		0.8 0.5 0.8
				Endscale	0.9 0.9 0.9
				#StartJitter	0.3 0.3 0
				PyrRotate	180 0 90
				PyrRotateJitter	15 15 15
				#StartColor	255 255 255
				PositionOffset	0 -0.1 -0
			End
			geom FX_Flower_01
		End
	End
#
	Condition
		On time
		Time 15
		Chance 0.8

		Event
			Ename flowerI
			At WreathChest
			Type Local
			Altpiv 10
			Bhvr CustomizeablePowers\PlantControl\NatureAffinity\MushGrow.bhvr
			BhvrOverride
				TintGeom	1
				FadeInLength	60
#				Scale		0.8 0.5 0.8
				Endscale	0.9 0.9 0.9
				#StartJitter	0.3 0.3 0
				PyrRotate	180 60 90
				PyrRotateJitter	15 15 15
				#StartColor	255 255 255
				PositionOffset	0 -0.1 -0.1
			End
			geom FX_Flower_01
		End
	End
#
	Condition
		On time
		Time 15
		Chance 0.6

		Event
			Ename flowerI
			At WreathChest
			Type Local
			Altpiv 11
			Bhvr CustomizeablePowers\PlantControl\NatureAffinity\MushGrow.bhvr
			BhvrOverride
				TintGeom	1
				FadeInLength	60
#				Scale		0.8 0.5 0.8
				Endscale	0.9 0.9 0.9
				#StartJitter	0.3 0.3 0
				PyrRotate	120 80 90
				PyrRotateJitter	15 15 15
				#StartColor	255 255 255
				PositionOffset	0 -0.1 -0
			End
			geom FX_Flower_01
		End
	End
#
	Condition
		On time
		Time 15
		Chance 0.6

		Event
			Ename flowerJ
			At WreathHips
			Type Local
			Altpiv 9
			Bhvr CustomizeablePowers\PlantControl\NatureAffinity\MushGrow.bhvr
			BhvrOverride
				TintGeom	1
				FadeInLength	60
#				Scale		0.8 0.5 0.8
				Endscale	0.9 0.9 0.9
				#StartJitter	0.3 0.3 0
				PyrRotate	180 0 90
				PyrRotateJitter	15 15 15
				#StartColor	255 255 255
				PositionOffset	0 -0.1 -0
			End
			geom FX_Flower_01
		End
	End
#
	Condition
		On time
		Time 15
		Chance 0.8

		Event
			Ename flowerK
			At WreathHips
			Type Local
			Altpiv 2
			Bhvr CustomizeablePowers\PlantControl\NatureAffinity\MushGrow.bhvr
			BhvrOverride
				TintGeom	1
				FadeInLength	60
#				Scale		0.8 0.5 0.8
				Endscale	0.9 0.9 0.9
				#StartJitter	0.3 0.3 0
				PyrRotate	180 40 90
				PyrRotateJitter	15 15 15
				#StartColor	255 255 255
				PositionOffset	0 0.1 0.05
			End
			geom FX_Flower_01
		End
	End
#
	Condition
		On time
		Time 15
		Chance 0.8

		Event
			Ename flowerL
			At WreathHips
			Type Local
			Altpiv 10
			Bhvr CustomizeablePowers\PlantControl\NatureAffinity\MushGrow.bhvr
			BhvrOverride
				TintGeom	1
				FadeInLength	60
#				Scale		0.8 0.5 0.8
				Endscale	0.5 0.9 0.9
				#StartJitter	0.3 0.3 0
				PyrRotate	160 90 90
				PyrRotateJitter	15 15 15
				#StartColor	255 255 255
				PositionOffset	-0.15 -0.2 0.05
			End
			geom FX_Flower_01
		End
	End
#
	Condition
		On time
		Time 15
		Chance 0.6

		Event
			Ename flowerM
			At WreathHips
			Type Local
			Altpiv 11
			Bhvr CustomizeablePowers\PlantControl\NatureAffinity\MushGrow.bhvr
			BhvrOverride
				TintGeom	1
				FadeInLength	60
#				Scale		0.8 0.5 0.8
				Endscale	0.5 0.9 0.9
				#StartJitter	0.3 0.3 0
				PyrRotate	120 110 90
				PyrRotateJitter	15 15 15
				#StartColor	255 255 255
				PositionOffset	-0 -0.1 0.
			End
			geom FX_Flower_01
		End
	End
#
	Condition
		On time
		Time 15
		Chance 0.8

		Event
			Ename flowerN
			At WreathHips
			Type Local
			Altpiv 3
			Bhvr CustomizeablePowers\PlantControl\NatureAffinity\MushGrow.bhvr
			BhvrOverride
				TintGeom	1
				FadeInLength	60
#				Scale		0.8 0.5 0.8
				#Endscale	1 1 1
				#StartJitter	0.3 0.3 0
				PyrRotate	0 0 90
				PyrRotateJitter	15 15 15
				#StartColor	255 255 255
				PositionOffset	-0.15 -0.25 0.00
			End
			geom FX_Flower_01
		End
	End
End
