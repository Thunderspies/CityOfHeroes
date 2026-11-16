#
FxInfo

	Input
		InpName Hips
	End

	LifeSpan 200


###########################################################

	Condition
		On Time
		Time 19

		Event
			At Root
			Type Local
			EName roothookup
			Bhvr behaviors/alpha.bhvr
			Geom RootToChestAdjustment
		End

		Event
			AltPiv 1
			At roothookup
			Type Local
			EName hookup
			Geom LungeNode
		End

		Event
			AltPiv 2
			At hookup
			Type Start
			EName Prime
			LookAt Target
			Magnet Target
			Bhvr :Shuriken.bhvr
			Part :ShurikenStreaks01.part
			Geom Tintable_GEO_WepR_Shuriken
		End

	End

	Condition
		On 	Time
		Time 	14.5

		Event
			EName	Streak
			Type	local
			At	Chest
			bhvr	behaviors/Claws.bhvr
			Sound clawtwirl5 100.0 100.0 .7
		End

	End

	Condition
		On PrimeHit

		Event
			At T_Chest
			Type Posit
			EName Fire
			Part :FireExplosion_Burst.part
			LifeSpan 8
		End

		Event
			At T_Chest
			Type Posit
			EName Sparks
			Part :GernadeLauncherSparks.part
			Part :FireExplosion.part
			Part :FireExplosion_Dark.part
			Sound grexplo2 120.0 120.0 1.0
			LifeSpan 5
		End

	Event
			EName 	Sparks
			Type	Posit
			At	T_Chest
			Part	:FireExplosionFlash.part
			Lifespan 5
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
