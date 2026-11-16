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
		Bhvr 	behaviors\BioArmor_Fem.bhvr
		BhvrOverride
			Positionoffset	0.1 0.1 0
#			EndScale	0 0 0
			TintGeom	1
		End
		geom	FX_FootL_BioArmor_01

	End

	Event
		Type	Local
		At	FootR
		Bhvr 	behaviors\BioArmor_Fem.bhvr
		BhvrOverride
			Positionoffset	-0.1 0.1 0
#			EndScale	0 0 0
			TintGeom	1
		End
		geom	FX_FootR_BioArmor_01
	End
End

Condition
	On 	Time
	Time 	3

	Event
		Type	Local
		At	LLEGL
		Bhvr 	behaviors\BioArmor_Fem.bhvr
		BhvrOverride
			Positionoffset	0 -0.05 0
#			EndScale	0 0 0
			TintGeom	1
		End
		geom	FX_LLEGL_BioArmor_01
	End

	Event
		Type	Local
		At	LLEGR
		Bhvr 	behaviors\BioArmor_Fem.bhvr
		BhvrOverride
			Positionoffset	0 0 0
#			EndScale	0 0 0
			TintGeom	1
		End
		geom	FX_LLEGR_BioArmor_01
	End
End

#############################################################

Condition
	On 	Time
	Time 	5

	Event
		Type	Local
		At	UlegR
		Bhvr 	behaviors\BioArmor_Fem.bhvr
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
		Bhvr 	behaviors\BioArmor_Fem.bhvr
		BhvrOverride
#			Positionoffset	0 0 0
#			EndScale	0 0 0
			TintGeom	1
		End
		geom	FX_ULEGL_BioArmor_01
	End
End

#############################################################

Condition
	On 	Time
	Time 	10

	Event
		Type	Local
		At	LARMR
		Bhvr 	behaviors\BioArmor_Fem.bhvr
		BhvrOverride
#			Positionoffset	0 0 0
#			EndScale	0 0 0
			TintGeom	1
		End
		geom	FX_LarmR_BioArmor_01
	End

	Event
		Type	Local
		At	LARML
		Bhvr 	behaviors\BioArmor_Fem.bhvr
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
	Time 	15

	Event
		Type	Local
		At	CHEST
		Bhvr 	behaviors\BioArmor_Fem.bhvr
		BhvrOverride
			Positionoffset	0.0 0.1 0.0
			EndScale	0.7 0.7 0.7
			TintGeom	1
		End
		geom	FX_Chest_BioArmor_01
	End

	Event
		Type	Local
		At	UARML
		Bhvr 	behaviors\BioArmor_Fem.bhvr
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
		Bhvr 	behaviors\BioArmor_Fem.bhvr
		BhvrOverride
#			Positionoffset	0 0 0
#			EndScale	0 0 0
			TintGeom	1
		End
		geom	FX_UarmR_BioArmor_01
	End

	Event
		Type	Local
		At	UARMR
		Bhvr 	behaviors\BioArmor_Fem.bhvr
		BhvrOverride
#			Positionoffset	0 0 0
#			EndScale	0 0 0
			TintGeom	1
		End
		geom	FX_UarmR_BioArmor_02
	End
End

End