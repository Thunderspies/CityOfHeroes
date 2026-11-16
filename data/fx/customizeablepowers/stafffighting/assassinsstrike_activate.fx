#
FxInfo

	LifeSpan 100


###########################################################

	Condition
		On 	Time
		Time	0
		Event
			Type	Local
			At	Root
			#Sound 	SM_SerpentsReach_Attack_01 100 100 .8
		End
	End

	Condition
		On Time
		Time 	3

		Event
			At C_Hips
			Type Local
			#Sound Brawling_SpinningStrike_Attack_01 200 100 0.8
		End

	End

	Condition
		On Time
		Time	0

		Event
			At Target
			Type StartPositOnly
			EName TargetAnchor
		End

		Event
			At chest
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
			Part CustomizeablePowers\StaffFighting\Projectile_AirStreaks.part
			Part CustomizeablePowers\StaffFighting\Projectile_AirStreaks_Additive.part
			Part CustomizeablePowers\StaffFighting\Projectile_AirStreaks_Additive_Core.part
			LifeSpan 0
		End

		Event
			At Root
			Type Start
			EName Prime2
			LookAt T_Root
			Magnet T_Root
			Bhvr CustomizeablePowers\StaffFighting\SerpentsReach_Projectile.bhvr
			Part CustomizeablePowers\StaffFighting\Ground_Dust_Traveling.part
			Part CustomizeablePowers\StaffFighting\Projectile_AirStreaks_Flat.part
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

End
