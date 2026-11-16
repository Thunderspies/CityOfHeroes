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

#	Event
#		Type	Local
#		At	LARMR
#		Bhvr 	behaviors\BioArmor_Huge.bhvr
#		BhvrOverride
#			Positionoffset	0 -0.1 0
#			EndScale	1.45 1.45 1.45
#			TintGeom	1
#		End
#		geom	FX_LarmR_BioArmor_01
#	End

	Event
		Type	Local
		At	LARML
		Bhvr 	behaviors\BioArmor_Huge.bhvr
		BhvrOverride
			Positionoffset	0 -0.1 0
			EndScale	1.45 1.45 1.45
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
		Bhvr 	behaviors\BioArmor_Huge.bhvr
		BhvrOverride
			Positionoffset	-0.05 -0.2 -0.1
			EndScale	1.55 1.35 1.35
			PYRrotate 	10 0 0
			TintGeom	1
		End
		geom	FX_Chest_BioArmor_01
	End

	Event
		Type	Local
		At	UARML
		Bhvr 	behaviors\BioArmor_Huge.bhvr
		BhvrOverride
			Positionoffset	0.2 0.0 0.03
			EndScale	1.4 1.4 1.4
			TintGeom	1
		End
		geom	FX_UarmL_BioArmor_01
	End

#	Event
#		Type	Local
#		At	UARMR
#		Bhvr 	behaviors\BioArmor_Huge.bhvr
#		BhvrOverride
#			Positionoffset	-0.5 0.0 0.0
#			EndScale	1.4 1.4 1.4
#			TintGeom	1
#		End
#		geom	FX_UarmR_BioArmor_01
#	End
#
#	Event
#		Type	Local
#		At	UARMR
#		Bhvr 	behaviors\BioArmor_Huge.bhvr
#		BhvrOverride
#			Positionoffset	-0.3 0 0
#			EndScale	1.4 1.4 1.4
#			TintGeom	1
#		End
#		geom	FX_UarmR_BioArmor_02
#	End
End

####################################################################

Condition
	On 	Time
	Time 	45

#	Event
#		Type	Local
#		At	FootL
#		Bhvr 	behaviors\BioArmor_Huge.bhvr
#		BhvrOverride
#			Positionoffset	0.1 0 0
#			TintGeom	1
#		End
#		geom	FX_FootL_BioArmor_01
#	End

	Event
		Type	Local
		At	FootR
		Bhvr 	behaviors\BioArmor_Huge.bhvr
		BhvrOverride
			Positionoffset	-0.1 0 0
			TintGeom	1
		End
		geom	FX_FootR_BioArmor_01
	End
End

Condition
	On 	Time
	Time 	48

#	Event
#		Type	Local
#		At	LLEGL
#		Bhvr 	behaviors\BioArmor_Huge.bhvr
#		BhvrOverride
#			Positionoffset	0.0 0.4 0.0
#			EndScale	1.4 1.4 1.4
#			TintGeom	1
#		End
#		geom	FX_LLEGL_BioArmor_01
#	End

	Event
		Type	Local
		At	LLEGR
		Bhvr 	behaviors\BioArmor_Huge.bhvr
		BhvrOverride
			Positionoffset	0 0 0
			EndScale	1.4 1.1 1.4
			TintGeom	1
		End
		geom	FX_LLEGR_BioArmor_01
	End
End

#############################################################
#
#Condition
#	On 	Time
#	Time 	50

#	Event
#		Type	Local
#		At	UlegR
#		Bhvr 	behaviors\BioArmor_Huge.bhvr
#		BhvrOverride
#			Positionoffset	-0.1 0.3 -0.05
#			EndScale	1.2 1.2 1.2
#			TintGeom	1
#		End
#		geom	FX_UlegR_BioArmor_01
#	End
#
#	Event
#		Type	Local
#		At	ULEGL
#		Bhvr 	behaviors\BioArmor_Huge.bhvr
#		BhvrOverride
#			Positionoffset	0.0 0.3 -0.1
#			EndScale	1.2 1.2 1.2
#			TintGeom	1
#		End
#		geom	FX_ULEGL_BioArmor_01
#	End

