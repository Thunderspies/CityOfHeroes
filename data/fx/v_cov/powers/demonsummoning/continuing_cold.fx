#############################################################
## IceHitContinuing.fx
#############################################################

FxInfo

#############################################################

Input
	Inpname	Hips
End

#############################################################

Condition
	On	Time
	Time	0


	Event
		Ename   Ice
		Type	local
		At	chest
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Continuing_IceChunks.part
		Part	:Continuing_IceChunks_Large.part
		LifeSpan 53
	End

	Event
		Ename   IceShards
		Type	local
		At	chest
		Part	:Continuing_Mist.part
		Part	:Continuing_Snow.part
	End
End

#############################################################

End