#
FxInfo

	LifeSpan 150


###########################################################
Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	SorceryPowerPool_ArcaneBolt_Hit_01 100 100 .8
	End
End

	Condition
		On Time
		Time 0

#		Event
#			At Origin
#			Type Local
#			Sound Incarnate_PyronicJudgement_Hit_01 100 100 0.8
#		End

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
			Part CustomizeablePowers\Brawling\Head_Shockwave_Small.part
			Part CustomizeablePowers\Brawling\Hit_Shockwave.part
			Part CustomizeablePowers\Brawling\Hit_Smoke_Additive.part
			Part CustomizeablePowers\Brawling\Hit_Smoke_Lingering.part
			Part CustomizeablePowers\Brawling\Head_Spikes.part
			Part :Tintable_AreaFireB_Mult.part
			Part :Tintable_AreaFire_Mult.part
			Part :Enchant_Rune_TwinkleStars.part
			Part :Enchant_Rune_TwinkleStars2.part
			Part CustomizeablePowers\Brawling\Hit_Flash_Big.part
			LifeSpan 3
		End

		Event
			At Origin
			Type StartPositOnly
			EName FireBallHit
			#Part :Smoke.part
		End

	End

End
