#                                                          
FxInfo

	LifeSpan 65


###########################################################

	Condition

		Event
			At Chest
			Type Posit
			EName Fire
			Part :FireExplosion.part
			LifeSpan 8
		End

		Event
			At Chest
			Type Posit
			EName Sparks
			Part :GernadeLauncherSparks.part
			Part :FireExplosionFlash.part
			Sound grexplo2 120.0 120.0 1.0
			LifeSpan 5
		End

		Event
			At Chest
			Type Local
			EName Prime
			Part :Shotgunflash.part
			Part :ShotgunflashStar.part
			LifeSpan 15
		End

		Event
			At Chest
			Type Start
			EName ExplosiveForce
			BhvrOverride
				physForcePower 200
				physForcePowerJitter 20
				physForceRadius 15
				physForceType Out
			End
			LifeSpan 1
		End

	End

	Condition
		On Time
		Time 0
		Repeat 11

		Event
			At Chest
			Type Start
			ChildFx :CHILD_Explosion_Shrapnel.fx
			EName Sparkout
			BhvrOverride
				PositionOffset 0 0 0
			End
		End

		Event
			At Chest
			Type Start
			ChildFx :CHILD_Explosion_Shrapnel_Out.fx
			EName Sparkout
			BhvrOverride
				PositionOffset 0 0 0
			End
		End

		Event
			At Chest
			Type Start
			ChildFx :CHILD_Explosion_Shrapnel.fx
			EName Sparkout
			HardwareOnly 
			BhvrOverride
				PositionOffset 0 0 0
			End
		End

		Event
			At Chest
			Type Start
			ChildFx :CHILD_Explosion_Shrapnel_Out.fx
			EName Sparkout
			HardwareOnly 
			BhvrOverride
				PositionOffset 0 0 0
			End
		End

	End

End
