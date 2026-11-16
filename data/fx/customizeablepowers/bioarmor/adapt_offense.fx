#
FxInfo

	#LifeSpan 5400


###########################################################
Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	OffensiveAdaptation_01 100 100 .8
	End
End

	Condition
		On Time
		Time 0

		#Event
			#At Root
			#Type Local
			#EName 9a5
			#Sound regen8 80 80 0.56
			#LifeSpan 90
		#End

		Event
			At Chest
			Type Local
			PMagnet UArmL
			Bhvr Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0 0.3 0
			End
			Part :LiteSmoke.part
			Part :LiteSmoke2.part
			Part :SmokeyTendril.part
			LifeSpan 30
		End

		Event
			At Chest
			Type Local
			PMagnet UArmR
			Bhvr Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0 0.3 0
			End
			Part :LiteSmoke.part
			Part :LiteSmoke2.part
			Part :SmokeyTendril.part
			Part :SmokeyTendril_Big.part
			LifeSpan 30
		End

		Event
			At ULegR
			Type Local
			EName OffsetHipRight
			BhvrOverride
				PositionOffset 0.1 0 0
			End
		End

		Event
			At ULegL
			Type Local
			EName OffsetHipLeft
			BhvrOverride
				PositionOffset -0.1 0 0
			End
		End

		Event
			At Chest
			Type Local
			PMagnet OffsetHipLeft
			Bhvr Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset -0.3 0 0
			End
			Part :SmokeyTendril.part
			LifeSpan 30
		End

		Event
			At Chest
			Type Local
			PMagnet OffsetHipRight
			Bhvr Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0.3 0 0
			End
			Part :SmokeyTendril.part
			LifeSpan 30
		End
#
#		Event
#			At ULegL
#			Type Local
#			PMagnet LLEGL
#			Bhvr Behaviors/GenericParticleFade.bhvr
#			Part :SmokeyTendril.part
#			Part :SmokeyTendril_Big.part
#			Part :OuterSmoke.part
#			LifeSpan 90
#		End
#
#		Event
#			At ULegR
#			Type Local
#			PMagnet LlegR
#			Bhvr Behaviors/GenericParticleFade.bhvr
#			Part :SmokeyTendril.part
#			Part :SmokeyTendril_Big.part
#			Part :OuterSmoke.part
#			LifeSpan 90
#		End
#
#		Event
#			At LLegL
#			Type Local
#			PMagnet FootL
#			Bhvr Behaviors/GenericParticleFade.bhvr
#			Part :SmokeyTendril.part
#			Part :SmokeyTendril_Big.part
#			Part :OuterSmoke.part
#			LifeSpan 90
#		End
#
#		Event
#			At LLegR
#			Type Local
#			PMagnet FootR
#			Bhvr Behaviors/GenericParticleFade.bhvr
#			Part :SmokeyTendril.part
#			Part :SmokeyTendril_Big.part
#			Part :OuterSmoke.part
#			LifeSpan 90
#		End

		Event
			At UArmL
			Type Local
			PMagnet LArmL
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :SmokeyTendril.part
		#	Part :SmokeyTendril_Big.part
		#	Part :OuterSmoke.part
			#LifeSpan 90
		End

		Event
			At LArmL
			Type Local
			PMagnet WepL
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :SmokeyTendril.part
			#Part :SmokeyTendril_Big.part
			Part :OuterSmoke.part
			#LifeSpan 90
		End

		Event
			At UArmR
			Type Local
			PMagnet LArmR
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :SmokeyTendril.part
		#	Part :SmokeyTendril_Big.part
		#	Part :OuterSmoke.part
			#LifeSpan 90
		End

		Event
			At LArmR
			Type Local
			PMagnet WepR
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :SmokeyTendril.part
			#Part :SmokeyTendril_Big.part
			Part :OuterSmoke.part
			#LifeSpan 90
		End

		Event
			At WepR
			Type Local
			EName DrawIn
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part CustomizeablePowers\BioArmor\Defense_DrawIn_Streaks_SM.part
			Part CustomizeablePowers\BioArmor\Defense_Glow_SM.part
			Part CustomizeablePowers\BioArmor\Defense_DrawIn_Spray_SM.part
			LifeSpan 30
		End

		Event
			At WepR
			Type PositOnly
			EName DrawIn
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part CustomizeablePowers\BioArmor\Defense_Glow_BurstB_SM.part
			Part CustomizeablePowers\BioArmor\Defense_Glow_BurstA_SM.part
			Part CustomizeablePowers\BioArmor\Drips_Hand.part
			Pmagnet LarmR
		End

		Event
			At WepL
			Type Local
			EName DrawIn
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part CustomizeablePowers\BioArmor\Defense_DrawIn_Streaks_SM.part
			Part CustomizeablePowers\BioArmor\Defense_Glow_SM.part
			Part CustomizeablePowers\BioArmor\Defense_DrawIn_Spray_SM.part
			LifeSpan 30
		End

		Event
			At WepL
			Type PositOnly
			EName DrawIn
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part CustomizeablePowers\BioArmor\Defense_Glow_BurstB_SM.part
			Part CustomizeablePowers\BioArmor\Defense_Glow_BurstA_SM.part
			Part CustomizeablePowers\BioArmor\Drips_Hand.part
			Pmagnet LarmL
		End

		Event
			At LarmR
			Type PositOnly
			EName DrawIn
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :LiteSmoke5.part
			Pmagnet WepR
		End

		Event
			At LarmL
			Type PositOnly
			EName DrawIn
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :LiteSmoke5.part
			Pmagnet WepL
		End

	End

End
