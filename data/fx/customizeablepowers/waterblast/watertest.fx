#
FxInfo

	LifeSpan 200


###########################################################

	Condition
		On Time
		Time 8

		Event
			At WepL
			Type Local
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :WaterShot_HandGlow.part
			Part :WaterShot_DrawIn_Spray2.part
			Part :WaterShot_GlowStreaks.part
			LifeSpan 30
		End

		Event
			At WepR
			Type Local
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :WaterShot_HandGlow.part
			Part :WaterShot_DrawIn_Spray2.part
			Part :WaterShot_GlowStreaks.part
			LifeSpan 30
		End

	End

	Condition
		On Time
		Time 30

		Event
			At LarmR
			Type Local
			EName Spike
			Bhvr behaviors/scale1a.bhvr
			BhvrOverride
				PyrRotate 90 -90 0
				PositionOffset 2 0 0
			End
			Part :WaterShot_HandGlow.part
			#Sound Breathattack 80.0 80.0 0.8
			LifeSpan 55
		End

		Event
			At WepR
			Type Local
			EName Spike
			Part :WaterShot_HandGlow.part
			LifeSpan 55
		End

		Event
			At Spike
			Type Local
			EName PushForce
			BhvrOverride
				PositionOffset 0 0 10
				PyrRotate 15 0 0
				physForceType Forward
				physForceRadius 15
				physForcePower 80
				physForcePowerJitter 10
			End
			LifeSpan 30
		End

		Event
			At Spike
			Type Local
			EName Prime
			Part :Steam_Streaks.part
			Lifespan 25
		End

		Event
			At WepL
			Type Local
			EName SpikeL
			Bhvr behaviors/scale1a.bhvr
			BhvrOverride
				PyrRotate 0 -90 0
			End
			Part :WaterShot_HandGlow.part
			Geom LocalPivot01
			LifeSpan 55
		End

		Event
			At SpikeL
			Type Local
			EName PushForceL
			BhvrOverride
				PositionOffset 0 0 10
				PyrRotate 15 0 0
				physForceType Forward
				physForceRadius 15
				physForcePower 80
				physForcePowerJitter 10
			End
			LifeSpan 30
		End

		Event
			AltPiv 1
			At SpikeL
			Type Local
			EName PrimeL
			#Part POWERS\ColdControl\FrostBreathIce.part
			#Part POWERS\ColdControl\FrostBreathSnow.part
			#Part POWERS\ColdControl\FrostBreathMist.part
		End

	End

End
