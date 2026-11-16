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
	Time 	0

	Event
		Type	Local
		At	FootL
		Bhvr 	behaviors\BioArmor_Huge.bhvr
		BhvrOverride
			Positionoffset	0.1 0 0
			TintGeom	1
		End
		geom	FX_FootL_BioArmor_01

	End

#	Event
#		Type	Local
#		At	FootR
#		Bhvr 	behaviors\BioArmor_Huge.bhvr
#		BhvrOverride
#			Positionoffset	-0.1 0 0
#			TintGeom	1
#		End
#		geom	FX_FootR_BioArmor_01
#	End
End

Condition
	On 	Time
	Time 	48

	Event
		Type	Local
		At	LLEGL
		Bhvr 	behaviors\BioArmor_Huge.bhvr
		BhvrOverride
			Positionoffset	0.0 0.1 0.0
			EndScale	1.4 1.2 1.4
			TintGeom	1
		End
		geom	FX_LLEGL_BioArmor_01
	End

#	Event
#		Type	Local
#		At	LLEGR
#		Bhvr 	behaviors\BioArmor_Huge.bhvr
#		BhvrOverride
#			Positionoffset	0 0.4 0
#			EndScale	1.4 1.4 1.4
#			TintGeom	1
#		End
#		geom	FX_LLEGR_BioArmor_01
#	End
End

#############################################################

Condition
	On 	Time
	Time 	50

	Event
		Type	Local
		At	UlegR
		Bhvr 	behaviors\BioArmor_Huge.bhvr
		BhvrOverride
			Positionoffset	-0.1 0.3 -0.05
			EndScale	1.2 1.2 1.2
			TintGeom	1
		End
		geom	FX_UlegR_BioArmor_01
	End

#	Event
#		Type	Local
#		At	ULegR
#		BhvrOverride
#			FadeInLength	120
#			FadeOutLength	30
#			Positionoffset	0 -0.4 0
#			Scale		1.4 1.4 1.4
#			PyrRotate	-90 -0 21
#			TintGeom	1
#		End
#		geom	FX_BioArmor_Spike_03
#	End

	Event
		Type	Local
		At	ULEGL
		Bhvr 	behaviors\BioArmor_Huge.bhvr
		BhvrOverride
			Positionoffset	0.0 0.3 -0.1
			EndScale	1.2 1.2 1.3
			TintGeom	1
		End
		geom	FX_ULEGL_BioArmor_01
	End
End

#############################################################

Condition
	On	Time
	Time	0

#	Event
#		EName	2
#		Type	Local
#		At	FootR
#		Bhvr	Behaviors/GenericParticleFade.bhvr
#		part1	:LiteSmoke.part
#		part2	:LiteSmoke2.part
#		Part3	:SmokeyTendril.part
#		Lifespan 45
#	End

	Event
		EName	3
		Type	Local
		At	ULEGL
		Bhvr	Behaviors/GenericParticleFade.bhvr
		#part1	:LiteSmoke.part
		Part3	:SmokeyTendril.part
		Part3	:SmokeyTendril_Big.part
		part1	:OuterSmoke.part
		PMagnet LLEGL
		Lifespan 45
	End

	Event
		EName	4
		Type	Local
		At	LLEGL
		Bhvr	Behaviors/GenericParticleFade.bhvr
		#part1	:LiteSmoke.part
		Part3	:SmokeyTendril.part
		Part3	:SmokeyTendril_Big.part
		part1	:OuterSmoke.part
		PMagnet FootL
		Lifespan 45
	End

	Event
		EName	5
		Type	Local
		At	FootL
		Bhvr	Behaviors/GenericParticleFade.bhvr
		#part1	:LiteSmoke.part
		Part3	:SmokeyTendril.part
		Part3	:SmokeyTendril_Big.part
		part1	:OuterSmoke.part
		Lifespan 45
	End

	Event
		EName	6
		Type	Local
		At	ULEGR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		#part1	:LiteSmoke.part
		Part3	:SmokeyTendril.part
		Part3	:SmokeyTendril_Big.part
		part1	:OuterSmoke.part
		PMagnet LlegR
		Lifespan 45
	End

#	Event
#		EName	7
#		Type	Local
#		At	LLEGR
#		Bhvr	Behaviors/GenericParticleFade.bhvr
#		#part1	:LiteSmoke.part
#		part2	:LiteSmoke3.part
#		part2	:LiteSmoke4.part
#		Part3	:SmokeyTendril.part
#		PMagnet FootR
#		Lifespan 45
#	End
End

##########################################################

End