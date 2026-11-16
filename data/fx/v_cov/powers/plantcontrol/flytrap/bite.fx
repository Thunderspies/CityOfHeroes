#############################################################
## Bite.fx
#############################################################

FxInfo

Lifespan 60

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	Down
		Type 	start
		At	root
		bhvr	behaviors\dir_down.bhvr
		Sound Snakebite1 60.0 60.0 0.78
	End
End

#############################################################

Condition
	On	Time
	Time	19

	Event
		EName	DripEmit
		Type 	local
		At	root
		bhvr	V_COV\Powers\PlantControl\Flytrap\bite_spittle_off.bhvr
		Part	V_COV\Powers\PlantControl\Flytrap\Splatter2.part
		PMagnet Down
	End
End

Condition
	On	Time
	Time	22

	Event
		Type 	Local
		At	DripEmit
		Part	V_COV\Powers\PlantControl\Flytrap\Splatter1.part
		Part	V_COV\Powers\PlantControl\Flytrap\Splatter3.part
		Lifespan 4
	End
End

#############################################################

End