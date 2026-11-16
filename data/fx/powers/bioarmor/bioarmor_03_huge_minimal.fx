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
		PMagnet UArmL
		Lifespan 45
	End

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
		Part3	:SmokeyTendril_Big.part
		PMagnet UArmR
		Lifespan 45
	End

	Event
		Ename	OffsetHipRight
		Type	Local
		At	ULegR
		BhvrOverride
			PositionOffset 0.1 0 0
		End
	End

	Event
		Ename	OffsetHipLeft
		Type	Local
		At	ULegL
		BhvrOverride
			PositionOffset -0.1 0 0
		End
	End

	Event
		Type	Local
		At	Chest
		Bhvr	Behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset -0.3 0 0
		End
		#part1	:LiteSmoke.part
		#part2	:LiteSmoke2.part
		Part3	:SmokeyTendril.part
		PMagnet OffsetHipLeft
		Lifespan 45
	End

	Event
		Type	Local
		At	Chest
		Bhvr	Behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset 0.3 0 0
		End
		#part1	:LiteSmoke.part
		#part2	:LiteSmoke2.part
		Part3	:SmokeyTendril.part
		PMagnet OffsetHipRight
		Lifespan 45
	End

	Event
		Type	Local
		At	UArmL
		Bhvr	Behaviors/GenericParticleFade.bhvr
		#part1	:LiteSmoke.part
		#part2	:LiteSmoke2.part
		Part3	:SmokeyTendril.part
		Part3	:SmokeyTendril_Big.part
		part1	:OuterSmoke.part
		PMagnet LArmL
		Lifespan 45
	End

	Event
		Type	Local
		At	LArmL
		Bhvr	Behaviors/GenericParticleFade.bhvr
		#part1	:LiteSmoke.part
		#part2	:LiteSmoke2.part
		Part3	:SmokeyTendril.part
		Part3	:SmokeyTendril_Big.part
		part1	:OuterSmoke.part
		PMagnet WepL
		Lifespan 45
	End

	Event
		Type	Local
		At	UArmR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		#part1	:LiteSmoke.part
		#part2	:LiteSmoke2.part
		Part3	:SmokeyTendril.part
		Part3	:SmokeyTendril_Big.part
		part1	:OuterSmoke.part
		PMagnet LArmR
		Lifespan 45
	End

	Event
		Type	Local
		At	LArmR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		#part1	:LiteSmoke.part
		#part2	:LiteSmoke2.part
		Part3	:SmokeyTendril.part
		Part3	:SmokeyTendril_Big.part
		part1	:OuterSmoke.part
		PMagnet WepR
		Lifespan 45
	End

	Event
		Type	Local
		At	ULEGL
		Bhvr	Behaviors/GenericParticleFade.bhvr
		BhvrOverride
			#Positionoffset 0 -0.5 0
		End
		#part1	:LiteSmoke.part
		#part2	:LiteSmoke2.part
		Part3	:SmokeyTendril.part
		Part3	:SmokeyTendril_Big.part
		part1	:OuterSmoke.part
		PMagnet LLEGL
		Lifespan 45
	End

	Event
		Type	Local
		At	ULEGR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		BhvrOverride
			#Positionoffset 0 -0.5 0
		End
		#part1	:LiteSmoke.part
		#part2	:LiteSmoke2.part
		Part3	:SmokeyTendril.part
		Part3	:SmokeyTendril_Big.part
		part1	:OuterSmoke.part
		PMagnet LlegR
		Lifespan 45
	End

	Event
		Type	Local
		At	LLEGL
		Bhvr	Behaviors/GenericParticleFade.bhvr
		#part1	:LiteSmoke.part
		#part2	:LiteSmoke2.part
		Part3	:SmokeyTendril.part
		Part3	:SmokeyTendril_Big.part
		part1	:OuterSmoke.part
		PMagnet FootL
		Lifespan 45
	End

	Event
		Type	Local
		At	LLEGR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		#part1	:LiteSmoke.part
		#part2	:LiteSmoke2.part
		Part3	:SmokeyTendril.part
		Part3	:SmokeyTendril_Big.part
		part1	:OuterSmoke.part
		PMagnet FootR
		Lifespan 45
	End

#	Event
#		Type	Local
#		At	FootL
#		Bhvr	Behaviors/GenericParticleFade.bhvr
#		#part1	:LiteSmoke.part
#		#part2	:LiteSmoke2.part
#		#Part3	:SmokeyTendril.part
#		Part3	:SmokeyTendril_Big.part
#		Pmagnet LLegL
#	End
#
#	Event
#		Type	Local
#		At	FootR
#		Bhvr	Behaviors/GenericParticleFade.bhvr
#		#part1	:LiteSmoke.part
#		#part2	:LiteSmoke2.part
#		#Part3	:SmokeyTendril.part
#		Part3	:SmokeyTendril_Big.part
#		Pmagnet	LLegR
#	End

End

#############################################################

End