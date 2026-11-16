#############################################################
## mu_LightningBlastHit.fx
#############################################################

FxInfo
LifeSpan	90

#############################################################

Input
	Inpname	Hips
End

#############################################################


#################################################################
##Hands
#################################################################

Condition

	On 	time
	Time 	0

	Event
		Type	Local

		At	origin

	Sound mu_elecballhit2 80 80 .9
	end
end


Condition
	On	Time
	Time	0

	Event
		Type	Posit
		At	origin
		ChildFX :CHILD_Grounder.fx
		lifespan 20
	End

	Event
		Type	local
		At	root
		Part1 :mu_Elec_Hit_residual1.part
		lifespan 10
		Pother	Chest
	End

	Event
		Type	local
		At	root
		childfx :CHILD_Body.fx
		lifespan 20
	End
End

Condition
	On	Time
	Time	1
	Event
		Type	Posit
		At	root
		ChildFX :CHILD_Grounder.fx
		lifespan 20
	End
	Event
		Type	local
		At	root
		Part1 :mu_Elec_Hit_residual1.part
		lifespan 10
		Pother	Chest
	End

End

Condition
	On	Time
	Time	2


	Event

		Type	start
		At	Origin

		Part1	:mu_shockring1.part
		lifespan	30
	End
End

Condition
	On	Time
	Time	0

	Event
		Type	local
		At	root
		ChildFX :CHILD_Body.fx
		lifespan 20
	End
End

Condition
	On	Time
	Time	15

	Event
		Type	local
		At	root
		ChildFX :CHILD_Body.fx
		lifespan 20
	End
End

Condition
	On	Time
	Time	30

	Event
		Type	local
		At	root
		ChildFX :CHILD_Body.fx
		lifespan 20
	End
End

Condition
	On	Time
	Time	45

	Event
		Type	local
		At	root
		ChildFX :CHILD_Body.fx
		lifespan 20
	End
End

Condition
	On	Time
	Time	60

	Event
		Type	local
		At	root
		ChildFX :CHILD_Body.fx
		lifespan 20
	End
End

Condition
	On	Time
	Time	75

	Event
		Type	local
		At	root
		ChildFX :CHILD_Body.fx
		lifespan 20
	End
End

Condition
	On	Time
	Time	90

	Event
		Type	local
		At	root
		ChildFX :CHILD_Body.fx
		lifespan 20
	End
End


End
