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
			PositionOffset	0 .5 0
		End
		part	:ringGreen01.part
		part	:ringGreen02.part
	
	End

	Event	
		ENAME	glow
		Type	local
		At	Root
		Bhvr	Behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0 0 0
		End
		part	:glowGreen01.part
	
	End

End