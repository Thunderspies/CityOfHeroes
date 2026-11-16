###########################################################

FxInfo

###########################################################

Condition
	On Time
	Time 0

		Event
			At Root
			Type Start
			EName Explode
			Part :soulFlash2.part
			Part :soulExplode2.part
			Part :soulSparks1.part
			Part :soulCloud1.part
			Part :DarkBlastPuddleExplosion.Part
			Sound dirtexplo3 80 80 0.75
			LifeSpan 5
		End

		Event
			At Root
			Type Start
			EName Mists
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :SummonMistLight.part
			Part :SummonMistDark.part
			LifeSpan 120
		End

	End

Condition
	On DEATH

		Event
			At Root
			Type Start
			EName Explode
			Part :soulFlash2.part
			Part :soulExplode2.part
			Part :soulSparks1.part
			Part :soulCloud1.part
			Part :DarkBlastPuddleExplosion.Part
			Sound dirtexplo3 80 80 0.75
			LifeSpan 5
		End

		Event
			At Root
			Type Start
			EName Mists
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :SummonMistLight.part
			Part :SummonMistDark.part
			LifeSpan 120
		End

	End

###########################################################

End