########################

#	Event
#		Type	Local
#		At	Chest
#		#Bhvr 	behaviors\BioArmor.bhvr
#		BhvrOverride
#			FadeInLength 35
#			Positionoffset	0.4 0.0 0
#			Scale		2 2 2
#			PyrRotate	115 -90 90
#			#PyrRotateJitter	0 30 45
#			TintGeom	1
#		End
#		geom	FX_BioArmor_Spike_01
#		geom	FX_BioArmor_Spike_02
#		geom	FX_BioArmor_Spike_03
#	End
#
#	Event
#		Type	Local
#		At	Chest
#		#Bhvr 	behaviors\BioArmor.bhvr
#		BhvrOverride
#			FadeInLength 35
#			Positionoffset	-0.1 0.45 0
#			Scale		2 2.5 2
#			PyrRotate	60 55 0
#			#PyrRotateJitter	0 30 45
#			TintGeom	1
#		End
#		geom	FX_BioArmor_Spike_01
#		geom	FX_BioArmor_Spike_02
#		geom	FX_BioArmor_Spike_03
#	End
#
#	Event
#		Type	Local
#		At	UArmL
#		#Bhvr 	behaviors\BioArmor.bhvr
#		BhvrOverride
#			FadeInLength 35
#			Positionoffset	0.0 -0.3 0
#			Scale		1 1 1
#			PyrRotate	90 45 45
#			#PyrRotateJitter	60 45 45
#			TintGeom	1
#		End
#		geom	FX_BioArmor_Spike_01
#		geom	FX_BioArmor_Spike_02
#		geom	FX_BioArmor_Spike_03
#	End
#
#	Event
#		Type	Local
#		At	UArmR
#		#Bhvr 	behaviors\BioArmor.bhvr
#		BhvrOverride
#			FadeInLength 35
#			Positionoffset	0.3 -0.0 0
#			Scale		1.8 1.8 2
#			PyrRotate	30 90 100
#			PyrRotateJitter	30 45 0
#			TintGeom	1
#		End
#		geom	FX_BioArmor_Spike_01
#		geom	FX_BioArmor_Spike_02
#		geom	FX_BioArmor_Spike_03
#	End
#
#	Event
#		Type	Local
#		At	LArmL
#		#Bhvr 	behaviors\BioArmor.bhvr
#		BhvrOverride
#			FadeInLength 35
#			Positionoffset	0.05 -0.05 0.05
#			Scale		2 1.5 2
#			PyrRotate	180 90 100
#			#PyrRotateJitter	45 45 45
#			TintGeom	1
#		End
#		geom	FX_BioArmor_Spike_01
#		geom	FX_BioArmor_Spike_02
#		geom	FX_BioArmor_Spike_03
#	End

#	Event
#		Type	Local
#		At	ULegL
#		#Bhvr 	behaviors\BioArmor.bhvr
#		BhvrOverride
#			FadeInLength 35
#			Positionoffset	-0.1 -0.8 0.1
#			Scale		1.8 1.4 1.4
#			PyrRotate	-90 -145 0
#			PyrRotateJitter	30 15 0
#			TintGeom	1
#		End
#		geom	FX_BioArmor_Spike_01
#		geom	FX_BioArmor_Spike_02
#		geom	FX_BioArmor_Spike_03
#	End

#	Event
#		Type	Local
#		At	ULegR
#		#Bhvr 	behaviors\BioArmor.bhvr
#		BhvrOverride
#			Positionoffset	0 -0.7 -0
#			Scale		1.8 1.4 1.8
#			PyrRotate	-90 -0 30
#			#PyrRotateJitter	45 45 45
#			TintGeom	1
#		End
#		geom	FX_BioArmor_Spike_01
#		geom	FX_BioArmor_Spike_02
#		geom	FX_BioArmor_Spike_03
#	End
#End

##############################################################

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