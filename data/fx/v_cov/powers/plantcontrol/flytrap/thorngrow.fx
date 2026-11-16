#############################################################
## ThornGrow.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	C_T2_R
		bhvr	V_COV\Powers\PlantControl\Flytrap\ThornGrowRot.bhvr
		Geom	Thorn4
		Lifespan 17
	End

	Event
		Type	Local
		At	C_T2_L
		bhvr	V_COV\Powers\PlantControl\Flytrap\ThornGrowRot.bhvr
		Geom	Thorn4
		Lifespan 17
	End
End

Condition
	On 	Time
	Time 	1

	Event
		Type	Local
		At	C_T3_R
		bhvr	V_COV\Powers\PlantControl\Flytrap\ThornGrowRot.bhvr
		Geom	Thorn4
		Lifespan 17
	End

	Event
		Type	Local
		At	C_T3_L
		bhvr	V_COV\Powers\PlantControl\Flytrap\ThornGrowRot.bhvr
		Geom	Thorn4
		Lifespan 17
	End
end

Condition
	On 	Time
	Time 	2

	Event
		Type	Local
		At	C_Hind_UlegR
		bhvr	V_COV\Powers\PlantControl\Flytrap\ThornGrowRot.bhvr
		Geom	Thorn4
		Lifespan 17
	End

	Event
		Type	Local
		At	C_Hind_UlegL
		bhvr	V_COV\Powers\PlantControl\Flytrap\ThornGrowRot.bhvr
		Geom	Thorn4
		Lifespan 17
	End
end

Condition
	On 	Time
	Time 	3


	Event
		Type	Local
		At	C_Hind_LlegR
		bhvr	V_COV\Powers\PlantControl\Flytrap\ThornGrowRot.bhvr
		Geom	Thorn4
		Lifespan 17
	End

	Event
		Type	Local
		At	C_Hind_LlegL
		bhvr	V_COV\Powers\PlantControl\Flytrap\ThornGrowRot.bhvr
		Geom	Thorn4
		Lifespan 17
	End
end

#############################################################

Condition
	On 	Time
	Time 	3

	Event
		Type	Local
		At	C_T1_R
		Part	V_COV\Powers\PlantControl\Flytrap\thorndust.part
		Lifespan 32
		pother	C_F2_R
	End

	Event
		Type	Local
		At	C_T2_R
		Part	V_COV\Powers\PlantControl\Flytrap\thorndust.part
		Lifespan 32
		pother	C_T1_R
	End

	Event
		Type	Local
		At	C_T3_R
		Part	V_COV\Powers\PlantControl\Flytrap\thorndust.part
		Lifespan 32
		pother	C_T2_R
	End

	Event
		Type	Local
		At	C_Hind_UlegR
		Part	V_COV\Powers\PlantControl\Flytrap\thorndust.part
		Lifespan 32
		pother	C_T3_R
	End

	Event
		Type	Local
		At	C_Hind_LlegR
		Part	V_COV\Powers\PlantControl\Flytrap\thorndust.part
		Lifespan 32
		pother	C_Hind_UlegR
	End

	Event
		Type	Local
		At	C_T1_L
		Part	V_COV\Powers\PlantControl\Flytrap\thorndust.part
		Lifespan 32
		pother	C_F2_L
	End

	Event
		Type	Local
		At	C_T2_L
		Part	V_COV\Powers\PlantControl\Flytrap\thorndust.part
		Lifespan 32
		pother	C_T1_L
	End

	Event
		Type	Local
		At	C_T3_L
		Part	V_COV\Powers\PlantControl\Flytrap\thorndust.part
		Lifespan 32
		pother	C_T2_L
	End

	Event
		Type	Local
		At	C_Hind_UlegL
		Part	V_COV\Powers\PlantControl\Flytrap\thorndust.part
		Lifespan 32
		pother	C_T3_L
	End

	Event
		Type	Local
		At	C_Hind_LlegL
		Part	V_COV\Powers\PlantControl\Flytrap\thorndust.part
		Lifespan 32
		pother	C_Hind_UlegL
	End
End

#############################################################

End