#############################################################
## StaticDischarge_Hit.fx
#############################################################

FxInfo
Lifespan 90

########################################################

Input
	Inpname	Hips
End

#############################################################

Condition
	On 	time
	Time 	0

	Event
		Type	Local
		At	Origin
	Sound mu_elecballhit2 80 80 0.9
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Posit
		At	Origin
		ChildFX	:Child_StaticDischarge_Grounder.fx
		Lifespan 20
	End

	Event
		Type	Local
		At	Root
		Part	:mu_Elec_Hit_residual1.part
		Pother	Chest
		Lifespan 20
	End
End

Condition
	On	Time
	Time	1

	Event
		Type	Posit
		At	Root
		ChildFX	:Child_StaticDischarge_Grounder.fx
		Lifespan 20
	End
End

Condition
	On	Time
	Time	2

	Event
		Type	start
		At	Origin
		Part	:mu_shockring1.part
		Lifespan 30
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Root
		childfx	:Child_StaticDischarge_Bodyshock.fx
		Lifespan 20
	End
End

Condition
	On	Cycle
	Time	15

	Event
		Ename	BodyShock_Cycle
		Type	Local
		At	Root
		childfx :Child_StaticDischarge_Bodyshock.fx
		Lifespan 20
	End
End

Condition
	On	Time
	Time	90

	Event
		Type	Destroy
		EName	BodyShock_Cycle
	End
End

#############################################################

End