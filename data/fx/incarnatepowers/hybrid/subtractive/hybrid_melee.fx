#############################################################
## Incandescence_Continuing.fx
#############################################################

FxInfo
Lifespan 100000
#############################################################

############################	WARM UP
Condition
	On 	Time
	Time	20
	Event
		Type	Local
		At	Root
		Sound 	Hybrid_Melee 100 100 .8
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
		Lifespan 30
	End
End

Condition
	On 	Time
	Time 	18

	Event
		Type	Local
		At	Chest
		ChildFX	:Hybrid_Melee_Child.fx
		Lifespan 40
	End
End

Condition
	On 	Cycle
	Time 	120

	Event
		Type	Local
		At	Chest
		Bhvr	Behaviors/GenericParticleFade.bhvr
		ChildFX	:Hybrid_Melee_Child.fx
		Lifespan 70
	End

	Event
		Type	PositOnly
		At	Chest
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part :FlashRing_Out.part
		Part :FlashRing_Add.part
		Lifespan 20
	End
End


##############################################################
End