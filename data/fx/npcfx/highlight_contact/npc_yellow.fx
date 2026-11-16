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
		part	:ringYellow01.part
		part	:ringYellow02.part
	
	End

	Event	
		ENAME	rings
		Type	Local
		At	Root
		Bhvr	Behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0 0 0
		End
		part	:glowYellow01.part
	
	End

#	Event	
#		ENAME	Spinner
#		Type	Local
#		At	Rings
#		Bhvr	Behaviors/GenericParticleFade.bhvr
#		BhvrOverride
#			PositionOffset	0 .0 0
#			Spin	0 .05 0
#		End
#	
#	End
#
#	Event	
#		ENAME	rings2
#		Type	Local
#		At	Spinner
#		Bhvr	Behaviors/GenericParticleFade.bhvr
#		BhvrOverride
#			PositionOffset	0 .55 0
#			pyrRotate	.2 0 0
#		End
#		part	:ringYellow01.part
#		part	:ringYellow02.part
#		part	:glowYellow01.part
#	
#	End
#
#	Event	
#		ENAME	rings3
#		Type	Local
#		At	spinner
#		Bhvr	Behaviors/GenericParticleFade.bhvr
#		BhvrOverride
#			PositionOffset	0 1.6 0
#			pyrRotate	.2 3.14159 0
#		End
#		part	:ringYellow01.part
#		part	:ringYellow02.part
#		part	:glowYellow01.part
#	
#	End
#
#End

End