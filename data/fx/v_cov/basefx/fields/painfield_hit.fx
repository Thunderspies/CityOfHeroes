#########################################################
##	CoT Red Crystal Damage Debuff
##
FxInfo

Input  
	InpName	Root
End

LifeSpan	15
#####################################################################################


Condition
	On 	Time
	Time 	0
		
	Event
		EName	Prime
		Type	Local
		At	Chest
		Bhvr	behaviors/genericParticleFade.bhvr
		BhvrOverride
			Scale .5 .5 .5
		End
		part4	:PainFieldGlow1.part
		LifeSpan	10
	End
			
	Event
		EName	Prime
		Type	Local
		At	Neck
		Bhvr	behaviors/genericParticleFade.bhvr
		BhvrOverride
			Scale .2 .5 .2
		End
		part4	:PainFieldGlow1.part
		LifeSpan	10
	End
		
	Event
		EName	Prime
		Type	Local
		At	Hips
		Bhvr	behaviors/genericParticleFade.bhvr
		BhvrOverride
			Scale .25 .5 .25
		End
		part3	:PainFieldSparkles.part
		part4	:PainFieldGlow1.part
		LifeSpan	10
	End

	Event
		EName	Prime
		Type	Local
		At	root
		Bhvr	behaviors/genericParticleFade.bhvr
		BhvrOverride
			PositionOffset 0 4 0
		End
		part3	:PainFieldFlash.part
		LifeSpan		5
	End

#	Event
#		EName	ring
#		Type	start
#		At	Root
#		geom	MegaRing2
#		bhvr	:BlastRingScale2a.bhvr
#		LifeSpan		2
#	End
#
	Event
		EName	sphere
		Type	Local
		At	Hips
		geom	CosmicPeaceBringerShield #MegaRing2
		bhvr	:BlastRingScale3.bhvr
		LifeSpan		9
	End

	Event
		EName	sphere2
		Type	Local
		At	Hips
		geom	CosmicPeaceBringerShield #MegaRing2
		bhvr	:BlastRingScale3.bhvr
		LifeSpan		9
	End

End



End	


			