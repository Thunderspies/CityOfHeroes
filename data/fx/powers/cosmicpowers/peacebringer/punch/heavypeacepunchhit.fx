#########################################################
##	Fire_Ball
##
FxInfo


LifeSpan	30

Condition
	On 	Time
	Time 	0

	Event
		EName	SwordFlash2
		Type	Local
		At	Chest
		BHVR	Behaviors/GenericParticleFade.bhvr
		Part	:HeavyBlastFlashOut.part
		Part	:HeavyBlastFlashTendrilsOut.part
		Part	:HeavyPunchHitSparks.part
		Part	:HeavyPeaceBlastHitSpinners.part
		Part	:HeavyBlueHitTendrils.part
		Sound PeaceHeavyHit3 80 80 1.0
		LifeSpan 5
	End


End

End	


			