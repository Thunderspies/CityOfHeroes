#########################################################
##	Fire_Ball
##
FxInfo

LifeSpan	200

Condition
	On 	Time
	Time 	0
	Event
		EName	SwordFlash2
		Type	Local
		At	Chest
		
		Part	:NictusSunFlareHitStreaks.part
		Part	:BlastFlash.part
		Part	:QuickBlastHitStreaks.part
		Part	:QuickBlastHitStreaksBlue.part
		Part	:QuickBlastHitStreaksPurple.part
		Part	:PeaceBlastHitSparks.part
		Part	:BlastFlashTendrils.part
		Part	:BlastFlashTendrilsPurple.part
		Part	:BlastFlashTendrilsBlue.part

		LifeSpan 10
	End

	########################spinningParticles1################

	Event
		ENAME	node
		Type	Local
		At	chest
		#Geom	RootToChestadjustment
		BHVR	:HitSpinFaster.bhvr
		LifeSpan 13
	End

	Event
		Type	Local
		At	node
		#AltPiv	1
		BHVR	:HitSpinFasterOpposite.bhvr

		Part	:Spinner.part
		LifeSpan 13
	End

########################spinningParticles2################

	Event
		Type	Local
		At	node
		#AltPiv	1
		BHVR	:HitSpinFaster.bhvr

		Part	:Spinner.part
		LifeSpan 13
	End

########################spinningParticles3################

	Event
		Type	Local
		At	node
		#AltPiv	1
		BHVR	:HitSpinFaster2Opposite.bhvr

		Part	:Spinner.part
		LifeSpan 13
	End

End


End	


			