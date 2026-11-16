#########################################################
##	Soul Storm
##
FxInfo


LifeSpan  120


################  Start   #####################


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
		Part	:soulCloud2.part
		LifeSpan	60
	End

End


Condition
	On	Time
	Time	100

	Event
		EName 	All 
		Type	Destroy
	End

End

End				