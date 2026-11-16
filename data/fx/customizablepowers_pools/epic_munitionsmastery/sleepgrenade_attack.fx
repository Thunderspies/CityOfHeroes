#                                                          
FxInfo

	LifeSpan 240


###########################################################

	Condition
		On Time
		Time 7

		Event
			At C_Chest
			Type Start
			Geom Tintable_Fx_GunBurst
			Sound Grenade2 120.0 120.0 1.0
			LifeSpan 2
		End

		Event
			At C_Chest
			Type Start
			Part :Gunfire_ShotgunMuzzflash.part
			Part :Gunfire_Shotgunsmoke.part
			Part :Gunfire_ShotgunMuzzflashSmall.part
			LifeSpan 12
		End

	End

	Condition
		On Time
		Time 7

		Event
			At C_Chest
			Type Start
			EName Prime
			LookAt T_Root
			Magnet T_Root
			Bhvr behaviors\MorterLobbprojectile2.bhvr
			BhvrOverride
				TintGeom 1
			End
			Part :BeanBagDust2.part
			Geom grenade
		End

	End

	Condition
		On PrimeHit

		Event
			At T_Root
			Type Posit
			EName Smoke
			Part :TearGasExplodeSmoke1.part
			Part :TearGasExplodeSmoke2.part
			Part :TearGas01.part
			Part :TearGasFlat.part
			LifeSpan 60
		End

		Event
			At T_Root
			Type Posit
			EName Sparks
			Part :FireExplosionFlash.part
			Part :DynamiteExplodeSparks.part
			Sound grexplo2 120.0 120.0 1.0
			LifeSpan 5
		End

		Event
			At T_Root
			Type Start
			EName ExplosiveForce
			BhvrOverride
				physForceType Out
				physForceRadius 20
				physForcePower 100
				physForcePowerJitter 10
			End
			LifeSpan 1
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
