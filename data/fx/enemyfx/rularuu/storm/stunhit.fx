#########################################################
##	EBlast
########################################################
FxInfo

Input  
	Inpname	Chest
End

#LifeSpan	40

#################################################################
##Hands
#################################################################

Condition
	Event
		Type	Local 
		At	Chest
		Sound stormhit_loop 70 70 1.0
		bhvr	behaviors/soundFade3.bhvr
		Flags	PowerLoopingSound
		LifeSpan 280
	End
End


Condition
	On	Time
	Time	0

	Event
		EName	continuingArchs
		Type	Local
		At	Chest
		part	:Storm_LightningHit_01Large.part
		part	:Storm_LightningHit_02Large.part
		part	:Stun_ThinRING.part
		Part	:Storm_Core.part
		#Sound	lightningbolt4 70 50 .8
		
	End
End



End
