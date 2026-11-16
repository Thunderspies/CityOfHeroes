#############################################################
## Incandescence_Continuing.fx
#############################################################

FxInfo
#Lifespan 1000
#############################################################

############################	WARM UP
Condition
	On 	Time
	Time	25
	Event
		Type	Local
		At	Root
		Sound 	Hybrid_Assault 100 100 .8
	End
End

Condition
	On 	Time
	Time 	10

	Event
		Type	Local
		At	Root
		ChildFx	IncarnatePowers/Incarnate_Gather.fx
		Lifespan 45
	End
End
##############################################################

Condition
	On	Time
	Time	15

	Event
		Type 	local
		At 	Chest
		Part 	:Lingering_Cloud_Out.part
		Lifespan 50
	End
End

Condition
	On 	Time
	Time 	40

	Event
		Type	Posit
		At	Hips
		ChildFX	:Assault_Child.fx
		Lifespan 100
	End
End

Condition
	On 	Time
	Time 	120

	Event
		Type	Posit
		At	Hips
	#	ChildFX	:Assault_Child.fx
		Lifespan 100
	End
End

Condition
	On 	Cycle
	Time 	120

	Event
		Type	Posit
		At	Hips
		ChildFX	:Assault_Child.fx
		Lifespan 100
	End
End


##############################################################
End