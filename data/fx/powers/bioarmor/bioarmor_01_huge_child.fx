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
	On 	Time
	Time 	50

	Event
		Type	Local
		At	LARMR
		Bhvr 	behaviors\BioArmor_Huge.bhvr
		BhvrOverride
			Positionoffset	0 -0.1 0
			EndScale	1.45 1.45 1.45
			TintGeom	1
		End
		geom	FX_LarmR_BioArmor_01
	End

#	Event
#		Type	Local
#		At	LARML
#		Bhvr 	behaviors\BioArmor_Huge.bhvr
#		BhvrOverride
#			Positionoffset	0 -0.1 0
#			EndScale	1.45 1.45 1.45
#			TintGeom	1
#		End
#		geom	FX_LarmL_BioArmor_01
#	End
End

#############################################################

Condition
	On 	Time
	Time 	45

#	Event
#		Type	Local
#		At	CHEST
#		Bhvr 	behaviors\BioArmor_Huge.bhvr
#		BhvrOverride
#			Positionoffset	0.0 -0.1 0.05
#			EndScale	1.35 1.35 1.35
#			PYRrotate 	10 0 0
#			TintGeom	1
#		End
#		geom	FX_Chest_BioArmor_01
#	End
#
#	Event
#		Type	Local
#		At	UARML
#		Bhvr 	behaviors\BioArmor_Huge.bhvr
#		BhvrOverride
#			Positionoffset	0.4 0.0 0.0
#			EndScale	1.4 1.4 1.4
#			TintGeom	1
#		End
#		geom	FX_UarmL_BioArmor_01
#	End

	Event
		Type	Local
		At	UARMR
		Bhvr 	behaviors\BioArmor_Huge.bhvr
		BhvrOverride
			Positionoffset	-0.3 0.0 0.045
			EndScale	1.4 1.4 1.4
			TintGeom	1
		End
		geom	FX_UarmR_BioArmor_01
	End

	Event
		Type	Local
		At	UARMR
		Bhvr 	behaviors\BioArmor_Huge.bhvr
		BhvrOverride
			Positionoffset	-0.3 0 0
			EndScale	1.4 1.4 1.4
			TintGeom	1
		End
		geom	FX_UarmR_BioArmor_02
	End
End

#########################################################

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

####################################################################

End