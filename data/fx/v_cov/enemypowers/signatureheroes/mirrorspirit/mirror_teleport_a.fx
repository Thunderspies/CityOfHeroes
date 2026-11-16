#########################################################
##	Mirror Cage
########################################################


FxInfo
Lifespan 120

########  offset for mirror spawn  #################

Condition
	On	Time
	Time	0

	Event
		EName	Offset
		Type	Start  ##Local
		At	Root
#		Geom	Testing_Target
#				Bhvr	Behaviors/GenericParticleFade.bhvr
#                        BhvrOverride
#                                    Behavior
#                                    PositionOffset		-.7 .7 -.7   ##  -.7 .7 -.7 
#			End

		ChildFX :Mirror_teleport_event.fx

		Lifespan	50
		
	End

End
#####################################

##Condition
##	On	Time
##	Time	50
##
##	Event
##		EName	Offset2
##		Type	Local
##		At	Root
##		Geom	Testing_Target
##				Bhvr	Behaviors/GenericParticleFade.bhvr
##                        BhvrOverride
##                                    Behavior
##                                    PositionOffset		-.7 .7 -.7 #-.2 .6 -.4 #.4 .7 -.7
##			End
##		
##		ChildFX :Mirror_teleport_event.fx
##
##		Lifespan	50
##		
##	End
##
##End

End
