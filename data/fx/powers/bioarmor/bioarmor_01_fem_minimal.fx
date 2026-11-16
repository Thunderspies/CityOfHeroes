#############################################################
## StoneArmor.fx
#############################################################

FxInfo

Flags    InheritAlpha InheritGeoScale

#############################################################

#	SOUND FX	#####################################

#Condition
#	On	Time
#	Time 	0
#
#	Event
#		Type	Local
#		At 	origin
#		Sound Stone6 100 100 0.95
#	End
#End
#
#Condition
#	On	Time
#	Time 	5
#
#	Event
#		Type	Local
#		At 	origin
#		Sound Stone_loop 100 100 0.3
#	End
#End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Chest
		Bhvr	Behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset 0 0.3 0
		End
		part1	:LiteSmoke.part
		part2	:LiteSmoke2.part
		Part3	:SmokeyTendril.part
		PMagnet UArmR
		Lifespan 45
	End

	Event
		Type	Local
		At	UArmR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		part1	:LiteSmoke.part
		part2	:LiteSmoke2.part
		Part3	:SmokeyTendril.part
		PMagnet LArmR
		Lifespan 45
	End

	Event
		Type	Local
		At	LArmR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		part1	:LiteSmoke.part
		part2	:LiteSmoke2.part
		Part3	:SmokeyTendril.part
		PMagnet WepR
		Lifespan 45
	End

End

#############################################################

End