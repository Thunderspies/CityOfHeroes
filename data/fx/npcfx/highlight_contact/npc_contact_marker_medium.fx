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
			PositionOffset	0 .1 0
		End
		part	:ring01Medium.part
		part	:ring02Medium.part
	
	End

	Event	
		ENAME	rings
		Type	Local
		At	Root
		Bhvr	Behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0 .1 0
		End
		part	:glow01Medium.part
	
	End


End