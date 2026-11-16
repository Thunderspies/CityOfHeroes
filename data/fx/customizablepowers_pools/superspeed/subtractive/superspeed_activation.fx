###########################################################

FxInfo

###########################################################

	Condition
		On Time
		Time 0

		Event
			At Root
			Type Local
			Flags PowerLoopingSound
			Bhvr behaviors/soundFade2.bhvr
			Sound ForceField5_loop 40.0 40.0 0.6
			LifeSpan 1
		End

		Event
			At Root
			Type Local
			EName Prime
			Part :SpeedGlowLocal02.part
			Part :SpeedEmberLocal.part
			Part :SpeedEmber.part
			Part :SpeedCloudBaseGlow.part
			Geom Dummy02
			Sound Run5 40.0 40.0 0.7
		End

		Event
			At Root
			Type Local
			Part :SpeedCloudBaseGlowFlat.part
			Part :SpeedStreaksLocal.part
			Part :SpeedStreaks.part
		End

		Event
			AltPiv 1
			At Prime
			Type Local
			Part :SpeedFlame01.part
			Part :SpeedFire.part
		End

		Event
			AltPiv 2
			At Prime
			Type Local
			Part :SpeedFlame01.part
			Part :SpeedFire.part
		End

		Event
			AltPiv 1
			At Prime
			Type Local
			Part :SpeedCloudStreaks.part
			Part :SpeedFire.part
			Part :SpeedGlow.part
		End

		Event
			AltPiv 2
			At Prime
			Type Local
			Part :SpeedCloudStreaks.part
			Part :SpeedFire.part
			Part :SpeedGlow.part
		End

		Event
			At FootR
			Type Local
			PMagnet Head
			Part :SpeedSpots.part
		End

		Event
			At FootL
			Type Local
			PMagnet Head
			Part :SpeedSpots.part
		End

		Event
			At Root
			Type Local
			EName LevitationForce
			BhvrOverride
				physForceType Up
				physForceRadius 8
				physForcePower 200
				physForcePowerJitter 15
				PhysForceSpeedScaleMax 60
			End
		End

		Event
			At Hips
			Type Local
			EName WakeForce
			BhvrOverride
				physForceType In
				physForceRadius 10
				physForcePower 30
				physForcePowerJitter 10
				PhysForceSpeedScaleMax 120
			End
		End

		Event
			At Root
			Type Local
			EName WakeForce2
			BhvrOverride
				physForceType In
				physForceRadius 5
				physForcePower 50
				physForcePowerJitter 10
				PhysForceSpeedScaleMax 80
			End
		End
	End

###########################################################

End
