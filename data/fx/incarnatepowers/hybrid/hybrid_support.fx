#############################################################
## Incandescence_Continuing.fx
#############################################################

FxInfo
Lifespan 10000
#############################################################

############################	WARM UP
Condition
	On 	Time
	Time	28
	Event
		Type	Local
		At	Root
		Sound 	Hybrid_Support 100 100 .8
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
		Bhvr behaviors/GenericParticleFade.bhvr
		Part 	:Lingering_Cloud_Out.part
		Lifespan 50
	End
End

Condition
	On	Time
	Time	35

	Event
		Type 	local
		At 	Chest
		Bhvr 	behaviors/GenericParticleFade.bhvr
		Childfx	:Hybrid_Support_Child.fx
		Lifespan 45
	End
End

Condition
	On	Cycle
	Time	120

	Event
		Type 	local
		At 	Chest
		Bhvr 	behaviors/GenericParticleFade.bhvr
		Childfx	:Hybrid_Support_Child.fx
		Lifespan 45
	End
End


##############################################################
End