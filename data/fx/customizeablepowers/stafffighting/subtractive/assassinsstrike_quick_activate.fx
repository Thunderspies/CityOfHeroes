#############################################################
## SmashingBlow_Activate.fx
#############################################################

FxInfo
Lifespan 80

#############################################################

	Condition
		On	Time
		Time	1

		Event
			Ename	ConeOffset
			Type	Start
			At	Root
			BhvrOverride
				PYRRotate	0 180 0
				Scale		1.85 1.85 1.85
			End
			GEOM	FX_ConeOffsets
		End

		Event
			EName 	TargetOffset
			Type	Local
			At	ConeOffset
			BhvrOverride
				PositionOffset	0 2.0 0
			End
			AltPiv	1
		End

		Event
			Type	Start
			At	TargetOffset
			Part	:Gust_Streaks_Thin.part
			Part	:Gust_Rings_Thin.part
			PMagnet	Target
			Lifespan 15
		End

##############################

		Event
			At Target
			Type StartPositOnly
			EName TargetAnchor
		End

		Event
			At Chest
			Type Local
			EName SwishAnchor
			LifeSpan 10
		End

		Event
			At SwishAnchor
			Type Start
			EName Prime
			LookAt TargetAnchor
			Magnet TargetAnchor
			Bhvr CustomizeablePowers\StaffFighting\SerpentsReach_Projectile.bhvr
			Part :Projectile_AirStreaks_Sm.part
			Part :Projectile_AirStreaks_Sm_Additive.part
			LifeSpan 0
		End

		Event
			At Root
			Type Start
			EName Prime2
			LookAt T_Root
			Magnet T_Root
			Bhvr CustomizeablePowers\StaffFighting\SerpentsReach_Projectile.bhvr
			Part :Projectile_AirStreaks_Flat.part
			LifeSpan 0
		End

	End

	Condition
		On Prime2Hit

		Event
			Type Destroy
			EName Prime1
		End

		Event
			Type Destroy
			EName Prime2
		End

	End
#############################################################

End