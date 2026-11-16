#
FxInfo

	LifeSpan 250


###########################################################
Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	ParasiticAura_01 100 100 .8
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
			LifeSpan 45
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
			LifeSpan 90
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
			LifeSpan 90
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
			LifeSpan 90
		End

		Event
			At UArmL
			Type Local
			PMagnet LArmL
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :SmokeyTendril.part
			Part :SmokeyTendril_Big.part
			Part :OuterSmoke.part
			LifeSpan 90
		End

		Event
			At LArmL
			Type Local
			PMagnet WepL
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :SmokeyTendril.part
			Part :SmokeyTendril_Big.part
			Part :OuterSmoke.part
			LifeSpan 90
		End

		Event
			At UArmR
			Type Local
			PMagnet LArmR
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :SmokeyTendril.part
			Part :SmokeyTendril_Big.part
			Part :OuterSmoke.part
			LifeSpan 90
		End

		Event
			At LArmR
			Type Local
			PMagnet WepR
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :SmokeyTendril.part
			Part :SmokeyTendril_Big.part
			Part :OuterSmoke.part
			LifeSpan 90
		End

		Event
			At ULegL
			Type Local
			PMagnet LLEGL
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :SmokeyTendril.part
			Part :SmokeyTendril_Big.part
			Part :OuterSmoke.part
			LifeSpan 90
		End

		Event
			At ULegR
			Type Local
			PMagnet LlegR
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :SmokeyTendril.part
			Part :SmokeyTendril_Big.part
			Part :OuterSmoke.part
			LifeSpan 90
		End

		Event
			At LLegL
			Type Local
			PMagnet FootL
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :SmokeyTendril.part
			Part :SmokeyTendril_Big.part
			Part :OuterSmoke.part
			LifeSpan 90
		End

		Event
			At LLegR
			Type Local
			PMagnet FootR
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :SmokeyTendril.part
			Part :SmokeyTendril_Big.part
			Part :OuterSmoke.part
			LifeSpan 90
		End

	End

	Condition
		On Time
		Time 25

		Event
			At Chest
			Type Local
			EName RINGS
			Part CustomizeablePowers\BioArmor\Power_Glow.part
			Part CustomizeablePowers\BioArmor\Power_Ring.part
			LifeSpan 45
		End

	End

	Condition
		On Time
		Time 25

		Event
			At Chest
			Type Local
			ChildFX	:Parasitic_ChestShell.fx
			#LifeSpan 45
		End

	End

End
