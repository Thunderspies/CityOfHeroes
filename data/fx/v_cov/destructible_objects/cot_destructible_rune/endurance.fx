#########################################################
##	chill touch hit
FxInfo

##################################

#Condition
#	
#	Event
#		EName	Ring
#		Type	Local
#		At	Root
#		part1	Powers/Ability/IntelligenceRingUp.part
#		sound	heal1 80 20 .8
#		
#	End
#
#End


Condition
	
	Event
		EName	1
		Type	Local
		At	chest
		
		BhvrOverride	
			PositionOffset	0.4 1 0
		End

		part	:endurance01.part
		part	:endurance02.part
		Part	:EnduranceBubbles.part
		bhvr	Behaviors/GenericParticleFade.bhvr
	End

End


End				