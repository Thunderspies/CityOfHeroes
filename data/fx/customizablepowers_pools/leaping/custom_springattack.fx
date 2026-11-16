#
FxInfo

	LifeSpan 500


###########################################################

	Condition
		On Time
		Time 5

		Event
			At Root
			Type Local
			EName Flash
			BhvrOverride
				PositionOffset 0 0.5 0
			End
			Part :Custom_SpringAttack_Flash.part
			Part :Custom_SpringAttack_Flash_LocalFacing.part
			Part :Custom_SpringAttack_Glow.part
			LifeSpan 3
		End

		Event
			At Root
			Type Start
			Sound SpringAttack_Jump_01 100 100 0.8
		End

	End

	Condition
		On Time
		Time 17.5

		Event
			At Head
			Type Local
			POther StartOffset_Head
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :Custom_SpringAttack_BodyStreak.part
			LifeSpan 1
		End

		Event
			At Chest
			Type Local
			POther StartOffset_Chest
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :Custom_SpringAttack_BodyStreak.part
			LifeSpan 1
		End

		Event
			At UArmL
			Type Local
			POther StartOffset_UArmL
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :Custom_SpringAttack_BodyStreak.part
			LifeSpan 1
		End

		Event
			At UArmR
			Type Local
			POther StartOffset_UArmR
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :Custom_SpringAttack_BodyStreak.part
			LifeSpan 1
		End

		Event
			At Hips
			Type Local
			POther StartOffset_Hips
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :Custom_SpringAttack_BodyStreak.part
			LifeSpan 1
		End

		Event
			At LLegL
			Type Local
			POther StartOffset_LLegL
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :Custom_SpringAttack_BodyStreak.part
			LifeSpan 1
		End

		Event
			At LLegR
			Type Local
			POther StartOffset_LLegR
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :Custom_SpringAttack_BodyStreak.part
			LifeSpan 1
		End

	End

	Condition
		On Time
		Time 20

		Event
			At Head
			Type Local
			POther StartOffset_Head
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :Custom_SpringAttack_StreakIn.part
			LifeSpan 1
		End

		Event
			At Chest
			Type Local
			POther StartOffset_Chest
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :Custom_SpringAttack_StreakIn.part
			LifeSpan 1
		End

		Event
			At UArmL
			Type Local
			POther StartOffset_UArmL
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :Custom_SpringAttack_StreakIn.part
			LifeSpan 1
		End

		Event
			At UArmR
			Type Local
			POther StartOffset_UArmR
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :Custom_SpringAttack_StreakIn.part
			LifeSpan 1
		End

		Event
			At Hips
			Type Local
			POther StartOffset_Hips
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :Custom_SpringAttack_StreakIn.part
			LifeSpan 1
		End

		Event
			At LLegL
			Type Local
			POther StartOffset_LLegL
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :Custom_SpringAttack_StreakIn.part
			LifeSpan 1
		End

		Event
			At LLegR
			Type Local
			POther StartOffset_LLegR
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :Custom_SpringAttack_StreakIn.part
			LifeSpan 1
		End

	End

	Condition
		On Time
		Time 24

		Event
			At Root
			Type Local
			Sound SpringAttack_Land_01 100 100 0.8
		End

	End

	Condition
		On Time
		Time 40

		Event
			At Root
			Type Start
			EName TeleportIn
			Bhvr behaviors/Braz/ExpandRingFast.bhvr
			BhvrOverride
				PositionOffset 0 1 0
			End
			Part :SpringAttack_DustShockwave.part
			LifeSpan 2
		End

	End

	Condition
		On Time
		Time 40

		Event
			At Root
			Type Start
			EName TeleportIn
			BhvrOverride
				PositionOffset 0 0.5 0
			End
			Part :Custom_SpringAttack_ImpactFlash.part
			Part :Custom_SpringAttack_ImpactFlash_Localfacing.part
			Part :Custom_SpringAttack_Glow2.part
			Part :Custom_SpringAttack_Glow2_localFacing.part
			LifeSpan 1
		End

		Event
			At Root
			Type Start
			EName DustUp
			Part :SpringAttack_DustPillar.part
			Part :SpringAttack_Rocks.part
			LifeSpan 5
		End

	End

	Condition
		On Time
		Time 40

		Event
			At Root
			Type Start
			EName GroundCrack
			Bhvr POWERS\SuperStrength\Splat_FootStomp.bhvr
			BhvrOverride
				PositionOffset 1.5 0 1
				StartColor 55 50 40
			End
			Splat Crater_Cracks2.tga
			LifeSpan 300
		End

	End

	#Condition
		#On Time
		#Time 50

		#Event
			#At StartOffset
			#Type Start
			#BhvrOverride
				#Shake 1.5
				#ShakeFallOff 0.075
				#ShakeRadius 128
			#End
			#LifeSpan 1
		#End

	#End

End
