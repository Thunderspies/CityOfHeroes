#                                                          
FxInfo

	LifeSpan 150


###########################################################

	Condition
		On Time
		Time 0

		#Event
			#At Origin
			#Type Local
			#Sound Incarnate_PyronicJudgement_Hit_01 100 100 0.8
		#End

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
			Bhvr behaviors/CameraShake02.bhvr
			Part ENEMYFX\Praetorian\SpiritStalkers\Judgment_Pyronic_Flash3.part
			Part ENEMYFX\Praetorian\SpiritStalkers\Judgment_Pyronic_Flash.part
			Part ENEMYFX\Praetorian\SpiritStalkers\Judgment_Pyronic_Flash2.part
			Part :Head_Shockwave_Small.part
			Part :Hit_Shockwave.part
			Part :Hit_Smoke_Additive.part
			Part :Hit_Smoke_Lingering.part
			Part :Head_Spikes.part
			Part CustomizablePowers_Pools\Sorcery\Subtractive\Tintable_AreaFireB_Mult.part
			Part CustomizablePowers_Pools\Sorcery\Subtractive\Tintable_AreaFire_Mult.part
			Part CustomizablePowers_Pools\Sorcery\Subtractive\Enchant_Rune_TwinkleStars.part
			Part CustomizablePowers_Pools\Sorcery\Subtractive\Enchant_Rune_TwinkleStars2.part
			Part :Hit_Flash_Big.part
			LifeSpan 3
		End

		Event
			At Origin
			Type StartPositOnly
			EName FireBallHit
			#Part CustomizablePowers_Pools\Sorcery\Subtractive\Smoke.part
		End

	End

End
