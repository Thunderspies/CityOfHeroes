#############################################################
## Ashes_Heavy.fx
#############################################################

FxInfo

#############################################################

 Condition
	On	Time
	Time	0

	Event
		Ename	MeteorFal
		Type	StartPositOnly
		At	Root
	End
End



Condition
	On		Cycle
	Time		20
	Repeat		1
	RepeatJitter	2
	Chance		0.3

	Event
		Ename	MeteorFal
		Type	Local
		At	Root

		ChildFX	:Meteor_Fall_CHILD.fx
		Lifespan 120
	End
End

#Condition
#	On	Cycle
#	Time	5
#
#	Event
#		Type	Local
#		At	MeteorAnchor
#		Bhvr	Behaviors/GenericParticleFade.bhvr
##		BhvrOverride
##			PositionOffset	-5.0 25.0 100.0
##		End
#		ChildFX	:Meteor_Fall_CHILD.fx
#	End
#End

#############################################################

End

