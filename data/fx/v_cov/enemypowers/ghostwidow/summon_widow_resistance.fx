#########################################################

FxInfo

#########################################################

Condition
	On	Time
	Time	0

	Event
		EName 	Explode
		Type	Start
		At	Root
		Part	:soulFlash2.part
		Part	:soulExplode2.part
		Part	:soulSparks1.part
		Part	:soulCloud1.part
		Part	POWERS\DarknessControl\DarkBlastPuddleExplosion.Part
		Sound dirtexplo3 80 80 .75
		Lifespan	5
	End

	Event
		EName 	Mists
		Type	start
		At	Root
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part	:SummonMistLight.part
		Part	:SummonMistDark.part
		LifeSpan	120
	End

End


Condition
	On	Death

	Event
		EName 	Explode
		Type	Start
		At	Root
		Part	:soulFlash2.part
		Part	:soulExplode2.part
		Part	:soulSparks1.part
		Part	:soulCloud1.part
		Part	POWERS\DarknessControl\DarkBlastPuddleExplosion.Part
		Sound dirtexplo3 80 80 .75
		Lifespan	5
	End

	Event
		EName 	Mists
		Type	start
		At	Root
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part	:SummonMistLight.part
		Part	:SummonMistDark.part
		LifeSpan	120
	End

End

#########################################################

End