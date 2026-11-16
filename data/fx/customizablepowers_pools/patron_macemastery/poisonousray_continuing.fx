#
FxInfo

	Flags InheritAnimScale


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Chest
			Type Local
			Sound SpitHit1 70 70 0.8
		End

	End

	Condition
		On Time
		Time 0

		Event
			At Chest
			Type Local
			EName PoisonSplat
			PMagnet Hips
			Part :Tarantula_P_VBoltSplat01.part
			Part :Tarantula_P_VBoltSplatImpact01.part
			Part :Tarantula_P_VBoltSplatImpact.part
			LifeSpan 10
		End

	End

	Condition
		On Time
		Time 2

		Event
			At UArmL
			Type PositOnly
			EName Streaks
			POther UArmR
			Part :Tarantula_P_VBAcidStreak.part
		End

		Event
			At LArmR
			Type PositOnly
			EName Streaks01
			POther RingR
			Part :Tarantula_P_VBAcidStreakSM.part
		End

		Event
			At LArmL
			Type PositOnly
			EName Streaks02
			POther RingL
			Part :Tarantula_P_VBAcidStreakSM.part
		End

	End

	Condition
		On Time
		Time 5

		Event
			At Chest
			Type Local
			EName Smoke
			POther LLegL
			Part :Tarantula_P_VBAcid.part
			Part :Tarantula_P_VBAcidDark.part
			Part :Tarantula_P_VBAcidBubbles.part
			Part :Tarantula_P_VBAcidBubblesdark.part
			Part :Tarantula_P_VBAcidBubble.part
		End

		Event
			At Chest
			Type Local
			EName Smoke01
			POther LLegR
			Part :Tarantula_P_VBAcid.part
			Part :Tarantula_P_VBAcidDark.part
			Part :Tarantula_P_VBAcidBubbles.part
			Part :Tarantula_P_VBAcidBubblesdark.part
			Part :Tarantula_P_VBAcidBubble.part
		End

		Event
			At UArmL
			Type Local
			EName Smoke02
			POther UArmR
			Part :Tarantula_P_VBAcid.part
			Part :Tarantula_P_VBAcidDark.part
			Part :Tarantula_P_VBAcidBubbles.part
			Part :Tarantula_P_VBAcidBubblesdark.part
			Part :Tarantula_P_VBAcidBubble.part
		End

	End

End
