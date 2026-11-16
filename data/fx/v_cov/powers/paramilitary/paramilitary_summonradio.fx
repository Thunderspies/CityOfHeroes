#############################################################
## Paramilitary_SummonRadio.fx
#############################################################

FxInfo

Flags InheritAnimScale

Lifespan 170

#################################################################################
#####################  Static Radio and Antenna Stump  ##########################
#################################################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	WepR
		Sound WalkieTalkie1 60 60 .55
		Sound WalkieTalkie2 60 60 .55
		Sound WalkieTalkie3 60 60 .55
	End
End

Condition
	On	Time
	Time	3

	Event
		EName	RadioBody
		Type	Local
		At	WepL
		bhvr	V_COV\Powers\Paramilitary\Paramilitary_B_SummonRadio.bhvr
		Geom	GEO_Radio
		Lifespan 156
	End

	Event
		EName	AntennaStump
		Type	Local
		At	RadioBody
		AltPiv	2
		bhvr	V_COV\Powers\Paramilitary\Paramilitary_B_SummonAntPos.bhvr
		Geom	Geo_Antenna
		Lifespan 156
	End
End

#################################################################################
############################  E X T E N D   Antenna  ############################
#################################################################################

Condition
	On	Time
	Time	21

	Event
		EName	AntennaOut
		Type	Local
		At	RadioBody
		AltPiv	2
		bhvr	V_COV\Powers\Paramilitary\Paramilitary_B_SummonAntOut.bhvr
		Geom	GEO_Antenna
		Lifespan 119
	End
End

#################################################################################
##########################  C O N T R A C T   Antenna  ##########################
#################################################################################

Condition
	On	Time
	Time	140

	Event
		EName	AntennaIn
		Type	Local
		At	RadioBody
		AltPiv	2
		bhvr	V_COV\Powers\Paramilitary\Paramilitary_B_SummonAntIn.bhvr
		Geom	GEO_Antenna
	End
End

#################################################################################
#####################  Static Radio and Antenna Shrink  #########################
#################################################################################

Condition
	On	Time
	Time	158

	Event
		EName	RadioBodySH
		Type	Local
		At	WepL
		bhvr	V_COV\Powers\Paramilitary\Paramilitary_B_SummonRadioSH.bhvr
		Geom	GEO_Radio
	End

	Event
		EName	AntennaStumpSH
		Type	Local
		At	RadioBody
		AltPiv	2
		bhvr	V_COV\Powers\Paramilitary\Paramilitary_B_SummonAntPos.bhvr
		Geom	Geo_Antenna
	End
End

#############################################################

End