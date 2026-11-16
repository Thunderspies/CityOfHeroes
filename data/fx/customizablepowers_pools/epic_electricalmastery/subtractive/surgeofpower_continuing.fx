#                                                          
FxInfo

	Flags DontSuppress


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Origin
			Type Local
			Sound electransform2 70 70 0.88
		End

	End

	Condition
		On Time
		Time 28

		Event
			At Origin
			Type Local
			Flags PowerLoopingSound
			Bhvr behaviors\soundFade3.bhvr
			Sound elecarmor5_loop 70 70 0.4
			LifeSpan 350
		End

	End

	Condition
		On Time
		Time 0

		Event
			At Hair
			Type Local
			BhvrOverride
				PositionOffset .15 .2 0
			End
			Part :BluePupil.part
			Part :WhitePupil.part
		End

		Event
			At Hair
			Type Local
			BhvrOverride
				PositionOffset -.15 .2 0
			End
			Part :BluePupil.part
			Part :WhitePupil.part
		End

		Event
			At Hair
			Type Local
			BhvrOverride
				PositionOffset -.11 -.1 0
			End
			Part :CreepyMouth.part
		End

	End

	Condition
		On Time
		Time 0

		Event
			At Head
			Type Local
			Bhvr Behaviors\GenericParticleFade.bhvr
			Part :ElectricityIonHaze.part
		End

		Event
			At SpadR
			Type Local
			Bhvr Behaviors\GenericParticleFade.bhvr
			Part :ElectricityIonHaze.part
			Part :Blue_ElectricityTendrilsSubtle.part
		End

		Event
			At SpadL
			Type Local
			Bhvr Behaviors\GenericParticleFade.bhvr
			Part :ElectricityIonHaze.part
			Part :Blue_ElectricityTendrilsSubtle.part
		End

		Event
			At LArmR
			Type Local
			Bhvr Behaviors\GenericParticleFade.bhvr
			Part :ElectricityIonHaze.part
		End

		Event
			At LArmL
			Type Local
			Bhvr Behaviors\GenericParticleFade.bhvr
			Part :ElectricityIonHaze.part
		End

		Event
			At ULegR
			Type Local
			Bhvr Behaviors\GenericParticleFade.bhvr
			Part :ElectricityIonHaze.part
			Part :Blue_ElectricityTendrilsSubtle.part
		End

		Event
			At ULegL
			Type Local
			Bhvr Behaviors\GenericParticleFade.bhvr
			Part :ElectricityIonHaze.part
			Part :Blue_ElectricityTendrilsSubtle.part
		End

		Event
			At LLegR
			Type Local
			Bhvr Behaviors\GenericParticleFade.bhvr
			Part :ElectricityIonHaze.part
			Part :Blue_ElectricityTendrils.part
		End

		Event
			At LLegL
			Type Local
			Bhvr Behaviors\GenericParticleFade.bhvr
			Part :ElectricityIonHaze.part
			Part :Blue_ElectricityTendrils.part
		End

		Event
			At FootR
			Type Local
			Bhvr Behaviors\GenericParticleFade.bhvr
			Part :ElectricityIonHaze.part
			Part :Blue_ElectricityTendrils.part
		End

		Event
			At FootL
			Type Local
			Bhvr Behaviors\GenericParticleFade.bhvr
			Part :ElectricityIonHaze.part
			Part :Blue_ElectricityTendrils.part
		End

	End

	Condition
		On Cycle
		Time 3
		Chance 0.7

		Event
			At Hair
			Type Local
			EName 3
			Bhvr Behaviors\GenericParticleFade.bhvr
			Part :Blue_ElectricityControl01white.part
			Part :Blue_ElectricityControl02.part
			Part :Blue_ElectricitySparkBigGlow.part
			Part :ElectricityArchContinuingz.part
			Part :ElectricityArchContinuing2z.part
			LifeSpan 3
		End

	End

	Condition
		On Cycle
		Time 4
		Chance 0.7

		Event
			At Chest
			Type Local
			EName 3
			Bhvr Behaviors\GenericParticleFade.bhvr
			Part :Blue_ElectricityControl01white.part
			Part :Blue_ElectricityControl02.part
			Part :Blue_ElectricitySparkBigGlow.part
			Part :ElectricityArchContinuingz.part
			Part :ElectricityArchContinuing2z.part
			LifeSpan 3
		End

	End

	Condition
		On Cycle
		Time 5
		Chance 0.7

		Event
			At Hips
			Type Local
			EName 3
			Bhvr Behaviors\GenericParticleFade.bhvr
			Part :Blue_ElectricityControl01white.part
			Part :Blue_ElectricityControl02.part
			Part :Blue_ElectricitySparkBigGlow.part
			Part :ElectricityArchContinuingz.part
			Part :ElectricityArchContinuing2z.part
			LifeSpan 3
		End

	End

	Condition
		On Cycle
		Time 6
		Chance 0.7

		Event
			At UArmL
			Type Local
			EName 3
			Bhvr Behaviors\GenericParticleFade.bhvr
			Part :Blue_ElectricityControl01white.part
			Part :Blue_ElectricityControl02.part
			Part :Blue_ElectricitySparkBigGlow.part
			Part :ElectricityArchContinuingz.part
			Part :ElectricityArchContinuing2z.part
			LifeSpan 3
		End

	End

	Condition
		On Cycle
		Time 6
		Chance 0.7

		Event
			At UArmR
			Type Local
			EName 3
			Bhvr Behaviors\GenericParticleFade.bhvr
			Part :Blue_ElectricityControl01white.part
			Part :Blue_ElectricityControl02.part
			Part :Blue_ElectricitySparkBigGlow.part
			Part :ElectricityArchContinuingz.part
			Part :ElectricityArchContinuing2z.part
			LifeSpan 3
		End

	End

	Condition
		On Cycle
		Time 6
		Chance 0.7

		Event
			At LArmL
			Type Local
			EName 3
			Bhvr Behaviors\GenericParticleFade.bhvr
			Part :Blue_ElectricityControl01white.part
			Part :Blue_ElectricityControl02.part
			Part :Blue_ElectricitySparkBigGlow.part
			Part :ElectricityArchContinuingz.part
			Part :ElectricityArchContinuing2z.part
			LifeSpan 3
		End

	End

	Condition
		On Cycle
		Time 6
		Chance 0.7

		Event
			At LArmR
			Type Local
			EName 3
			Bhvr Behaviors\GenericParticleFade.bhvr
			Part :Blue_ElectricityControl01white.part
			Part :Blue_ElectricityControl02.part
			Part :Blue_ElectricitySparkBigGlow.part
			Part :ElectricityArchContinuingz.part
			Part :ElectricityArchContinuing2z.part
			LifeSpan 3
		End

	End

	Condition
		On Cycle
		Time 6
		Chance 0.7

		Event
			At WepL
			Type Local
			EName 3
			Bhvr Behaviors\GenericParticleFade.bhvr
			Part :Blue_ElectricityControl01white.part
			Part :Blue_ElectricityControl02.part
			Part :Blue_ElectricitySparkBigGlow.part
			Part :ElectricityArchContinuingz.part
			Part :ElectricityArchContinuing2z.part
			LifeSpan 3
		End

	End

	Condition
		On Cycle
		Time 6
		Chance 0.7

		Event
			At WepR
			Type Local
			EName 3
			Bhvr Behaviors\GenericParticleFade.bhvr
			Part :Blue_ElectricityControl01white.part
			Part :Blue_ElectricityControl02.part
			Part :Blue_ElectricitySparkBigGlow.part
			Part :ElectricityArchContinuingz.part
			Part :ElectricityArchContinuing2z.part
			LifeSpan 3
		End

	End

	Condition
		On Cycle
		Time 6
		Chance 0.7

		Event
			At ULegL
			Type Local
			EName 3
			Bhvr Behaviors\GenericParticleFade.bhvr
			Part :Blue_ElectricityControl01white.part
			Part :Blue_ElectricityControl02.part
			Part :Blue_ElectricitySparkBigGlow.part
			Part :ElectricityArchContinuingz.part
			Part :ElectricityArchContinuing2z.part
			LifeSpan 3
		End

	End

	Condition
		On Cycle
		Time 6
		Chance 0.7

		Event
			At ULegR
			Type Local
			EName 3
			Bhvr Behaviors\GenericParticleFade.bhvr
			Part :Blue_ElectricityControl01white.part
			Part :Blue_ElectricityControl02.part
			Part :Blue_ElectricitySparkBigGlow.part
			Part :ElectricityArchContinuingz.part
			Part :ElectricityArchContinuing2z.part
			LifeSpan 3
		End

	End

	Condition
		On Cycle
		Time 6
		Chance 0.7

		Event
			At LLegL
			Type Local
			EName 3
			Bhvr Behaviors\GenericParticleFade.bhvr
			Part :Blue_ElectricityControl01white.part
			Part :Blue_ElectricityControl02.part
			Part :Blue_ElectricitySparkBigGlow.part
			Part :ElectricityArchContinuingz.part
			Part :ElectricityArchContinuing2z.part
			LifeSpan 3
		End

	End

	Condition
		On Cycle
		Time 6
		Chance 0.7

		Event
			At LLegR
			Type Local
			EName 3
			Bhvr Behaviors\GenericParticleFade.bhvr
			Part :Blue_ElectricityControl01white.part
			Part :Blue_ElectricityControl02.part
			Part :Blue_ElectricitySparkBigGlow.part
			Part :ElectricityArchContinuingz.part
			Part :ElectricityArchContinuing2z.part
			LifeSpan 3
		End

	End

	Condition
		On Cycle
		Time 6
		Chance 0.7

		Event
			At FootL
			Type Local
			EName 3
			Bhvr Behaviors\GenericParticleFade.bhvr
			Part :Blue_ElectricityControl01white.part
			Part :Blue_ElectricityControl02.part
			Part :Blue_ElectricitySparkBigGlow.part
			Part :ElectricityArchContinuingz.part
			Part :ElectricityArchContinuing2z.part
			LifeSpan 3
		End

	End

	Condition
		On Cycle
		Time 6
		Chance 0.7

		Event
			At FootR
			Type Local
			EName 3
			Bhvr Behaviors\GenericParticleFade.bhvr
			Part :Blue_ElectricityControl01white.part
			Part :Blue_ElectricityControl02.part
			Part :Blue_ElectricitySparkBigGlow.part
			Part :ElectricityArchContinuingz.part
			Part :ElectricityArchContinuing2z.part
			LifeSpan 3
		End

	End

	Condition
		On Cycle
		Time 3
		Chance 0.4

		Event
			At Neck
			Type Local
			EName 10
			PMagnet UArmL
			Bhvr Behaviors\GenericParticleFade.bhvr
			Part :Blue_ChargedBolt01.Part
			Part :Blue_ChargedBolt02.Part
			Part :Blue_ChargedBolt05.part
			Part :Blue_ChargedBolt06.part
			LifeSpan 2
		End

	End

	Condition
		On Cycle
		Time 3
		Chance 0.4

		Event
			At Neck
			Type Local
			EName 10
			PMagnet UArmR
			Bhvr Behaviors\GenericParticleFade.bhvr
			Part :Blue_ChargedBolt01.Part
			Part :Blue_ChargedBolt02.Part
			Part :Blue_ChargedBolt05.part
			Part :Blue_ChargedBolt06.part
			LifeSpan 2
		End

	End

	Condition
		On Cycle
		Time 3
		Chance 0.4

		Event
			At UArmR
			Type Local
			EName 10
			PMagnet Hips
			Bhvr Behaviors\GenericParticleFade.bhvr
			Part :Blue_ChargedBolt01.Part
			Part :Blue_ChargedBolt02.Part
			Part :Blue_ChargedBolt05.part
			Part :Blue_ChargedBolt06.part
			LifeSpan 2
		End

	End

	Condition
		On Cycle
		Time 3
		Chance 0.4

		Event
			At UArmL
			Type Local
			EName 10
			PMagnet Hips
			Bhvr Behaviors\GenericParticleFade.bhvr
			Part :Blue_ChargedBolt01.Part
			Part :Blue_ChargedBolt02.Part
			Part :Blue_ChargedBolt05.part
			Part :Blue_ChargedBolt06.part
			LifeSpan 2
		End

	End

	Condition
		On Cycle
		Time 3
		Chance 0.4

		Event
			At Neck
			Type Local
			EName 10
			PMagnet Hips
			Bhvr Behaviors\GenericParticleFade.bhvr
			Part :Blue_ChargedBolt01.Part
			Part :Blue_ChargedBolt02.Part
			Part :Blue_ChargedBolt05.part
			Part :Blue_ChargedBolt06.part
			LifeSpan 2
		End

	End

	Condition
		On Cycle
		Time 3
		Chance 0.4

		Event
			At UArmL
			Type Local
			EName 10
			PMagnet LArmL
			Bhvr Behaviors\GenericParticleFade.bhvr
			Part :Blue_ChargedBolt01.Part
			Part :Blue_ChargedBolt02.Part
			Part :Blue_ChargedBolt05.part
			Part :Blue_ChargedBolt06.part
			LifeSpan 2
		End

	End

	Condition
		On Cycle
		Time 3
		Chance 0.4

		Event
			At LArmL
			Type Local
			EName 11
			PMagnet WepL
			Bhvr Behaviors\GenericParticleFade.bhvr
			Part :Blue_ChargedBolt01.Part
			Part :Blue_ChargedBolt02.Part
			Part :Blue_ChargedBolt05.part
			Part :Blue_ChargedBolt06.part
			LifeSpan 2
		End

	End

	Condition
		On Cycle
		Time 3
		Chance 0.4

		Event
			At UArmR
			Type Local
			EName 13
			PMagnet LArmR
			Bhvr Behaviors\GenericParticleFade.bhvr
			Part :Blue_ChargedBolt01.Part
			Part :Blue_ChargedBolt02.Part
			Part :Blue_ChargedBolt05.part
			Part :Blue_ChargedBolt06.part
			LifeSpan 2
		End

	End

	Condition
		On Cycle
		Time 3
		Chance 0.4

		Event
			At LArmR
			Type Local
			EName 14
			PMagnet WepR
			Bhvr Behaviors\GenericParticleFade.bhvr
			Part :Blue_ChargedBolt01.Part
			Part :Blue_ChargedBolt02.Part
			Part :Blue_ChargedBolt05.part
			Part :Blue_ChargedBolt06.part
			LifeSpan 2
		End

	End

	Condition
		On Cycle
		Time 3
		Chance 0.4

		Event
			At ULegL
			Type Local
			EName 3
			PMagnet Chest
			Bhvr Behaviors\GenericParticleFade.bhvr
			Part :Blue_ChargedBolt01.Part
			Part :Blue_ChargedBolt02.Part
			Part :Blue_ChargedBolt05.part
			Part :Blue_ChargedBolt06.part
			LifeSpan 2
		End

	End

	Condition
		On Cycle
		Time 3
		Chance 0.4

		Event
			At ULegL
			Type Local
			EName 4
			PMagnet LLEGL
			Bhvr Behaviors\GenericParticleFade.bhvr
			Part :Blue_ChargedBolt01.Part
			Part :Blue_ChargedBolt02.Part
			Part :Blue_ChargedBolt05.part
			Part :Blue_ChargedBolt06.part
			LifeSpan 2
		End

	End

	Condition
		On Cycle
		Time 3
		Chance 0.4

		Event
			At LLegL
			Type Local
			EName 4
			PMagnet ULEGL
			Bhvr Behaviors\GenericParticleFade.bhvr
			Part :Blue_ChargedBolt01.Part
			Part :Blue_ChargedBolt02.Part
			Part :Blue_ChargedBolt05.part
			Part :Blue_ChargedBolt06.part
			LifeSpan 2
		End

	End

	Condition
		On Cycle
		Time 3
		Chance 0.4

		Event
			At LLegL
			Type Local
			EName 5
			PMagnet FootL
			Bhvr Behaviors\GenericParticleFade.bhvr
			Part :Blue_ChargedBolt01.Part
			Part :Blue_ChargedBolt02.Part
			Part :Blue_ChargedBolt05.part
			Part :Blue_ChargedBolt06.part
			LifeSpan 2
		End

	End

	Condition
		On Cycle
		Time 3
		Chance 0.4

		Event
			At ULegR
			Type Local
			EName 3
			PMagnet Chest
			Bhvr Behaviors\GenericParticleFade.bhvr
			Part :Blue_ChargedBolt01.Part
			Part :Blue_ChargedBolt02.Part
			Part :Blue_ChargedBolt05.part
			Part :Blue_ChargedBolt06.part
			LifeSpan 2
		End

	End

	Condition
		On Cycle
		Time 3
		Chance 0.4

		Event
			At LLegR
			Type Local
			EName 4
			PMagnet ULEGR
			Bhvr Behaviors\GenericParticleFade.bhvr
			Part :Red_ElectricitySparkBurst2.part
			Part :Blue_ChargedBolt01.Part
			Part :Blue_ChargedBolt02.Part
			Part :Blue_ChargedBolt05.part
			Part :Blue_ChargedBolt06.part
			LifeSpan 2
		End

	End

	Condition
		On Cycle
		Time 3
		Chance 0.4

		Event
			At ULegR
			Type Local
			EName 4
			PMagnet LLEGR
			Bhvr Behaviors\GenericParticleFade.bhvr
			Part :Blue_ChargedBolt01.Part
			Part :Blue_ChargedBolt02.Part
			Part :Blue_ChargedBolt05.part
			Part :Blue_ChargedBolt06.part
			LifeSpan 2
		End

	End

	Condition
		On Cycle
		Time 3
		Chance 0.4

		Event
			At LLegR
			Type Local
			EName 5
			PMagnet FootR
			Bhvr Behaviors\GenericParticleFade.bhvr
			Part :Blue_ChargedBolt01.Part
			Part :Blue_ChargedBolt02.Part
			Part :Blue_ChargedBolt05.part
			Part :Blue_ChargedBolt06.part
			LifeSpan 2
		End

	End

	Condition
		On Cycle
		Time 3
		Chance 0.4

		Event
			At FootL
			Type Local
			EName 5
			PMagnet LLEGL
			Bhvr Behaviors\GenericParticleFade.bhvr
			Part :Blue_ChargedBolt01.Part
			Part :Blue_ChargedBolt02.Part
			Part :Blue_ChargedBolt05.part
			Part :Blue_ChargedBolt06.part
			LifeSpan 2
		End

	End

	Condition
		On Cycle
		Time 3
		Chance 0.4

		Event
			At FootR
			Type Local
			EName 5
			PMagnet LLEGR
			Bhvr Behaviors\GenericParticleFade.bhvr
			Part :Blue_ChargedBolt01.Part
			Part :Blue_ChargedBolt02.Part
			Part :Blue_ChargedBolt05.part
			Part :Blue_ChargedBolt06.part
			LifeSpan 2
		End

	End

	Condition
		On Cycle
		Time 14
		Chance 0.6

		Event
			At Hair
			Type Local
			PMagnet Chest
			POther Chest
			#Bhvr Behaviors\TelportationFade.bhvr
			Part :Blue_BodyArcsSmall01.part
			Part :Blue_BodyArcsSmall02.part
			LifeSpan 10
		End

	End

	Condition
		On Cycle
		Time 11
		Chance 0.6

		Event
			At UArmR
			Type Local
			PMagnet LarmR
			POther LarmR
			#Bhvr Behaviors\TelportationFade.bhvr
			Part :Blue_BodyArcsSmall01.part
			Part :Blue_BodyArcsSmall02.part
			LifeSpan 10
		End

	End

	Condition
		On Cycle
		Time 16
		Chance 0.6

		Event
			At WepR
			Type Local
			PMagnet LarmR
			POther LarmR
			#Bhvr Behaviors\TelportationFade.bhvr
			Part :Blue_BodyArcsSmall01.part
			Part :Blue_BodyArcsSmall02.part
			LifeSpan 10
		End

	End

	Condition
		On Cycle
		Time 20
		Chance 0.6

		Event
			At UArmL
			Type Local
			PMagnet LarmL
			POther LarmL
			#Bhvr Behaviors\TelportationFade.bhvr
			Part :Blue_BodyArcsSmall01.part
			Part :Blue_BodyArcsSmall02.part
			LifeSpan 10
		End

	End

	Condition
		On Cycle
		Time 9
		Chance 0.6

		Event
			At WepL
			Type Local
			PMagnet LarmL
			POther LarmL
			#Bhvr Behaviors\TelportationFade.bhvr
			Part :Blue_BodyArcsSmall01.part
			Part :Blue_BodyArcsSmall02.part
			LifeSpan 10
		End

	End

	Condition
		On Cycle
		Time 17
		Chance 0.6

		Event
			At ULegL
			Type Local
			PMagnet Llegl
			POther Llegl
			#Bhvr Behaviors\TelportationFade.bhvr
			Part :Blue_BodyArcsSmall01.part
			Part :Blue_BodyArcsSmall02.part
			LifeSpan 10
		End

	End

	Condition
		On Cycle
		Time 8
		Chance 0.6

		Event
			At FootL
			Type Local
			PMagnet Llegl
			POther Llegl
			#Bhvr Behaviors\TelportationFade.bhvr
			Part :Blue_BodyArcsSmall01.part
			Part :Blue_BodyArcsSmall02.part
			LifeSpan 10
		End

	End

	Condition
		On Cycle
		Time 12
		Chance 0.6

		Event
			At ULegR
			Type Local
			PMagnet LlegR
			POther LlegR
			#Bhvr Behaviors\TelportationFade.bhvr
			Part :Blue_BodyArcsSmall01.part
			Part :Blue_BodyArcsSmall02.part
			LifeSpan 10
		End

	End

	Condition
		On Cycle
		Time 10
		Chance 0.6

		Event
			At FootR
			Type Local
			PMagnet LlegR
			POther LlegR
			#Bhvr Behaviors\TelportationFade.bhvr
			Part :Blue_BodyArcsSmall01.part
			Part :Blue_BodyArcsSmall02.part
			LifeSpan 10
		End

	End

End
