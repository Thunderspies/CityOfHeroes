#
FxInfo


###########################################################

	Condition
		On Time
		Time 0

		Event
			At ULegR
			Type Local
			EName OffsetHipRight
			Bhvr Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0.2 0 0.3
			End
		End

		Event
			At ULegL
			Type Local
			EName OffsetHipLeft
			Bhvr Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset -0.2 0.0 0.3
			End
		End

		Event
			At Chest
			Type Local
			PMagnet OffsetHipRight
			Bhvr Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0.3 0 0
			End
			Part :SmokeyTendril_Sm.part
			Part :OuterSmoke.part
			Part :Boils.part
		End

		Event
			At Chest
			Type Local
			PMagnet OffsetHipLeft
			Bhvr Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset -0.3 0 0
			End
			Part :SmokeyTendril_Sm.part
			Part :OuterSmoke.part
			Part :Boils.part
		End

		Event
			At UArmL
			Type Local
			PMagnet LArmL
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :SmokeyTendril_Sm.part
			Part :OuterSmoke.part
			Part :Boils.part
		End

		Event
			At LArmL
			Type Local
			PMagnet WepL
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :SmokeyTendril_Sm.part
			Part :OuterSmoke.part
		End

		Event
			At UArmR
			Type Local
			PMagnet LArmR
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :SmokeyTendril_Sm.part
			Part :OuterSmoke.part
			Part :Boils.part
		End

		Event
			At LArmR
			Type Local
			PMagnet WepR
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :SmokeyTendril_Sm.part
			Part :OuterSmoke.part
		End

		Event
			At OffsetHipLeft
			Type Local
			PMagnet LLEGL
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :SmokeyTendril_Sm.part
			Part :OuterSmoke.part
		End

		Event
			At OffsetHipright
			Type Local
			PMagnet LlegR
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :SmokeyTendril_Sm.part
			Part :OuterSmoke.part
		End

		Event
			At LLegL
			Type Local
			PMagnet FootL
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :SmokeyTendril_Sm.part
			Part :OuterSmoke.part
			Part :Boils.part
		End

		Event
			At LLegR
			Type Local
			PMagnet FootR
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :SmokeyTendril_Sm.part
			Part :OuterSmoke.part
			Part :Boils.part
		End

		Event
			At footL
			Type Local
			PMagnet LLegL
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :Boils.part
		End

		Event
			At FootR
			Type Local
			PMagnet LLegR
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :Boils.part
		End

	End

End
