#
FxInfo

	LifeSpan 200


###########################################################
Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	WB_WaterBurst 100 100 .8
	End
End

	Condition
		On Time
		Time 0

		Event
			Ename SplodeAnchor
			At T_Root
			Type StartPositOnly
		End

		Event
			At WepR
			Type Local
			EName mainAnchor
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :WaterBurst_DrawIn_Spray.part
			Part :WaterShot_GlowStreaks.part
			LifeSpan 30
		End

	End

	Condition
		On Time
		Time 3

		Event
			At mainAnchor
			Type Local
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :WaterBurst_DrawIn_Splatter.part
			LifeSpan 28
		End

	End

	Condition
		On Time
		Time 5

		Event
			At mainAnchor
			Type Local
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :WaterShot_HandGlow.part
			LifeSpan 25
		End

	End

	Condition
		On Time
		Time 24 #8

#		Event
#			At WepR
#			Type Start
#			EName Prime
#			LookAt T_Root
#			Magnet T_Root
#			Bhvr CustomizeablePowers\SonicAttack\SMLProjectile.bhvr
#			BhvrOverride
#				InitialVelocity 0 -1 0
#				TrackRate 2.5
#			End
#			#geom Geo_FX_TestSphere
#		End
#
#	End
#
#	Condition
#		On PrimeHit
#
#		Event
#			Type Destroy
#			EName Prime
#		End

		Event
			At SplodeAnchor
			Type StartPositOnly
			ChildFx :WaterBurst_Hit.fx
		End

	End

End
