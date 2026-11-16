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

#Flags	Inheritalpha


Condition
	
	Event
		EName	1
		Type	Local
		At	chest
		
		Bhvroverride
			PositionOffset	0 1 0
		End

		Part	:PoisonHit.part		
		Part	:PoisonHitBlubbles.part
		bhvr	Behaviors/GenericParticleFade.bhvr
	End
	
End

Condition
	On	Cycle
	Time	10
	Chance	.55

	Event
		EName	1
		Type	Local
		At	chest
		
		Bhvroverride
			PositionOffset	0 1 0
		End

		ChildFx	:PoisonDripsmall.fx
		
		LifeSpan	20
	End

End


End				