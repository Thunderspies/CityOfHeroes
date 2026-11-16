#
FxInfo

	LifeSpan 150
Input
	InpName	Hips
End

###########################################################

	Condition
		On time
		time 3

		Event
			At LArmR
			type Local
			Geom FX_Kelp_Wreath_LegL
			Bhvr CustomizeablePowers\PlantControl\NatureAffinity\QuickGrow.bhvr
			BhvrOverride
				FadeInLength	20
				PyrRotate	0 15 -5
				PositionOffset	0.7 0 0.1
			End
		End

		Event
			At LArmR
			type Local
			Geom FX_Kelp_Wreath_LegL
			Bhvr CustomizeablePowers\PlantControl\NatureAffinity\QuickGrow.bhvr
			BhvrOverride
				FadeInLength	20
				PyrRotate	180 -0 -15
				PositionOffset	0.9 0 0.0
			End
		End
	End

	Condition
		On time
		time 10

		Event
			At LarmR
			type Local
			Geom FX_NatureBlast_Flower_Head
			Bhvr CustomizeablePowers\PlantControl\NatureAffinity\QuickGrow.bhvr
			BhvrOverride
				FadeInLength	20
				FadeoutLength	0
				#PyrRotate	180 -0 -15
				PositionOffset	1.5 0 0.0
				#Scale		2 .75 .75
			End
			Lifespan 10
		End
	End

	Condition
		On time
		time 15

		Event
			At LarmR
			type Local
			Geom FX_NatureBlast_Flower_Head
			Bhvr CustomizeablePowers\PlantControl\NatureAffinity\QuickGrow.bhvr
			BhvrOverride
				FadeInLength	0
				FadeOutLength	45
				#PyrRotate	180 -0 -15
				PositionOffset	1.5 0 0.0
				Scale		2 .75 .75
			End
			Lifespan 5
		End
	End

######################################################
#
	Condition
		On Time
		Time 15

#		Event
#			At wepr
#			Type Start
#			EName Prime
#			LookAt Target
#			Magnet Target
#			Bhvr CustomizeablePowers\SonicAttack\SMLProjectile.bhvr
#			BhvrOverride
#				TrackRate 	3
#				Spin		1 0 0
#				SpinJitter	0 0 0
#			End
#		End



		Event
			EName Prime
			At WepR
			Type Start
			Bhvr CustomizeablePowers\BeastMastery\CallSwarm_CC_Projectile.bhvr
			BhvrOverride
				InitialVelocity		0.0 -0.2 0.0
				Trackrate		3
			End
			LookAt Target
			Magnet Target
		End

		Event
			EName Prime2
			At WepR
			Type Start
			Bhvr CustomizeablePowers\BeastMastery\CallSwarm_CC_Projectile.bhvr
			BhvrOverride
				InitialVelocity		0.0 0.2 0.0
				Trackrate		3
			End
			LookAt Target
			Magnet Target
		End

		Event
			EName Trail
			At Prime
			Type Local
			BhvrOverride
				PositionOffset	0.2 0.0 0
			End
			part :Ivy_Trail.part
		End

		Event
			EName Trail2
			At Prime2
			Type Local
			BhvrOverride
				PositionOffset	-0.2 0.0 0.0
			End
			part :Ivy_Trail.part
		End
	End



	Condition
		On PrimeHit

		Event
			Type Destroy
			EName Prime
		End

		Event
			Type Destroy
			EName Prime2
		End

	End

End
