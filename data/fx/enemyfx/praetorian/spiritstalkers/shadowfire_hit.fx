#
FxInfo

	Input
		InpName Hips
	End

	LifeSpan 200


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Origin
			Type Local
			Sound ShadowHound_Shadowfire_Hit 100 100 0.8
		End

		Event
			At Origin
			Type Start
			EName ExplosiveForce
			BhvrOverride
				physForcePower 500
				physForcePowerJitter 100
				physForceRadius 30
				physForceType Out
			End
			LifeSpan 1
		End

		Event
			At Origin
			Type StartPositOnly
			EName FireBallHit
			#Bhvr behaviors/CameraShake02.bhvr
			Part ENEMYFX\Praetorian\SpiritStalkers\Judgment_Pyronic_Flash3.part
			Part :Judgment_Pyronic_Flash.part
			Part :Judgment_Pyronic_Flash2.part
			Part :Judgment_Pyronic_AreaFire_Mult.part
			Part :Judgment_Pyronic_AreaFire.part
			Part :Judgment_Pyronic_AreaAmber.part
			Part :Judgment_Pyronic_AreaAshes.part
			LifeSpan 3
		End

	End


	Condition
		On Time
		Time 4

		Event
			At Origin
			Type StartPositOnly
			EName FireBallHit
			Part :Judgment_Pyronic_Smoke.part
		End

	End

End
