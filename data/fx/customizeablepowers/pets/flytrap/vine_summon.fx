#############################################################
## Vine_Summon.fx
#############################################################

FxInfo

Lifespan 150

#############################################################

Condition
	On	Time
	Time	25


	Event
		ename	dirtholeR
		Type 	local
		At	root
		Part	V_COV\Powers\PlantControl\Flytrap\summon_dirt.part
		bhvr	V_COV\Powers\PlantControl\Flytrap\summon_dirtR.bhvr
		Lifespan 44
	End

	Event
		ename	dirtholeL
		Type 	local
		At	root
		Part	V_COV\Powers\PlantControl\Flytrap\summon_dirt.part
		bhvr	V_COV\Powers\PlantControl\Flytrap\summon_dirtL.bhvr
		Lifespan 44
	End
End

Condition
	On	Time
	Time	45


	Event
		Type 	local
		At	C_Hind_LlegR
		Part	V_COV\Powers\PlantControl\Flytrap\summon_squirt.part
		Part	V_COV\Powers\PlantControl\Flytrap\summon_fumes.part
		bhvr	V_COV\Powers\PlantControl\Flytrap\summon_tentoff.bhvr
		lifespan 80
	End

	Event
		Type 	local
		At	C_Hind_LlegL
		Part	V_COV\Powers\PlantControl\Flytrap\summon_squirt.part
		Part	V_COV\Powers\PlantControl\Flytrap\summon_fumes.part
		bhvr	V_COV\Powers\PlantControl\Flytrap\summon_tentoff.bhvr
		lifespan 080
	End

	Event
		Type 	local
		At	dirtholeR
		Part	V_COV\Powers\PlantControl\Flytrap\summon_squirt.part
	end

	Event
		Type 	local
		At	dirtholeL
		Part	V_COV\Powers\PlantControl\Flytrap\summon_squirt.part
	end
End

#############################################################

End