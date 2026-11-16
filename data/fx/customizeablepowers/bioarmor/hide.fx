#############################################################
## StoneArmor.fx
#############################################################

FxInfo

Flags    InheritAlpha InheritGeoScale

#############################################################

Condition
	On 	Time
	Time 	50

	Event
		Type	Local
		At	LARMR
		Bhvr 	:Ablative.bhvr
		BhvrOverride
#			Positionoffset	0 0 0
#			EndScale	0 0 0
			TintGeom	1
		End
	#	geom	FX_LarmR_BioArmor_01
	End

	Event
		Type	Local
		At	LARML
		Bhvr 	:Ablative.bhvr
		BhvrOverride
#			Positionoffset	0 0 0
#			EndScale	0 0 0
			TintGeom	1
		End
		geom	FX_LarmL_BioArmor_01
	End
End

#############################################################


Condition
	On 	Time
	Time 	45

	Event
		Type	Local
		At	CHEST
		Bhvr 	:Ablative.bhvr
		BhvrOverride
#			Positionoffset	0 0 0
#			EndScale	0 0 0
			TintGeom	1
		End
		geom	FX_Chest_BioArmor_01
	End

	Event
		Type	Local
		At	UARML
		Bhvr 	:Ablative.bhvr
		BhvrOverride
#			Positionoffset	0 0 0
#			EndScale	0 0 0
			TintGeom	1
		End
		geom	FX_UarmL_BioArmor_01
	End

	Event
		Type	Local
		At	UARMR
		Bhvr 	:Ablative.bhvr
		BhvrOverride
#			Positionoffset	0 0 0
#			EndScale	0 0 0
			TintGeom	1
		End
	#	geom	FX_UarmR_BioArmor_01
	End

	Event
		Type	Local
		At	UARMR
		Bhvr 	:Ablative.bhvr
		BhvrOverride
#			Positionoffset	0 0 0
#			EndScale	0 0 0
			TintGeom	1
		End
		geom	FX_UarmR_BioArmor_02
	End
End

####################################################################

Condition
	On 	Time
	Time 	45

	Event
		Type	Local
		At	FootL
		Bhvr 	:Ablative.bhvr
		BhvrOverride
#			Positionoffset	0 0 0
#			EndScale	0 0 0
			TintGeom	1
		End
	#	geom	FX_FootL_BioArmor_01

	End

	Event
		Type	Local
		At	FootR
		Bhvr 	:Ablative.bhvr
		BhvrOverride
#			Positionoffset	0 0 0
#			EndScale	0 0 0
			TintGeom	1
		End
		geom	FX_FootR_BioArmor_01
	End
End

Condition
	On 	Time
	Time 	48

	Event
		Type	Local
		At	LLEGL
		Bhvr 	:Ablative.bhvr
		BhvrOverride
#			Positionoffset	0 0 0
#			EndScale	0 0 0
			TintGeom	1
		End
	#	geom	FX_LLEGL_BioArmor_01
	End

	Event
		Type	Local
		At	LLEGR
		Bhvr 	:Ablative.bhvr
		BhvrOverride
#			Positionoffset	0 0 0
#			EndScale	0 0 0
			TintGeom	1
		End
		geom	FX_LLEGR_BioArmor_01
	End
End

#############################################################

Condition
	On 	Time
	Time 	50

	Event
		Type	Local
		At	UlegR
		Bhvr 	:Ablative.bhvr
		BhvrOverride
#			Positionoffset	0 0 0
#			EndScale	0 0 0
			TintGeom	1
		End
		geom	FX_UlegR_BioArmor_01
	End

	Event
		Type	Local
		At	ULEGL
		Bhvr 	:Ablative.bhvr
		BhvrOverride
#			Positionoffset	0 0 0
#			EndScale	0 0 0
			TintGeom	1
		End
	#	geom	FX_ULEGL_BioArmor_01
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
		PMagnet UArmL
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

End