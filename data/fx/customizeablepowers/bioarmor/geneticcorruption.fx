#
FxInfo

	Flags InheritAlpha InheritGeoScale


###########################################################

Condition
	On 	Time
	Time	4
	Event
		Type	Local
		At	Root
		Sound 	EvolvingAmor_01 100 100 .8
	End
End

	Condition
		On TriggerBits
		TriggerBits MALE
		DoMany 1

		Event
			EName MaleAura
			At Root
			Type Local
			Flags	OneAtATime
			ChildFx POWERS\BioArmor\BioArmor_03_Male_CHILD.fx
			While MALE
		End

	End

	Condition
		On TriggerBits
		TriggerBits FEMALE
		DoMany 1

		Event
			EName FemaleAura
			At Root
			Type Local
			Flags	OneAtATime
			ChildFx POWERS\BioArmor\BioArmor_03_Fem_CHILD.fx
			While FEMALE
		End

	End

	Condition
		On TriggerBits
		TriggerBits HUGE
		DoMany 1

		Event
			EName HugeAura
			At Root
			Type Local
			Flags	OneAtATime
			ChildFx POWERS\BioArmor\BioArmor_03_Huge_CHILD.fx
			While HUGE
		End

	End


##################################  CHARGEUP
Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	GeneticContamination_01 100 100 .8
	End
End

	Condition
		On Time
		Time 0

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

		Event
			At UArmL
			Type Local
			PMagnet LArmL
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :SmokeyTendril.part
			Part :SmokeyTendril_Big.part
			Part :OuterSmoke.part
			LifeSpan 30
		End

		Event
			At LArmL
			Type Local
			PMagnet WepL
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :SmokeyTendril.part
			Part :SmokeyTendril_Big.part
			Part :OuterSmoke.part
			LifeSpan 30
		End

		Event
			At UArmR
			Type Local
			PMagnet LArmR
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :SmokeyTendril.part
			Part :SmokeyTendril_Big.part
			Part :OuterSmoke.part
			LifeSpan 30
		End

		Event
			At LArmR
			Type Local
			PMagnet WepR
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :SmokeyTendril.part
			Part :SmokeyTendril_Big.part
			Part :OuterSmoke.part
			LifeSpan 30
		End

		Event
			At ULegL
			Type Local
			PMagnet LLEGL
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :SmokeyTendril.part
			Part :SmokeyTendril_Big.part
			Part :OuterSmoke.part
			LifeSpan 30
		End

		Event
			At ULegR
			Type Local
			PMagnet LlegR
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :SmokeyTendril.part
			Part :SmokeyTendril_Big.part
			Part :OuterSmoke.part
			LifeSpan 30
		End

		Event
			At LLegL
			Type Local
			PMagnet FootL
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :SmokeyTendril.part
			Part :SmokeyTendril_Big.part
			Part :OuterSmoke.part
			LifeSpan 30
		End

		Event
			At LLegR
			Type Local
			PMagnet FootR
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :SmokeyTendril.part
			Part :SmokeyTendril_Big.part
			Part :OuterSmoke.part
			LifeSpan 30
		End

		Event
			At Root
			Type Local
			EName RingNode
			Bhvr behaviors/genericparticlefade.bhvr
			BhvrOverride
				PositionOffset 0 2 0
			End
		End

	End


##########################################  DUST RING


	Condition
		On Time
		Time 19

		Event
			At RingNode
			Type Local
			EName FogRing1
			Bhvr behaviors/genericparticlefade.bhvr
			Part CustomizeablePowers\BioArmor\Genetic_RingSmall.part
		End

		Event
			At RingNode
			Type Local
			EName SwirlForce
			BhvrOverride
				physForceType CWSwirl
				physForceRadius 30
				physForceCentripetal .6
				physForcePower 50
				physForcePowerJitter 5
			End
		End

		Event
			At Root
			Type Local
			EName LevitationForce
			BhvrOverride
				physForceType Up
				physForceRadius 16
				physForcePower 45
				physForcePowerJitter 5
			End
		End

		Event
			At RingNode
			Type Local
			EName LevitationForce
			BhvrOverride
				PositionOffset 0 6 0
				physForceType Up
				physForceRadius 10
				physForcePower -15
				physForcePowerJitter 5
			End
		End

		Event
			At RingNode
			Type Local
			EName Hurricane
			Bhvr behaviors/genericparticlefade.bhvr
			Part CustomizeablePowers\BioArmor\Genetic_Ring02.part
		End

	End

	Condition
		On Time
		Time 40

		Event
			At RingNode
			Type Local
			EName RadiusFxScale
		End

	End

########################################  PARTICLE BURST

	Condition
		On Cycle
		Time 40

		Event
			At RadiusFxScale
			Type Local
			EName Prime1
			Bhvr :GeneticBubble.bhvr
			Part CustomizeablePowers\BioArmor\Genetic_RingStreaks.part
			Part CustomizeablePowers\BioArmor\Genetic_Dust.part
			LifeSpan 50
		End

	End

End
