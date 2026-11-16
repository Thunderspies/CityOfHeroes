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
		#Part	:HeavyPunchHitSparks.part
		Part	:HeavyPeaceBlastHitSpinners.part
		Part	:HeavyPeaceBlastHitSpinnersPurple.part
		Part	:HeavyPeaceBlastHitSpinnersBlue.part
		Part	:HeavyBlueHitTendrils.part
		Part	:HeavyBLKHitTendrils.part
		Part	:HeavyPurpleHitTendrils.part
		Sound PeaceHeavyHit3 70 70 .8
		LifeSpan 5
	End


End

End	


			