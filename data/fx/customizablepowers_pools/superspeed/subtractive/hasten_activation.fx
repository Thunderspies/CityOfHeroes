#
FxInfo


###########################################################

	Condition
		On TriggerBits
		TriggerBits NONHUMAN

		Event
			Type Destroy
			EName ALL
		End

	End

	Condition
		On Time
		Time 1

		Event
			At Emblem
			Type Posit
			EName Bubblies
			Bhvr Behaviors\GenericParticleFade.bhvr
			Part :AccelerateCombatBubbles01.part
			Part :AccelerateCombatFGlow01.part
			Part :AccelerateCombatFGlow02.part
			LifeSpan 55
		End

	End

	Condition
		On Time
		Time 10

		Event
			At Neck
			Type Posit
			EName Bubblies
			PMagnet Head
			Bhvr Behaviors\GenericParticleFade.bhvr
			Part :AccelerateCombatBubbles01.part
			Part :AccelerateCombatGlow01.part
			Part :AccelerateCombatGlow02.part
			Part :AccelerateCombatGlowBig01.part
			Sound hasten2 60 60 0.75
			Sound hasten2 60 60 0.75
			Sound hasten2 60 60 0.75
			Sound hasten2 60 60 0.75
			Sound hasten2 60 60 0.75
			Sound hasten2 60 60 0.75
			Sound hasten2 60 60 0.75
			Sound hasten2 60 60 0.75
			Sound hasten2 60 60 0.75
			Sound hasten2 60 60 0.75
			Sound hasten2 60 60 0.75
			Sound hasten2 60 60 0.75
			Sound hasten2 60 60 0.75
			Sound hasten2 60 60 0.75
			Sound hasten2 60 60 0.75
			Sound hasten2 60 60 0.75
			LifeSpan 30
		End

	End

	Condition
		On Time
		Time 20

		Event
			At Eyes
			Type Posit
			EName Bubblies
			Bhvr Behaviors\GenericParticleFade.bhvr
			Part :AccelerateCombatBubbles01.part
			Part :AccelerateCombatFGlow01.part
			Part :AccelerateCombatFGlow02.part
			LifeSpan 30
		End

	End

	Condition
		On Time
		Time 25

		Event
			At UArmR
			Type Posit
			EName Bubblies
			PMagnet Head
			Bhvr Behaviors\GenericParticleFade.bhvr
			Part :AccelerateCombatBubbles01.part
			Part :AccelerateCombatFGlow01.part
			Part :AccelerateCombatFGlow02.part
			Part :AccelerateCombatLinkerGlow01.part
			LifeSpan 15
		End

		Event
			At UArmL
			Type Posit
			EName Bubblies
			PMagnet Head
			Bhvr Behaviors\GenericParticleFade.bhvr
			Part :AccelerateCombatBubbles01.part
			Part :AccelerateCombatFGlow01.part
			Part :AccelerateCombatFGlow02.part
			Part :AccelerateCombatLinkerGlow01.part
			LifeSpan 15
		End

	End

	Condition
		On Time
		Time 30

		Event
			At LArmR
			Type Posit
			EName Bubblies
			PMagnet UarmR
			Bhvr Behaviors\GenericParticleFade.bhvr
			Part :AccelerateCombatBubbles01.part
			Part :AccelerateCombatFGlow01.part
			Part :AccelerateCombatFGlow02.part
			Part :AccelerateCombatLinkerGlow01.part
			LifeSpan 15
		End

		Event
			At LArmL
			Type Posit
			EName Bubblies
			PMagnet UarmL
			Bhvr Behaviors\GenericParticleFade.bhvr
			Part :AccelerateCombatBubbles01.part
			Part :AccelerateCombatFGlow01.part
			Part :AccelerateCombatFGlow02.part
			Part :AccelerateCombatLinkerGlow01.part
			LifeSpan 15
		End

	End

	Condition
		On Time
		Time 35

		Event
			At HandR
			Type Posit
			EName Bubblies
			PMagnet LarmR
			Bhvr Behaviors\GenericParticleFade.bhvr
			Part :AccelerateCombatBubbles01.part
			Part :AccelerateCombatFGlow01.part
			Part :AccelerateCombatFGlow02.part
			Part :AccelerateCombatLinkerGlow01.part
		End

		Event
			At HandL
			Type Posit
			EName Bubblies
			PMagnet LarmL
			Bhvr Behaviors\GenericParticleFade.bhvr
			Part :AccelerateCombatBubbles01.part
			Part :AccelerateCombatFGlow01.part
			Part :AccelerateCombatFGlow02.part
			Part :AccelerateCombatLinkerGlow01.part
		End

	End

End
