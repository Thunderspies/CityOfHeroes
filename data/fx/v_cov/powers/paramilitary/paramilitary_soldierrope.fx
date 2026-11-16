#########################################################
##	pump_smoke
##
FxInfo

Input
	InpName	Origin
End

LifeSpan 150




Condition
	On Time
	Time 1

	Event
		EName	8
		Type	Local
		At	Mystic
		#Geom	Testing_Target
	End

	Event
		EName 	Rope
		Type	Start
		At	BendMystic

		bhvr	V_COV\Powers\Paramilitary\Paramilitary_B_RopeStretch.bhvr
		Geom	GEO_Rope
		LookAt	Mystic
	End

	Event
		EName 	RopeDangle
		Type	Posit
		At	Mystic

		bhvr	V_COV\Powers\Paramilitary\Paramilitary_B_RopeStretch.bhvr
		Geom	GEO_Rope
		LookAt	WepR
		Lifespan 60
	End

End


#Condition
#	On	Cycle
#	Time	3
#
#	Event
#		EName 	Ropes
#		Type	Start
#		At	Mystic
#
#		bhvr	V_COV\Powers\Paramilitary\Paramilitary_B_RopeStretch.bhvr
#		Geom	GEO_Rope
#		Lifespan 60
#		LookAt	Mystic
#	End
#
#End


End