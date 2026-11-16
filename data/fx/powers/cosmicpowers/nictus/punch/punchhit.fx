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
		Part	:HeavyBlastFlashTendrilsOutBlue.part
		Part	:HeavyBlastFlashTendrilsOutPurple.part
		Part	:HeavyPunchHitSparks.part
		Part	:HeavyPeaceBlastHitSpinners.part
		Part	:HeavyPeaceBlastHitSpinnersBlue.part
		Part	:HeavyPeaceBlastHitSpinnersPurple.part
		Part	:HeavyBlueHitTendrils.part
		Part	:HeavyBLKHitTendrils.part
		Part	:HeavyPurpleHitTendrils.part
		Sound peacehit2 70 70 .85
		LifeSpan 5
	End


End

End		


			