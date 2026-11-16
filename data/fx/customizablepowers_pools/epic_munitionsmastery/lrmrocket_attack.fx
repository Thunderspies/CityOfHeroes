#                                                          
FxInfo

	LifeSpan 100


###########################################################

	Condition
		On Time
		Time 3

		Event
			At C_Chest
			Type Start
			BhvrOverride
				TintGeom 1
			End
			Geom Tintable_FX_MachineGunBurst
			LifeSpan 1
		End

		Event
			At C_Chest
			Type Start
			EName Prime
			LookAt Target
			Magnet Target
			Bhvr Behaviors/Missleprojectile.bhvr
			BhvrOverride
				TintGeom 1
			End
			Part :5thmissile_smoke.part
			Part :5thmissile_smoke2.part
			Part :5thmissile_fire.part
			Geom Tintable_Missile
		End

		Event
			At C_Chest
			Type Start
			EName Blast1
			Part :Gunfire_ShotgunMuzzflash.part
			Part :Gunfire_Shotgunsmoke.part
			Part :Gunfire_ShotgunMuzzflashSmall.part
			Sound rocketlaunch 100.0 100.0 0.9
		End

		Event
			At C_Chest
			Type Start
			EName KickBackSmoke
			Part :RocketLauncherSmoke.part
			Part :Gunfire_Shotgunsmoke.part
			Part :Gunfire_MissleShell.Part
		End

	End

	Condition
		On PrimeHit
		Repeat 7

		Event
			At T_Chest
			Type Start
			ChildFx :CHILD_Explosion_Shrapnel.fx
			EName Sparkup
			BhvrOverride
				PositionOffset 0 0 0
			End
		End

	End

	Condition
		On PrimeHit
		Repeat 18

		Event
			At T_Chest
			Type Start
			ChildFx :CHILD_Explosion_Shrapnel.fx
			EName Sparkup
			HardwareOnly 
			BhvrOverride
				PositionOffset 0 0 0
			End
		End

	End

	Condition
		On PrimeHit

		Event
			At T_Chest
			Type Posit
			EName Fire
			Part :FireExplosion.part
			LifeSpan 8
		End

		Event
			At T_Chest
			Type Posit
			EName Sparks
			Part :GernadeLauncherSparks.part
			Sound grexplo2 120.0 120.0 1.0
			LifeSpan 5
		End

		Event
			At T_Chest
			Type Posit
			EName Sparks
			Part :FireExplosionFlash.part
			LifeSpan 5
		End

		Event
			At T_Chest
			Type Start
			EName ExplosiveForce
			BhvrOverride
				physForceType Out
				physForceRadius 20
				physForcePower 500
				physForcePowerJitter 25
			End
			LifeSpan 1
		End

		Event
			Type Destroy
			EName Prime
		End

		Event
			Type Destroy
			EName Explosion
		End

	End

	Condition
		On Time
		Time 6

		Event
			Type Destroy
			EName Blast1
		End

	End

	Condition
		On Time
		Time 10

		Event
			Type Destroy
			EName KickBackSmoke
		End

	End

End
