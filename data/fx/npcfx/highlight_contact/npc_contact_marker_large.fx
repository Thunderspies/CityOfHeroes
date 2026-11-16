#########################################################
##	template

FxInfo


Condition
	
	On	Time
	Time	00

	Event	
		ENAME	rings
		Type	Local
		At	Root
		Bhvr	Behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0 .1 5
		End
		part	:ring01Large.part
		part	:ring02Large.part
	
	End

	Event	
		ENAME	rings
		Type	Local
		At	Root
		Bhvr	Behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0 .1 5
		End
		part	:glow01Large.part
	
	End


End